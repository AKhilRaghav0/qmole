#!/usr/bin/env python

# This is a wrapper script around the zenmap executable, used in a Mac OS X .app
# bundle. It sets environment variables, fills in template configuration files,
# starts X11 if necessary, and execs the real zenmap executable.
#
# This program is the second link in the chain
#     zenmap_auth -> zenmap_wrapper.py -> zenmap.bin

import errno
import os
import os.path
import sys

HOME = os.path.expanduser("~")

def create_dir(path):
    """Create a directory with os.makedirs without raising an error if the
        directory already exists."""
    try:
        os.makedirs(path)
    except OSError, e:
        if e.errno != errno.EEXIST:
            raise

# We will need to rewrite some configuration files to refer to directories
# inside the application bundle, wherever it may be. This is tricky because of
# escaping issues in the formats of the configuration files. The following
# functions handle it.

# The format of pango/pangorc is called "key file." It's described at
# http://library.gnome.org/devel/glib/stable/glib-Key-value-file-parser.

# Escape a string as approprite for a "key file."
def escape_key_file_value(value):
    result = []
    for c in value:
        if c == "\n":
            c = "\\n"
        elif c == "\t":
            c = "\\t"
        elif c == "\r":
            c = "\\r"
        elif c == "\\":
            c = "\\\\"
        result.append(c)
    if len(result) > 0 and result[0] == " ":
        result[0] = "\\s"
    result = "".join(result)
    return result

def substitute_key_file_line(line, replacements):
    for text, rep in replacements.items():
        line = line.replace(text, escape_key_file_value(rep))
    return line

# Substitute a dict of replacements into a "key file."
def substitute_key_file(in_file_name, out_file_name, replacements):
    in_file = open(in_file_name, "r")
    out_file = open(out_file_name, "w")
    for line in in_file:
        out_file.write(substitute_key_file_line(line, replacements))
    in_file.close()
    out_file.close()

# The format of gtk-2.0/gdk-pixbuf.loaders, gtk-2.0/gtk.immodules, and
# pango/pango.modules is lines of whitespace-separated strings, possibly quoted.
# Split a line of strings into a list with no escaping or quoting.
def split_modules_file_line(line):
    parts = []
    i = 0
    while i < len(line):
        # Skip whitespace.
        while i < len(line) and line[i].isspace():
            i += 1
        if i >= len(line):
            break
        current = []
        if line[i] == "\"":
            i += 1
            backslash = False
            while i < len(line):
                c = line[i]
                if backslash:
                    if c == "n":
                        c = "\n"
                    elif c == "t":
                        c = "\t"
                    current.append(c)
                    backslash = False
                else:
                    if c == "\"":
                        break
                    elif c == "\\":
                        backslash = True
                    else:
                        current.append(c)
                i += 1
            if backslash:
                raise ValueError, "Escaped string ends with a backslash."
            if not (i < len(line) and line[i] == "\""):
                raise ValueError, "Unterminated quoted string: %d, %d" % (i, len(line))
            i += 1
        else:
            while i < len(line) and not line[i].isspace():
                current.append(line[i])
                i += 1
        current = "".join(current)
        parts.append(current)
    return parts

# Escape a string so it can be read by pango_scan_string. For simplicity this
# always quotes the string.
def escape_modules_file_value(value):
    result = []
    for c in value:
        if c == "\n":
            c = "\\n"
        elif c == "\"":
            c = "\\\""
        elif c == "\\":
            c = "\\\\"
        result.append(c)
    return "\"" + "".join(result) + "\""

# Substitute a dict of replacements into a line from a modules file, unescaping
# before the substitution and reescaping afterwards.
def substitute_modules_file_line(line, replacements):
    if not line.startswith("#"):
        parts = split_modules_file_line(line)
        out_parts = []
        for part in parts:
            for text, rep in replacements.items():
                part = part.replace(text, rep)
            out_parts.append(escape_modules_file_value(part))
        line = " ".join(out_parts) + "\n"
    return line

# Substitute a dict of replacements into a modules file.
def substitute_modules_file(in_file_name, out_file_name, replacements):
    in_file = open(in_file_name, "r")
    out_file = open(out_file_name, "w")
    for line in in_file:
        out_file.write(substitute_modules_file_line(line, replacements))
    in_file.close()
    out_file.close()

def escape_shell(arg):
    """Escape a string to be a shell argument."""
    result = []
    for c in arg:
        if c in "$`\"\\":
            c = "\\" + c
        result.append(c)
    return "\"" + "".join(result) + "\""

def hack_xinitrc(system_xinitrc_filename, home_xinitrc_filename):
    """Hack the system xinitrc file and put the modified contents into another
    file. The parameter names reflect the fact that this is intended to copy
    the system xinitrc into ~/.xinitrc. The modified xinitrc will delete itself
    on its first invocation and will not run any instances of xterm. This is
    necessary on Mac OS X 10.4 and earlier, which include a call to xterm in
    the system xinitrc."""
    system_xinitrc = open(system_xinitrc_filename, "r")
    home_xinitrc = open(home_xinitrc_filename, "w")
    lines = iter(system_xinitrc)
    # Look for the first non-comment line so we don't pre-empt the #! line.
    for line in lines:
        if not line.lstrip().startswith("#"):
            break
        home_xinitrc.write(line)
    # Write the self-destruct line.
    home_xinitrc.write("\n")
    home_xinitrc.write("rm -f %s\n" % escape_shell(home_xinitrc_filename))
    home_xinitrc.write(line)
    # Copy the rest, removing any calls to xterm
    for line in lines:
        if line.lstrip().startswith("xterm"):
            line = "# " + line
        home_xinitrc.write(line)
    system_xinitrc.close()
    home_xinitrc.close()

def start_x11():
    """Start the X11 server if necessary and set the DISPLAY environment as
    appropriate. If the user hasn't set up a custom ~/.xinitrc, call
    hack_xinitrc to make a ~/.xinitrc that will not start an xterm along with
    the application. A similar approach is taken by Wireshark and Inkscape."""
    if os.environ.has_key("DISPLAY"):
        return
    system_xinitrc_filename = "/usr/X11R6/lib/X11/xinit/xinitrc"
    home_xinitrc_filename = os.path.join(HOME, ".xinitrc")
    if not os.path.exists(home_xinitrc_filename):
        hack_xinitrc(system_xinitrc_filename, home_xinitrc_filename)
    os.system("open -a X11")
    os.environ["DISPLAY"] = ":0"

if __name__ == "__main__":
    # Make the real UID equal the effective UID. They are unequal when running
    # with privileges under AuthorizationExecuteWithPrivileges. GTK+ refuses to
    # run if they are different.
    if os.getuid() != os.geteuid():
        os.setuid(os.geteuid())

    # Paths within the application bundle.
    currentdir = os.path.dirname(os.path.abspath(sys.argv[0]))
    parentdir = os.path.dirname(currentdir)
    resourcedir = os.path.join(parentdir, "Resources")

    # A directory where we put automatically generated GTK+ and Pango files.
    # This could be something different like /tmp or "~/Library/Application
    # Support/Zenmap". It is put somewhere other than within the application
    # bundle to allow running from a read-only filesystem.
    etcdir = os.path.join(HOME, ".zenmap-etc")

    # Override the dynamic library search path. This makes the various GTK+ and
    # Pango shared objects look at the bundled copies of the libraries.  py2app
    # puts .dylibs in Contents/Frameworks.
    os.environ["DYLD_LIBRARY_PATH"] = os.path.join(parentdir, "Frameworks")

    # See http://library.gnome.org/devel/gtk/2.12/gtk-running.html for the
    # meaning of the GTK+ environment variables. These files are static and
    # live inside the application bundle.
    os.environ["GTK_DATA_PREFIX"] = resourcedir
    os.environ["GTK_EXE_PREFIX"] = resourcedir
    os.environ["GTK_PATH"] = resourcedir
    os.environ["FONTCONFIG_PATH"] = os.path.join(resourcedir, "etc", "fonts")

    # The following environment variables refer to files within ~/.zenmap-etc
    # that are automatically generated from templates.
    os.environ["GTK_IM_MODULE_FILE"] = os.path.join(etcdir, "gtk-2.0", "gtk.immodules")
    os.environ["GDK_PIXBUF_MODULE_FILE"] = os.path.join(etcdir, "gtk-2.0", "gdk-pixbuf.loaders")
    os.environ["PANGO_RC_FILE"] = os.path.join(etcdir, "pango", "pangorc")

    # Create the template directory.
    create_dir(os.path.join(etcdir, "gtk-2.0"))
    create_dir(os.path.join(etcdir, "pango"))

    REPLACEMENTS = {
        "${RESOURCES}": resourcedir,
        "${ETC}": etcdir
    }

    # Fill in the templated configuration files with the correct substitutions.
    KEY_FILE_TEMPLATES = (
        "pango/pangorc",
    )
    for f in KEY_FILE_TEMPLATES:
        in_file_name = os.path.join(resourcedir, "etc", f + ".in")
        out_file_name = os.path.join(etcdir, f)
        substitute_key_file(in_file_name, out_file_name, REPLACEMENTS)
    MODULES_FILE_TEMPLATES = (
        "pango/pango.modules",
        "gtk-2.0/gdk-pixbuf.loaders",
        "gtk-2.0/gtk.immodules"
    )
    for f in MODULES_FILE_TEMPLATES:
        in_file_name = os.path.join(resourcedir, "etc", f + ".in")
        out_file_name = os.path.join(etcdir, f)
        substitute_modules_file(in_file_name, out_file_name, REPLACEMENTS)

    start_x11()

    # exec the real program.
    os.execl(os.path.join(os.path.dirname(sys.argv[0]), "zenmap.bin"), *sys.argv)
