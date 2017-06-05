/*
 * awesome-menu.c - menu window for awesome
 *
 * Copyright © 2008 Julien Danjou <julien@danjou.info>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

/* asprintf() */
#define _GNU_SOURCE

#define CHUNK_SIZE 4096

#include <getopt.h>

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include <X11/Xutil.h>

#include "common/swindow.h"
#include "common/util.h"
#include "common/version.h"
#include "common/configopts.h"
#include "common/xutil.h"
#include "common/xscreen.c"

#define PROGNAME "awesome-menu"

#define CLEANMASK(mask) (mask & ~(globalconf.numlockmask | LockMask))

/** awesome-menu run status */
typedef enum
{
    /** Stop awesome-menu */
    STOP = 0,
    /** Run awesome-menu */
    RUN = 1,
    /** Stop awesome-menu and cancel any operation */
    CANCEL = 2
} status_t;

/** Is awesome-menu running ? */
static status_t status = RUN;

/** Import awesome config file format */
extern cfg_opt_t awesome_opts[];

/** Item_t typedef */
typedef struct item_t item_t;
/** Item_t structure */
struct item_t
{
    /** Data */
    char *data;
    /** Previous and next elems in item_t list */
    item_t *prev, *next;
    /** True if the item currently matches */
    Bool match;
};

/** Destructor for item structure
 * \param item item pointer
 */
static void
item_delete(item_t **item)
{
    p_delete(&(*item)->data);
    p_delete(item);
}

DO_SLIST(item_t, item, item_delete)

/** awesome-run global configuration structure */
typedef struct
{
    /** Display ref */
    Display *display;
    /** The window */
    SimpleWindow *sw;
    /** The draw contet */
    DrawCtx *ctx;
    /** Colors */
    struct
    {
        style_t normal;
        style_t focus;
    } styles;
    /** Numlock mask */
    unsigned int numlockmask;
    /** The text */
    char *text;
    /** The text when we asked to complete */
    char *text_complete;
    /** The text length */
    size_t text_size;
    /** Item list */
    item_t *items;
    /** Selected item */
    item_t *item_selected;
    /** What to do with the result text */
    char *exec;
    /** Prompt */
    char *prompt;
    /** String match mode */
    Bool match_string;
    /** Autocompletion mode */
    Bool autocomplete;
} AwesomeMenuConf;

static AwesomeMenuConf globalconf;

/** Exit with given exit code
 * \param exit_code exit code
 * \return never returns
 */
static void __attribute__ ((noreturn))
exit_help(int exit_code)
{
    FILE *outfile = (exit_code == EXIT_SUCCESS) ? stdout : stderr;
    fprintf(outfile, "Usage: %s [-c config] [-e command] <message>\n",
            PROGNAME);
    exit(exit_code);
}

/** Parse configuration file and fill up AwesomeMenuConf
 * data structures with configuration directives.
 * \param screen screen number
 * \param confpatharg configuration file pathname, or NULL if auto
 * \param menu_title menu title
 * \param geometry geometry to fill up with supplied information from
 *        configuration file
 * \return cfg_parse status
 */
static int
config_parse(int screen, const char *confpatharg,
             const char *menu_title, area_t *geometry)
{
    int ret, i;
    char *confpath;
    cfg_t *cfg, *cfg_menu = NULL, *cfg_screen = NULL,
          *cfg_styles, *cfg_menu_styles = NULL;

    if(!confpatharg)
        confpath = config_file();
    else
        confpath = a_strdup(confpatharg);

    cfg = cfg_new();

    switch((ret = cfg_parse(cfg, confpath)))
    {
      case CFG_FILE_ERROR:
        warn("parsing configuration file failed: %s\n", strerror(errno));
        break;
      case CFG_PARSE_ERROR:
        cfg_error(cfg, "W: awesome: parsing configuration file %s failed.\n", confpath);
        break;
    }

    if(ret)
        return ret;

    if(menu_title && !(cfg_menu = cfg_gettsec(cfg, "menu", menu_title)))
        warn("no definition for menu %s in configuration file: using default\n",
             menu_title);
        
    /* get global screen section */
    if(!(cfg_screen = cfg_getnsec(cfg, "screen", screen)))
        cfg_screen = cfg_getsec(cfg, "screen");

    if(cfg_menu)
    {
        cfg_menu_styles = cfg_getsec(cfg_menu, "styles");
        if((i = cfg_getint(cfg_menu, "x")) != (int) 0xffffffff)
           geometry->x = i;
        if((i = cfg_getint(cfg_menu, "y")) != (int) 0xffffffff)
           geometry->y = i;
        if((i = cfg_getint(cfg_menu, "width")) > 0)
           geometry->width = i;
        if((i = cfg_getint(cfg_menu, "height")) > 0)
           geometry->height = i;
        globalconf.match_string = cfg_getbool(cfg_menu, "match_string");
        globalconf.autocomplete = cfg_getbool(cfg_menu, "autocomplete");
    }

    if(cfg_screen
       && (cfg_styles = cfg_getsec(cfg_screen, "styles")))
    {
        /* Grab default styles */
        draw_style_init(globalconf.display, DefaultScreen(globalconf.display),
                        cfg_getsec(cfg_styles, "normal"),
                        &globalconf.styles.normal, NULL);
    
        draw_style_init(globalconf.display, DefaultScreen(globalconf.display),
                        cfg_getsec(cfg_styles, "focus"),
                        &globalconf.styles.focus, &globalconf.styles.normal);
    }

    /* Now grab menu styles if any */
    if(cfg_menu_styles)
    {
        draw_style_init(globalconf.display, DefaultScreen(globalconf.display),
                        cfg_getsec(cfg_menu_styles, "normal"),
                        &globalconf.styles.normal, NULL);

        draw_style_init(globalconf.display, DefaultScreen(globalconf.display),
                        cfg_getsec(cfg_menu_styles, "focus"),
                        &globalconf.styles.focus, &globalconf.styles.normal);
    }

    if(!globalconf.styles.normal.font)
        eprint("no default font available\n");

    p_delete(&confpath);

    return ret;
}

/** Return the last word for a text.
 * \param text the text to look into
 * \return a pointer to the last word position in text
 */
static char *
get_last_word(char *text)
{
    char *last_word;

    if((last_word = strrchr(text, ' ')))
        last_word++;
    else
        last_word = text;

    return last_word;
}

/** Fill the completion list for awesome-menu with file list.
 * \param directory directory to look into
 * \return always true
 */
static Bool
item_list_fill_file(const char *directory)
{
    char *cwd, *home, *user, *filename;
    const char *file;
    DIR *dir;
    struct dirent *dirinfo;
    item_t *item;
    ssize_t len, lenfile;
    struct passwd *passwd = NULL;
    struct stat st;

    item_list_wipe(&globalconf.items);

    if(!directory)
        cwd = a_strdup("./");
    else if(a_strlen(directory) > 1 && directory[0] == '~')
    {
        if(directory[1] == '/')
        {
            home = getenv("HOME");
            asprintf(&cwd, "%s%s", (home ? home : ""), directory + 1);
        }
        else
        {
            if(!(file = strchr(directory, '/')))
                file = directory + a_strlen(directory);
            len = (file - directory) + 1;
            user = p_new(char, len);
            a_strncpy(user, len, directory + 1, (file - directory) - 1);
            if((passwd = getpwnam(user)))
            {
                asprintf(&cwd, "%s%s", passwd->pw_dir, file);
                p_delete(&user);
            }
            else
            {
                p_delete(&user);
                return False;
            }
        }
    }
    else
        cwd = a_strdup(directory);

    if(!(dir = opendir(cwd)))
    {
        p_delete(&cwd);
        return False;
    }

    while((dirinfo = readdir(dir)))
    {
        item = p_new(item_t, 1);

        /* + 1 for \0 + 1 for / if directory */
        len = a_strlen(directory) + a_strlen(dirinfo->d_name) + 2;

        item->data = p_new(char, len);
        if(a_strlen(directory))
            a_strcpy(item->data, len, directory);
        a_strcat(item->data, len, dirinfo->d_name);

        lenfile = a_strlen(cwd) + a_strlen(dirinfo->d_name) + 2;

        filename = p_new(char, lenfile);
        a_strcpy(filename, lenfile, cwd);
        a_strcat(filename, lenfile, dirinfo->d_name);

        if(!stat(filename, &st) && S_ISDIR(st.st_mode))
            a_strcat(item->data, len, "/");

        p_delete(&filename);

        item_list_push(&globalconf.items, item);
    }

    closedir(dir);
    p_delete(&cwd);

    return True;
}

static void
complete(Bool reverse)
{
    int loop = 2;
    item_t *item = NULL;
    item_t *(*item_iter)(item_t **, item_t *) = item_list_next_cycle;

    if(reverse)
        item_iter = item_list_prev_cycle;

    if(globalconf.item_selected)
        item = item_iter(&globalconf.items, globalconf.item_selected);
    else
        item = globalconf.items;

    for(; item && loop; item = item_iter(&globalconf.items, item))
    {
        if(item->match)
        {
            a_strcpy(globalconf.text_complete,
                     globalconf.text_size - (globalconf.text_complete - globalconf.text),
                     item->data);
            globalconf.item_selected = item;
            return;
        }
        /* Since loop is 2, it will be 1 at first iter, and then 0 if we
         * get back before matching an item (i.e. no items match) to the
         * first elem: so it will break the loop, otherwise it loops for
         * ever
         */
        if(item == globalconf.items)
            loop--;
    }
}

/** Compute a match from completion list for word.
 * \param word the word to match
 */
static int
compute_match(const char *word)
{
    ssize_t len = a_strlen(word);
    item_t *item;
    int matches = 0;

    /* reset the selected item to NULL */
    globalconf.item_selected = NULL;

    if(len)
    {
        if(word[len - 1] == '/'
           || word[len - 1] == ' ')
            item_list_fill_file(word);

        for(item = globalconf.items; item; item = item->next)
            if((globalconf.match_string && strstr(item->data, word) != NULL) || (!globalconf.match_string && !a_strncmp(word, item->data, a_strlen(word))))
            {
                item->match = True;
                matches++;
            }
            else
                item->match = False;
    }
    else
    {
        if(a_strlen(globalconf.text))
            item_list_fill_file(NULL);
        for(item = globalconf.items; item; item = item->next)
        {
            item->match = True;
            matches++;
        }
    }

    return matches;

}

/* Why not? */
#define MARGIN 10

/** Redraw the menu. */
static void
redraw(void)
{
    item_t *item;
    area_t geometry = { 0, 0, 0, 0, NULL, NULL };
    Bool selected_item_is_drawn = False;
    int len, prompt_len, x_of_previous_item;
    style_t style;

    geometry.width = globalconf.sw->geometry.width;
    geometry.height = globalconf.sw->geometry.height;

    if(a_strlen(globalconf.prompt))
    {
        draw_text(globalconf.ctx, geometry, AlignLeft,
                  MARGIN, globalconf.prompt, globalconf.styles.focus);

        len = MARGIN * 2 + draw_textwidth(globalconf.display, globalconf.styles.focus.font, globalconf.prompt);
        geometry.x += len;
        geometry.width -= len;
    }

    draw_text(globalconf.ctx, geometry, AlignLeft,
              MARGIN, globalconf.text, globalconf.styles.normal);

    len = MARGIN * 2 + MAX(draw_textwidth(globalconf.display, globalconf.styles.normal.font, globalconf.text),
                           geometry.width / 20);
    geometry.x += len;
    geometry.width -= len;
    prompt_len = geometry.x;

    for(item = globalconf.items; item && geometry.width > 0; item = item->next)
        if(item->match)
        {
            style = item == globalconf.item_selected ? globalconf.styles.focus : globalconf.styles.normal;
            len = MARGIN + draw_textwidth(globalconf.display, style.font, item->data);
            if(item == globalconf.item_selected)
            {
                if(len > geometry.width)
                    break;
                else
                    selected_item_is_drawn = True;
            }
            draw_text(globalconf.ctx, geometry, AlignLeft,
                      MARGIN / 2, item->data, style);
            geometry.x += len;
            geometry.width -= len;
        }

    /* we have an item selected but not drawn, so redraw in the other side */
    if(globalconf.item_selected && !selected_item_is_drawn)
    {
        geometry.x = globalconf.sw->geometry.width;

        for(item = globalconf.item_selected; item; item = item_list_prev(&globalconf.items, item))
            if(item->match)
            {
                style = item == globalconf.item_selected ? globalconf.styles.focus : globalconf.styles.normal;
                x_of_previous_item = geometry.x;
                geometry.width = MARGIN + draw_textwidth(globalconf.display, style.font, item->data);
                geometry.x -= geometry.width;

                if(geometry.x < prompt_len)
                    break;

                draw_text(globalconf.ctx, geometry, AlignLeft,
                          MARGIN / 2, item->data, style);
            }

        if(item)
        {
            geometry.x = prompt_len;
            geometry.width = x_of_previous_item - prompt_len;
            draw_rectangle(globalconf.ctx, geometry, 1.0, True, globalconf.styles.normal.bg);
        }
    }
    else if(geometry.width)
        draw_rectangle(globalconf.ctx, geometry, 1.0, True, globalconf.styles.normal.bg);

    simplewindow_refresh_drawable(globalconf.sw, DefaultScreen(globalconf.display));
    XSync(globalconf.display, False);
}

/** Handle keypress event in awesome-menu.
 * \param e received XKeyEvent
 */
static void
handle_kpress(XKeyEvent *e)
{
    char buf[32];
    KeySym ksym;
    int num, matches;
    ssize_t len;
    size_t text_dst_len;

    len = a_strlen(globalconf.text);
    num = XLookupString(e, buf, sizeof(buf), &ksym, 0);
    if(e->state & ControlMask)
        switch(ksym)
        {
          default:
            return;
          case XK_bracketleft:
            ksym = XK_Escape;
            break;
          case XK_h:
          case XK_H:
            ksym = XK_BackSpace;
            break;
          case XK_i:
          case XK_I:
            ksym = XK_Tab;
            break;
          case XK_j:
          case XK_J:
            ksym = XK_Return;
            break;
          case XK_c:
          case XK_C:
            status = CANCEL;
            break;
          case XK_w:
          case XK_W:
            /* erase word */
            if(len)
            {
                int i = len - 1;
                while(i >= 0 && globalconf.text[i] == ' ')
                    globalconf.text[i--] = '\0';
                while(i >= 0 && globalconf.text[i] != ' ')
                    globalconf.text[i--] = '\0';
                matches = compute_match(get_last_word(globalconf.text));
                if (globalconf.autocomplete && matches == 1)
                    complete(False);
                redraw();
            }
            return;
        }
    else if(CLEANMASK(e->state) & Mod1Mask)
        switch(ksym)
        {
          default:
            return;
          case XK_h:
            ksym = XK_Left;
            break;
          case XK_l:
            ksym = XK_Right;
            break;
        }

    switch(ksym)
    {
      case XK_space:
        globalconf.text_complete = globalconf.text + a_strlen(globalconf.text) + 1;
      default:
        if(num && !iscntrl((int) buf[0]))
        {
            if(buf[0] != '/' || globalconf.text[len - 1] != '/')
            {
                buf[num] = '\0';

                /* Reallocate text string if needed to hold
                 * concatenation of text and buf */
                if((text_dst_len = (a_strlen(globalconf.text) + num - 1)) > globalconf.text_size)
                {
                    globalconf.text_size += ((int) (text_dst_len / globalconf.text_size)) * CHUNK_SIZE;
                    p_realloc(&globalconf.text, globalconf.text_size);
                }
                a_strncat(globalconf.text, globalconf.text_size, buf, num);
            }
            matches = compute_match(get_last_word(globalconf.text));
            if (globalconf.autocomplete && matches == 1)
                complete(False);
            redraw();
        }
        break;
      case XK_BackSpace:
        if(len)
        {
            globalconf.text[--len] = '\0';
            matches = compute_match(get_last_word(globalconf.text));
            redraw();
        }
        break;
      case XK_ISO_Left_Tab:
      case XK_Left:
        complete(True);
        redraw();
        break;
      case XK_Right:
      case XK_Tab:
        complete(False);
        redraw();
        break;
      case XK_Escape:
        status= CANCEL;
        break;
      case XK_Return:
        status = STOP;
        break;
    }
}

#ifndef HAVE_GETLINE
static int
getline(char ** buf, size_t* len, FILE* in)
{
    int i;
    if (*buf) {
        p_delete(buf);
        (*buf) = NULL;
    }
    if (*len)
        *len = 0;

    do {
        p_realloc(buf, *len + 10);
        (*len) += 10;
        for (i = 0; i < 10 && !feof(in); i++) {
            (*buf)[*len - 10 + i] = getchar();
            if ((*buf)[*len - 10 + i] == '\n' ||
                (*buf)[*len - 10 + i] == '\r') {
                return (*len - 10 + i + 1);
            }
        }
    }
    while(!feof(in));
    return -1;
}
#endif

/** Fill the completion by reading on stdin.
 * \return true if something has been filled up, false otherwise.
 */
static Bool
item_list_fill_stdin(void)
{
    char *buf = NULL;
    size_t len = 0;
    ssize_t line_len;

    item_t *newitem;
    Bool has_entry = False;

    item_list_init(&globalconf.items);

    if((line_len = getline(&buf, &len, stdin)) != -1)
        has_entry = True;

    if(has_entry)
        do
        {
            buf[line_len - 1] = '\0';
            newitem = p_new(item_t, 1);
            newitem->data = a_strdup(buf);
            newitem->match = True;
            item_list_append(&globalconf.items, newitem);
        }
        while((line_len = getline(&buf, &len, stdin)) != -1);

    if(buf)
        p_delete(&buf);

    return has_entry;
}

/** Grab the keyboard.
 */
static void
keyboard_grab(void)
{
    int i;
    for(i = 1000; i; i--)
    {
        if(XGrabKeyboard(globalconf.display, DefaultRootWindow(globalconf.display), True,
                         GrabModeAsync, GrabModeAsync, CurrentTime) == GrabSuccess)
            break;
        usleep(1000);
    }
    if(!i)
        eprint("cannot grab keyboard");
}

/** Main function of awesome-menu.
 * \param argc number of elements in argv
 * \param argv arguments array
 * \return EXIT_SUCCESS
 */
int
main(int argc, char **argv)
{
    XEvent ev;
    int opt, ret, x, y, i, screen = 0, matches;
    char *configfile = NULL, *cmd;
    ssize_t len;
    const char *shell = getenv("SHELL");
    area_t geometry = { 0, 0, 0, 0, NULL, NULL };
    ScreensInfo *si;
    unsigned int ui;
    Window dummy;
    static struct option long_options[] =
    {
        {"help",    0, NULL, 'h'},
        {"version", 0, NULL, 'v'},
        {"exec",    0, NULL, 'e'},
        {NULL,      0, NULL, 0}
    };

    while((opt = getopt_long(argc, argv, "vhf:b:x:y:n:c:e:",
                             long_options, NULL)) != -1)
        switch(opt)
        {
          case 'v':
            eprint_version(PROGNAME);
            break;
          case 'h':
            exit_help(EXIT_SUCCESS);
            break;
          case 'c':
            configfile = a_strdup(optarg);
            break;
          case 'e':
            globalconf.exec = a_strdup(optarg);
            break;
        }

    if(argc - optind >= 1)
        globalconf.prompt = a_strdup(argv[optind]);

    if(!(globalconf.display = XOpenDisplay(NULL)))
        eprint("unable to open display");

    /* Get the numlock mask */
    globalconf.numlockmask = xgetnumlockmask(globalconf.display);

    si = screensinfo_new(globalconf.display);
    if(si->xinerama_is_active)
    {
        if(XQueryPointer(globalconf.display, RootWindow(globalconf.display, DefaultScreen(globalconf.display)),
                         &dummy, &dummy, &x, &y, &i, &i, &ui))
        {
            screen = screen_get_bycoord(si, 0, x, y);

            geometry.x = si->geometry[screen].x;
            geometry.y = si->geometry[screen].y;
            geometry.width = si->geometry[screen].width;
        }
    }
    else
    {
        screen = DefaultScreen(globalconf.display);
        if(!geometry.width)
            geometry.width = DisplayWidth(globalconf.display, DefaultScreen(globalconf.display));
    }

    if((ret = config_parse(screen, configfile, globalconf.prompt, &geometry)))
        return ret;

    /* Init the geometry */
    if(!geometry.height)
        geometry.height = 1.5 * MAX(globalconf.styles.normal.font->height,
                                    globalconf.styles.focus.font->height);

    screensinfo_delete(&si);

    /* Create the window */
    globalconf.sw = simplewindow_new(globalconf.display, DefaultScreen(globalconf.display),
                                     geometry.x, geometry.y, geometry.width, geometry.height, 0);

    XStoreName(globalconf.display, globalconf.sw->window, PROGNAME);

    /* Create the drawing context */
    globalconf.ctx = draw_context_new(globalconf.display, DefaultScreen(globalconf.display),
                                      geometry.width, geometry.height,
                                      globalconf.sw->drawable);

    /* Allocate a default size for the text on the heap instead of
     * using stack allocation with PATH_MAX (may not been defined
     * according to POSIX). This string size may be increased if
     * needed */
    globalconf.text_complete = globalconf.text = p_new(char, CHUNK_SIZE);
    globalconf.text_size = CHUNK_SIZE;

    if(isatty(STDIN_FILENO))
    {
        if(!item_list_fill_stdin())
            item_list_fill_file(NULL);
        keyboard_grab();
    }
    else
    {
        keyboard_grab();
        if(!item_list_fill_stdin())
            item_list_fill_file(NULL);
    }

    matches = compute_match(NULL);
    if (globalconf.autocomplete && matches == 1)
        complete(False);

    redraw();

    XMapRaised(globalconf.display, globalconf.sw->window);

    while(status == RUN)
    {
        XNextEvent(globalconf.display, &ev);
        switch(ev.type)
        {
          case ButtonPress:
            status = CANCEL;
            break;
          case KeyPress:
            handle_kpress(&ev.xkey);
            break;
          case Expose:
            if(!ev.xexpose.count)
                simplewindow_refresh_drawable(globalconf.sw, DefaultScreen(globalconf.display));
            break;
          default:
            break;
        }
    }

    if(status != CANCEL)
    {
        if(!globalconf.exec)
            printf("%s\n", globalconf.text);
        else
        {
            if(!shell)
                shell = a_strdup("/bin/sh");
            len = a_strlen(globalconf.exec) + a_strlen(globalconf.text) + 1;
            cmd = p_new(char, len);
            a_strcpy(cmd, len, globalconf.exec);
            a_strcat(cmd, len, globalconf.text);
            execl(shell, shell, "-c", cmd, NULL);
        }
    }

    p_delete(&globalconf.text);
    draw_context_delete(&globalconf.ctx);
    simplewindow_delete(&globalconf.sw);
    XCloseDisplay(globalconf.display);

    return EXIT_SUCCESS;
}

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
