/*
 * Loosely based on code bearing the following copyright:
 *
 *   Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 */

/*
 * Copyright 1992-2003 by The XFree86 Project, Inc.
 * Copyright 1997 by Metro Link, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/*
 *
 * Authors:
 *	Dirk Hohndel <hohndel@XFree86.Org>
 *	David Dawes <dawes@XFree86.Org>
 *      Marc La France <tsi@XFree86.Org>
 *      Egbert Eich <eich@XFree86.Org>
 *      ... and others
 */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#ifdef XF86DRI
#include <sys/types.h>
#include <grp.h>
#endif

#include "xf86.h"
#include "xf86Parser.h"
#include "xf86tokens.h"
#include "xf86Config.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"
#include "configProcs.h"
#include "globals.h"
#include "extension.h"
#include "Pci.h"

#ifdef XINPUT
#include "xf86Xinput.h"
extern DeviceAssocRec mouse_assoc;
#endif

#ifdef XKB
#undef XKB_IN_SERVER
#define XKB_IN_SERVER
#include <xkbsrv.h>
#endif

#ifdef RENDER
#include "picture.h"
#endif

#if (defined(__i386__)) && \
    (defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || \
     defined(__NetBSD__) || defined(linux) || \
     (defined(SVR4) && !defined(sun)) || defined(__GNU__))
#define SUPPORT_PC98
#endif

/*
 * These paths define the way the config file search is done.  The escape
 * sequences are documented in parser/scan.c.
 */
#ifndef ROOT_CONFIGPATH
#define ROOT_CONFIGPATH	"%A," "%R," \
			"/etc/X11/%R," "%P/etc/X11/%R," \
			"%E," "%F," \
			"/etc/X11/%F," "%P/etc/X11/%F," \
			"/etc/X11/%X-%M," "/etc/X11/%X," "/etc/%X," \
			"%P/etc/X11/%X.%H," "%P/etc/X11/%X-%M," \
			"%P/etc/X11/%X," \
			"%P/lib/X11/%X.%H," "%P/lib/X11/%X-%M," \
			"%P/lib/X11/%X"
#endif
#ifndef USER_CONFIGPATH
#define USER_CONFIGPATH	"/etc/X11/%S," "%P/etc/X11/%S," \
			"/etc/X11/%G," "%P/etc/X11/%G," \
			"/etc/X11/%X-%M," "/etc/X11/%X," "/etc/%X," \
			"%P/etc/X11/%X.%H," "%P/etc/X11/%X-%M," \
			"%P/etc/X11/%X," \
			"%P/lib/X11/%X.%H," "%P/lib/X11/%X-%M," \
			"%P/lib/X11/%X"
#endif
#ifndef PROJECTROOT
#define PROJECTROOT	"/usr/X11R6"
#endif

static char *fontPath = NULL;

static ModuleDefault ModuleDefaults[] = {
    {.name = "extmod",   .toLoad = TRUE,    .load_opt=NULL},
    {.name = "dbe",      .toLoad = TRUE,    .load_opt=NULL},
    {.name = "glx",      .toLoad = TRUE,    .load_opt=NULL},
    {.name = "freetype", .toLoad = TRUE,    .load_opt=NULL},
#ifdef XRECORD
    {.name = "record",   .toLoad = TRUE,    .load_opt=NULL},
#endif
    {.name = "dri",      .toLoad = TRUE,    .load_opt=NULL},
#ifdef DRI2
    {.name = "dri2",     .toLoad = TRUE,    .load_opt=NULL},
#endif
    {.name = NULL,       .toLoad = FALSE,   .load_opt=NULL}
};


/* Forward declarations */
static Bool configScreen(confScreenPtr screenp, XF86ConfScreenPtr conf_screen,
			 int scrnum, MessageType from);
static Bool configMonitor(MonPtr monitorp, XF86ConfMonitorPtr conf_monitor);
static Bool configDevice(GDevPtr devicep, XF86ConfDevicePtr conf_device,
			 Bool active);
static Bool configInput(IDevPtr inputp, XF86ConfInputPtr conf_input,
			MessageType from);
static Bool configDisplay(DispPtr displayp, XF86ConfDisplayPtr conf_display);
static Bool addDefaultModes(MonPtr monitorp);
#ifdef XF86DRI
static void configDRI(XF86ConfDRIPtr drip);
#endif
static void configExtensions(XF86ConfExtensionsPtr conf_ext);

/*
 * xf86GetPathElem --
 *	Extract a single element from the font path string starting at
 *	pnt.  The font path element will be returned, and pnt will be
 *	updated to point to the start of the next element, or set to
 *	NULL if there are no more.
 */
static char *
xf86GetPathElem(char **pnt)
{
  char *p1;

  p1 = *pnt;
  *pnt = index(*pnt, ',');
  if (*pnt != NULL) {
    **pnt = '\0';
    *pnt += 1;
  }
  return(p1);
}

/*
 * xf86ValidateFontPath --
 *	Validates the user-specified font path.  Each element that
 *	begins with a '/' is checked to make sure the directory exists.
 *	If the directory exists, the existence of a file named 'fonts.dir'
 *	is checked.  If either check fails, an error is printed and the
 *	element is removed from the font path.
 */

#define DIR_FILE "/fonts.dir"
static char *
xf86ValidateFontPath(char *path)
{
  char *tmp_path, *out_pnt, *path_elem, *next, *p1, *dir_elem;
  struct stat stat_buf;
  int flag;
  int dirlen;

  tmp_path = xcalloc(1,strlen(path)+1);
  out_pnt = tmp_path;
  path_elem = NULL;
  next = path;
  while (next != NULL) {
    path_elem = xf86GetPathElem(&next);
    if (*path_elem == '/') {
      dir_elem = xnfcalloc(1, strlen(path_elem) + 1);
      if ((p1 = strchr(path_elem, ':')) != 0)
	dirlen = p1 - path_elem;
      else
	dirlen = strlen(path_elem);
      strncpy(dir_elem, path_elem, dirlen);
      dir_elem[dirlen] = '\0';
      flag = stat(dir_elem, &stat_buf);
      if (flag == 0)
	if (!S_ISDIR(stat_buf.st_mode))
	  flag = -1;
      if (flag != 0) {
        xf86Msg(X_WARNING, "The directory \"%s\" does not exist.\n", dir_elem);
	xf86ErrorF("\tEntry deleted from font path.\n");
	xfree(dir_elem);
	continue;
      }
      else {
	p1 = xnfalloc(strlen(dir_elem)+strlen(DIR_FILE)+1);
	strcpy(p1, dir_elem);
	strcat(p1, DIR_FILE);
	flag = stat(p1, &stat_buf);
	if (flag == 0)
	  if (!S_ISREG(stat_buf.st_mode))
	    flag = -1;
	xfree(p1);
	if (flag != 0) {
	  xf86Msg(X_WARNING,
		  "`fonts.dir' not found (or not valid) in \"%s\".\n", 
		  dir_elem);
	  xf86ErrorF("\tEntry deleted from font path.\n");
	  xf86ErrorF("\t(Run 'mkfontdir' on \"%s\").\n", dir_elem);
	  xfree(dir_elem);
	  continue;
	}
      }
      xfree(dir_elem);
    }

    /*
     * Either an OK directory, or a font server name.  So add it to
     * the path.
     */
    if (out_pnt != tmp_path)
      *out_pnt++ = ',';
    strcat(out_pnt, path_elem);
    out_pnt += strlen(path_elem);
  }
  return(tmp_path);
}


/*
 * use the datastructure that the parser provides and pick out the parts
 * that we need at this point
 */
char **
xf86ModulelistFromConfig(pointer **optlist)
{
    int count = 0, i = 0;
    char **modulearray;
    char *ignore[] = { "GLcore", "speedo", "bitmap", "drm", NULL };
    pointer *optarray;
    XF86LoadPtr modp;
    Bool found;
    
    /*
     * make sure the config file has been parsed and that we have a
     * ModulePath set; if no ModulePath was given, use the default
     * ModulePath
     */
    if (xf86configptr == NULL) {
        xf86Msg(X_ERROR, "Cannot access global config data structure\n");
        return NULL;
    }
    
    if (xf86configptr->conf_modules) {
        /* Walk the disable list and let people know what we've parsed to
         * not be loaded 
         */
        modp = xf86configptr->conf_modules->mod_disable_lst;
        while (modp) {
            xf86Msg(X_WARNING, "\"%s\" will not be loaded unless you've specified it to be loaded elsewhere.\n", modp->load_name);
	        modp = (XF86LoadPtr) modp->list.next;
        }
        /*
         * Walk the default settings table. For each module listed to be
         * loaded, make sure it's in the mod_load_lst. If it's not, make
         * sure it's not in the mod_no_load_lst. If it's not disabled,
         * append it to mod_load_lst
         */
         for (i=0 ; ModuleDefaults[i].name != NULL ; i++) {
            if (ModuleDefaults[i].toLoad == FALSE) {
                xf86Msg(X_WARNING, "\"%s\" is not to be loaded by default. Skipping.\n", ModuleDefaults[i].name);
                continue;
            }
            found = FALSE;
            modp = xf86configptr->conf_modules->mod_load_lst;
            while (modp) {
                if (strcmp(modp->load_name, ModuleDefaults[i].name) == 0) {
                    xf86Msg(X_INFO, "\"%s\" will be loaded. This was enabled by default and also specified in the config file.\n", ModuleDefaults[i].name);
                    found = TRUE;
                    break;
                }
	        modp = (XF86LoadPtr) modp->list.next;
            }
            if (found == FALSE) {
                modp = xf86configptr->conf_modules->mod_disable_lst;
                while (modp) {
                    if (strcmp(modp->load_name, ModuleDefaults[i].name) == 0) {
                        xf86Msg(X_INFO, "\"%s\" will be loaded even though the default is to disable it.\n", ModuleDefaults[i].name);
                        found = TRUE;
                        break;
                    }
	                modp = (XF86LoadPtr) modp->list.next;
                }
            }
            if (found == FALSE) {
	            XF86ConfModulePtr ptr = xf86configptr->conf_modules;
	            ptr = xf86addNewLoadDirective(ptr, ModuleDefaults[i].name, XF86_LOAD_MODULE, ModuleDefaults[i].load_opt);
                xf86Msg(X_INFO, "\"%s\" will be loaded by default.\n", ModuleDefaults[i].name);
            }
         }
    } else {
	xf86configptr->conf_modules = xnfcalloc(1, sizeof(XF86ConfModuleRec));
	for (i=0 ; ModuleDefaults[i].name != NULL ; i++) {
	    if (ModuleDefaults[i].toLoad == TRUE) {
		XF86ConfModulePtr ptr = xf86configptr->conf_modules;
		ptr = xf86addNewLoadDirective(ptr, ModuleDefaults[i].name, XF86_LOAD_MODULE, ModuleDefaults[i].load_opt);
	    }
	}
    }

	    /*
	     * Walk the list of modules in the "Module" section to determine how
	     * many we have.
	    */
	    modp = xf86configptr->conf_modules->mod_load_lst;
	    while (modp) {
                for (i = 0; ignore[i]; i++) {
                    if (strcmp(modp->load_name, ignore[i]) == 0)
                        modp->ignore = 1;
                }
                if (!modp->ignore)
	            count++;
	        modp = (XF86LoadPtr) modp->list.next;
	    }

    /*
     * allocate the memory and walk the list again to fill in the pointers
     */
    modulearray = xnfalloc((count + 1) * sizeof(char*));
    optarray = xnfalloc((count + 1) * sizeof(pointer));
    count = 0;
    if (xf86configptr->conf_modules) {
	    modp = xf86configptr->conf_modules->mod_load_lst;
	    while (modp) {
            if (!modp->ignore) {
	            modulearray[count] = modp->load_name;
	            optarray[count] = modp->load_opt;
	            count++;
            }
	        modp = (XF86LoadPtr) modp->list.next;
	    }
    }
    modulearray[count] = NULL;
    optarray[count] = NULL;
    if (optlist)
	    *optlist = optarray;
    else
	    xfree(optarray);
    return modulearray;
}


char **
xf86DriverlistFromConfig()
{
    int count = 0;
    int j;
    char **modulearray;
    screenLayoutPtr slp;
    
    /*
     * make sure the config file has been parsed and that we have a
     * ModulePath set; if no ModulePath was given, use the default
     * ModulePath
     */
    if (xf86configptr == NULL) {
        xf86Msg(X_ERROR, "Cannot access global config data structure\n");
        return NULL;
    }
    
    /*
     * Walk the list of driver lines in active "Device" sections to
     * determine now many implicitly loaded modules there are.
     *
     */
    if (xf86ConfigLayout.screens) {
        slp = xf86ConfigLayout.screens;
        while ((slp++)->screen) {
	    count++;
        }
    }

    /*
     * Handle the set of inactive "Device" sections.
     */
    j = 0;
    while (xf86ConfigLayout.inactives[j++].identifier)
	count++;

    if (count == 0)
	return NULL;

    /*
     * allocate the memory and walk the list again to fill in the pointers
     */
    modulearray = xnfalloc((count + 1) * sizeof(char*));
    count = 0;
    slp = xf86ConfigLayout.screens;
    while (slp->screen) {
	modulearray[count] = slp->screen->device->driver;
	count++;
	slp++;
    }

    j = 0;

    while (xf86ConfigLayout.inactives[j].identifier) 
	modulearray[count++] = xf86ConfigLayout.inactives[j++].driver;

    modulearray[count] = NULL;

    /* Remove duplicates */
    for (count = 0; modulearray[count] != NULL; count++) {
	int i;

	for (i = 0; i < count; i++)
	    if (xf86NameCmp(modulearray[i], modulearray[count]) == 0) {
		modulearray[count] = "";
		break;
	    }
    }
    return modulearray;
}

char **
xf86InputDriverlistFromConfig()
{
    int count = 0;
    char **modulearray;
    IDevPtr* idp;
    
    /*
     * make sure the config file has been parsed and that we have a
     * ModulePath set; if no ModulePath was given, use the default
     * ModulePath
     */
    if (xf86configptr == NULL) {
        xf86Msg(X_ERROR, "Cannot access global config data structure\n");
        return NULL;
    }
    
    /*
     * Walk the list of driver lines in active "InputDevice" sections to
     * determine now many implicitly loaded modules there are.
     */
    if (xf86ConfigLayout.inputs) {
        idp = xf86ConfigLayout.inputs;
        while (*idp) {
	    count++;
	    idp++;
        }
    }

    if (count == 0)
	return NULL;

    /*
     * allocate the memory and walk the list again to fill in the pointers
     */
    modulearray = xnfalloc((count + 1) * sizeof(char*));
    count = 0;
    idp = xf86ConfigLayout.inputs;
    while (idp && *idp) {
        modulearray[count] = (*idp)->driver;
	count++;
	idp++;
    }
    modulearray[count] = NULL;

    /* Remove duplicates */
    for (count = 0; modulearray[count] != NULL; count++) {
	int i;

	for (i = 0; i < count; i++)
	    if (xf86NameCmp(modulearray[i], modulearray[count]) == 0) {
		modulearray[count] = "";
		break;
	    }
    }
    return modulearray;
}

static void
fixup_video_driver_list(char **drivers)
{
    static const char *fallback[4] = { "vesa", "fbdev", "wsfb", NULL };
    char **end, **drv;
    char *x;
    char **ati, **atimisc;
    int i;

    /* walk to the end of the list */
    for (end = drivers; *end && **end; end++) ;
    end--;

    /*
     * for each of the fallback drivers, if we find it in the list,
     * swap it with the last available non-fallback driver.
     */
    for (i = 0; fallback[i]; i++) {
        for (drv = drivers; drv != end; drv++) {
            if (strstr(*drv, fallback[i])) {
                x = *drv; *drv = *end; *end = x;
                end--;
                break;
            }
        }
    }
    /*
     * since the ati wrapper driver is gross and awful, sort ati before
     * atimisc, which makes sure all the ati symbols are visible in xorgcfg.
     */
    for (drv = drivers; drv != end; drv++) {
        if (!strcmp(*drv, "atimisc")) {
            atimisc = drv;
            for (drv = atimisc; drv != end; drv++) {
                if (!strcmp(*drv, "ati")) {
                    ati = drv;
                    x = *ati; *ati = *atimisc; *atimisc = x;
                    return;
                }
            }
            /* if we get here, ati was already ahead of atimisc */
            return;
        }
    }
}

static char **
GenerateDriverlist(char * dirname)
{
    char **ret;
    const char *subdirs[] = { dirname, NULL };
    static const char *patlist[] = {"(.*)_drv\\.so", "(.*)_drv\\.o", NULL};
    ret = LoaderListDirs(subdirs, patlist);
    
    /* fix up the probe order for video drivers */
    if (strstr(dirname, "drivers") && ret != NULL)
        fixup_video_driver_list(ret);

    return ret;
}

char **
xf86DriverlistFromCompile(void)
{
    static char **driverlist = NULL;

    if (!driverlist)
        driverlist = GenerateDriverlist("drivers");

    return driverlist;
}

/*
 * xf86ConfigError --
 *      Print a READABLE ErrorMessage!!!  All information that is 
 *      available is printed.
 */
static void
xf86ConfigError(char *msg, ...)
{
    va_list ap;

    ErrorF("\nConfig Error:\n");
    va_start(ap, msg);
    VErrorF(msg, ap);
    va_end(ap);
    ErrorF("\n");
    return;
}

static void
configFiles(XF86ConfFilesPtr fileconf)
{
  MessageType pathFrom = X_DEFAULT;
  int countDirs;
  char *temp_path;
  char *log_buf;

  /* FontPath */
  /* Try XF86Config FontPath first */
  if (!xf86fpFlag) {
   if (fileconf) {
    if (fileconf->file_fontpath) {
      char *f = xf86ValidateFontPath(fileconf->file_fontpath);
      pathFrom = X_CONFIG;
      if (*f) {
        if (xf86Info.useDefaultFontPath) {
          xf86Msg(X_DEFAULT, "Including the default font path %s.\n", defaultFontPath);
          char *g = xnfalloc(strlen(defaultFontPath) + strlen(f) + 3);
          strcpy(g, f);
          strcat(g, ",");
          defaultFontPath = strcat(g, defaultFontPath);
          xfree(f);
        } else {
          defaultFontPath = f;
        }
      } else {
	xf86Msg(X_WARNING,
	    "FontPath is completely invalid.  Using compiled-in default.\n");
        fontPath = NULL;
        pathFrom = X_DEFAULT;
      }
    } 
   } else {
      xf86Msg(X_DEFAULT,
	    "No FontPath specified.  Using compiled-in default.\n");
      pathFrom = X_DEFAULT;
   }
  } else {
    /* Use fontpath specified with '-fp' */
    if (fontPath)
    {
      fontPath = NULL;
    }
    pathFrom = X_CMDLINE;
  }
  if (!fileconf) {
      /* xf86ValidateFontPath will write into it's arg, but defaultFontPath
       could be static, so we make a copy. */
    char *f = xnfalloc(strlen(defaultFontPath) + 1);
    f[0] = '\0';
    strcpy (f, defaultFontPath);
    defaultFontPath = xf86ValidateFontPath(f);
    xfree(f);
  } else {
   if (fileconf) {
    if (!fileconf->file_fontpath) {
      /* xf86ValidateFontPath will write into it's arg, but defaultFontPath
       could be static, so we make a copy. */
     char *f = xnfalloc(strlen(defaultFontPath) + 1);
     f[0] = '\0';
     strcpy (f, defaultFontPath);
     defaultFontPath = xf86ValidateFontPath(f);
     xfree(f);
    }
   }
  }

  /* If defaultFontPath is still empty, exit here */

  if (! *defaultFontPath)
    FatalError("No valid FontPath could be found.");

  /* make fontpath more readable in the logfiles */
  countDirs = 1;
  temp_path = defaultFontPath;
  while((temp_path = index(temp_path, ',')) != NULL) {
    countDirs++;
    temp_path++;
  }
  log_buf = xnfalloc(strlen(defaultFontPath) + (2 * countDirs) + 1);
  if(!log_buf) /* fallback to old method */
    xf86Msg(pathFrom, "FontPath set to \"%s\"\n", defaultFontPath);
  else {
    char *start, *end;
    int size;
    temp_path = log_buf;
    start = defaultFontPath;
    while((end = index(start, ',')) != NULL) {
      size = (end - start) + 1;
      *(temp_path++) = '\t';
      strncpy(temp_path, start, size);
      temp_path += size;
      *(temp_path++) = '\n';
      start += size;
    }
    /* copy last entry */
    *(temp_path++) = '\t';
    strcpy(temp_path, start);
    xf86Msg(pathFrom, "FontPath set to:\n%s\n", log_buf);
    xfree(log_buf);
  }


  if (fileconf && fileconf->file_inputdevs) {
      xf86InputDeviceList = fileconf->file_inputdevs;
      xf86Msg(X_CONFIG, "Input device list set to \"%s\"\n",
	  xf86InputDeviceList);
  }
  
  
  /* ModulePath */

  if (fileconf) {
    if (xf86ModPathFrom != X_CMDLINE && fileconf->file_modulepath) {
      xf86ModulePath = fileconf->file_modulepath;
      xf86ModPathFrom = X_CONFIG;
    }
  }

  xf86Msg(xf86ModPathFrom, "ModulePath set to \"%s\"\n", xf86ModulePath);

#if 0
  /* LogFile */
  /*
   * XXX The problem with this is that the log file is already open.
   * One option might be to copy the exiting contents to the new location.
   * and re-open it.  The down side is that the default location would
   * already have been overwritten.  Another option would be to start with
   * unique temporary location, then copy it once the correct name is known.
   * A problem with this is what happens if the server exits before that
   * happens.
   */
  if (xf86LogFileFrom == X_DEFAULT && fileconf->file_logfile) {
    xf86LogFile = fileconf->file_logfile;
    xf86LogFileFrom = X_CONFIG;
  }
#endif

  return;
}

typedef enum {
    FLAG_NOTRAPSIGNALS,
    FLAG_DONTVTSWITCH,
    FLAG_DONTZAP,
    FLAG_DONTZOOM,
    FLAG_DISABLEVIDMODE,
    FLAG_ALLOWNONLOCAL,
    FLAG_DISABLEMODINDEV,
    FLAG_MODINDEVALLOWNONLOCAL,
    FLAG_ALLOWMOUSEOPENFAIL,
    FLAG_VTSYSREQ,
    FLAG_XKBDISABLE,
    FLAG_PCIPROBE1,
    FLAG_PCIPROBE2,
    FLAG_PCIFORCECONFIG1,
    FLAG_PCIFORCECONFIG2,
    FLAG_PCIFORCENONE,
    FLAG_PCIOSCONFIG,
    FLAG_SAVER_BLANKTIME,
    FLAG_DPMS_STANDBYTIME,
    FLAG_DPMS_SUSPENDTIME,
    FLAG_DPMS_OFFTIME,
    FLAG_PIXMAP,
    FLAG_PC98,
    FLAG_ESTIMATE_SIZES_AGGRESSIVELY,
    FLAG_NOPM,
    FLAG_XINERAMA,
    FLAG_ALLOW_DEACTIVATE_GRABS,
    FLAG_ALLOW_CLOSEDOWN_GRABS,
    FLAG_LOG,
    FLAG_RENDER_COLORMAP_MODE,
    FLAG_HANDLE_SPECIAL_KEYS,
    FLAG_RANDR,
    FLAG_AIGLX,
    FLAG_IGNORE_ABI,
    FLAG_ALLOW_EMPTY_INPUT,
    FLAG_USE_DEFAULT_FONT_PATH,
    FLAG_AUTO_ADD_DEVICES,
    FLAG_AUTO_ENABLE_DEVICES,
    FLAG_GLX_VISUALS,
    FLAG_DRI2,
} FlagValues;
   
static OptionInfoRec FlagOptions[] = {
  { FLAG_NOTRAPSIGNALS,		"NoTrapSignals",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_DONTVTSWITCH,		"DontVTSwitch",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_DONTZAP,		"DontZap",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_DONTZOOM,		"DontZoom",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_DISABLEVIDMODE,	"DisableVidModeExtension",	OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_ALLOWNONLOCAL,		"AllowNonLocalXvidtune",	OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_DISABLEMODINDEV,	"DisableModInDev",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_MODINDEVALLOWNONLOCAL,	"AllowNonLocalModInDev",	OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_ALLOWMOUSEOPENFAIL,	"AllowMouseOpenFail",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_VTSYSREQ,		"VTSysReq",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_XKBDISABLE,		"XkbDisable",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_PCIPROBE1,		"PciProbe1"		,	OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_PCIPROBE2,		"PciProbe2",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_PCIFORCECONFIG1,	"PciForceConfig1",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_PCIFORCECONFIG2,	"PciForceConfig2",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_PCIFORCENONE,		"PciForceNone",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_PCIOSCONFIG,	        "PciOsConfig",   		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_SAVER_BLANKTIME,	"BlankTime"		,	OPTV_INTEGER,
	{0}, FALSE },
  { FLAG_DPMS_STANDBYTIME,	"StandbyTime",			OPTV_INTEGER,
	{0}, FALSE },
  { FLAG_DPMS_SUSPENDTIME,	"SuspendTime",			OPTV_INTEGER,
	{0}, FALSE },
  { FLAG_DPMS_OFFTIME,		"OffTime",			OPTV_INTEGER,
	{0}, FALSE },
  { FLAG_PIXMAP,		"Pixmap",			OPTV_INTEGER,
	{0}, FALSE },
  { FLAG_PC98,			"PC98",				OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_ESTIMATE_SIZES_AGGRESSIVELY,"EstimateSizesAggressively",OPTV_INTEGER,
	{0}, FALSE },
  { FLAG_NOPM,			"NoPM",				OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_XINERAMA,		"Xinerama",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_ALLOW_DEACTIVATE_GRABS,"AllowDeactivateGrabs",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_ALLOW_CLOSEDOWN_GRABS, "AllowClosedownGrabs",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_LOG,			"Log",				OPTV_STRING,
	{0}, FALSE },
  { FLAG_RENDER_COLORMAP_MODE,	"RenderColormapMode",		OPTV_STRING,
        {0}, FALSE },
  { FLAG_HANDLE_SPECIAL_KEYS,	"HandleSpecialKeys",		OPTV_STRING,
        {0}, FALSE },
  { FLAG_RANDR,			"RandR",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_AIGLX,			"AIGLX",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_ALLOW_EMPTY_INPUT,     "AllowEmptyInput",              OPTV_BOOLEAN,
        {0}, FALSE },
  { FLAG_IGNORE_ABI,		"IgnoreABI",			OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_USE_DEFAULT_FONT_PATH,  "UseDefaultFontPath",		OPTV_BOOLEAN,
	{0}, FALSE },
  { FLAG_AUTO_ADD_DEVICES,       "AutoAddDevices",		OPTV_BOOLEAN,
        {0}, TRUE },
  { FLAG_AUTO_ENABLE_DEVICES,    "AutoEnableDevices",		OPTV_BOOLEAN,
        {0}, TRUE },
  { FLAG_GLX_VISUALS,		"GlxVisuals",			OPTV_STRING,
        {0}, FALSE },
  { FLAG_DRI2,			"DRI2",				OPTV_BOOLEAN,
	{0}, FALSE },
  { -1,				NULL,				OPTV_NONE,
	{0}, FALSE },
};

#ifdef __i386__
static Bool
detectPC98(void)
{
#ifdef SUPPORT_PC98
    unsigned char buf[2];

    if (xf86ReadBIOS(0xf8000, 0xe80, buf, 2) != 2)
	return FALSE;
    if ((buf[0] == 0x98) && (buf[1] == 0x21))
	return TRUE;
    else
	return FALSE;
#else
    return FALSE;
#endif
}
#endif /* __i386__ */

static Bool
configServerFlags(XF86ConfFlagsPtr flagsconf, XF86OptionPtr layoutopts)
{
    XF86OptionPtr optp, tmp;
    int i;
    Pix24Flags pix24 = Pix24DontCare;
    Bool value;
    MessageType from;
    const char *s;

    /*
     * Merge the ServerLayout and ServerFlags options.  The former have
     * precedence over the latter.
     */
    optp = NULL;
    if (flagsconf && flagsconf->flg_option_lst)
	optp = xf86optionListDup(flagsconf->flg_option_lst);
    if (layoutopts) {
	tmp = xf86optionListDup(layoutopts);
	if (optp)
	    optp = xf86optionListMerge(optp, tmp);
	else
	    optp = tmp;
    }

    xf86ProcessOptions(-1, optp, FlagOptions);

    xf86GetOptValBool(FlagOptions, FLAG_NOTRAPSIGNALS, &xf86Info.notrapSignals);
    xf86GetOptValBool(FlagOptions, FLAG_DONTVTSWITCH, &xf86Info.dontVTSwitch);
    xf86GetOptValBool(FlagOptions, FLAG_DONTZAP, &xf86Info.dontZap);
    xf86GetOptValBool(FlagOptions, FLAG_DONTZOOM, &xf86Info.dontZoom);

    xf86GetOptValBool(FlagOptions, FLAG_ALLOW_DEACTIVATE_GRABS,
		      &(xf86Info.grabInfo.allowDeactivate));
    xf86GetOptValBool(FlagOptions, FLAG_ALLOW_CLOSEDOWN_GRABS,
		      &(xf86Info.grabInfo.allowClosedown));
    xf86GetOptValBool(FlagOptions, FLAG_IGNORE_ABI, &xf86Info.ignoreABI);
    if (xf86Info.ignoreABI) {
	    xf86Msg(X_CONFIG, "Ignoring ABI Version\n");
    }

    if (xf86IsOptionSet(FlagOptions, FLAG_AUTO_ADD_DEVICES)) {
        xf86GetOptValBool(FlagOptions, FLAG_AUTO_ADD_DEVICES,
                          &xf86Info.autoAddDevices);
        from = X_CONFIG;
    }
    else {
        xf86Info.autoAddDevices = TRUE;
        from = X_DEFAULT;
    }
    xf86Msg(from, "%sutomatically adding devices\n",
            xf86Info.autoAddDevices ? "A" : "Not a");

    if (xf86IsOptionSet(FlagOptions, FLAG_AUTO_ENABLE_DEVICES)) {
        xf86GetOptValBool(FlagOptions, FLAG_AUTO_ENABLE_DEVICES,
                          &xf86Info.autoEnableDevices);
        from = X_CONFIG;
    }
    else {
        xf86Info.autoEnableDevices = TRUE;
        from = X_DEFAULT;
    }
    xf86Msg(from, "%sutomatically enabling devices\n",
            xf86Info.autoEnableDevices ? "A" : "Not a");

    /*
     * Set things up based on the config file information.  Some of these
     * settings may be overridden later when the command line options are
     * checked.
     */
#ifdef XF86VIDMODE
    if (xf86GetOptValBool(FlagOptions, FLAG_DISABLEVIDMODE, &value))
	xf86Info.vidModeEnabled = !value;
    if (xf86GetOptValBool(FlagOptions, FLAG_ALLOWNONLOCAL, &value))
	xf86Info.vidModeAllowNonLocal = value;
#endif

#ifdef XF86MISC
    if (xf86GetOptValBool(FlagOptions, FLAG_DISABLEMODINDEV, &value))
	xf86Info.miscModInDevEnabled = !value;
    if (xf86GetOptValBool(FlagOptions, FLAG_MODINDEVALLOWNONLOCAL, &value))
	xf86Info.miscModInDevAllowNonLocal = value;
#endif

    if (xf86GetOptValBool(FlagOptions, FLAG_ALLOWMOUSEOPENFAIL, &value))
	xf86Info.allowMouseOpenFail = value;

    if (xf86GetOptValBool(FlagOptions, FLAG_VTSYSREQ, &value)) {
#ifdef USE_VT_SYSREQ
	xf86Info.vtSysreq = value;
	xf86Msg(X_CONFIG, "VTSysReq %s\n", value ? "enabled" : "disabled");
#else
	if (value)
	    xf86Msg(X_WARNING, "VTSysReq is not supported on this OS\n");
#endif
    }

    if (xf86GetOptValBool(FlagOptions, FLAG_XKBDISABLE, &value)) {
#ifdef XKB
	noXkbExtension = value;
	xf86Msg(X_CONFIG, "Xkb %s\n", value ? "disabled" : "enabled");
#else
	if (!value)
	    xf86Msg(X_WARNING, "Xserver doesn't support XKB\n");
#endif
    }

    if (xf86IsOptionSet(FlagOptions, FLAG_PCIPROBE1))
	xf86Info.pciFlags = PCIProbe1;
    if (xf86IsOptionSet(FlagOptions, FLAG_PCIPROBE2))
	xf86Info.pciFlags = PCIProbe2;
    if (xf86IsOptionSet(FlagOptions, FLAG_PCIFORCECONFIG1))
	xf86Info.pciFlags = PCIForceConfig1;
    if (xf86IsOptionSet(FlagOptions, FLAG_PCIFORCECONFIG2))
	xf86Info.pciFlags = PCIForceConfig2;
    if (xf86IsOptionSet(FlagOptions, FLAG_PCIOSCONFIG))
	xf86Info.pciFlags = PCIOsConfig;
    if (xf86IsOptionSet(FlagOptions, FLAG_PCIFORCENONE))
	xf86Info.pciFlags = PCIForceNone;

    xf86Info.pmFlag = TRUE;
    if (xf86GetOptValBool(FlagOptions, FLAG_NOPM, &value)) 
	xf86Info.pmFlag = !value;
    {
	if ((s = xf86GetOptValString(FlagOptions, FLAG_LOG))) {
	    if (!xf86NameCmp(s,"flush")) {
		xf86Msg(X_CONFIG, "Flushing logfile enabled\n");
		xf86Info.log = LogFlush;
		LogSetParameter(XLOG_FLUSH, TRUE);
	    } else if (!xf86NameCmp(s,"sync")) {
		xf86Msg(X_CONFIG, "Syncing logfile enabled\n");
		xf86Info.log = LogSync;
		LogSetParameter(XLOG_FLUSH, TRUE);
		LogSetParameter(XLOG_SYNC, TRUE);
	    } else {
		xf86Msg(X_WARNING,"Unknown Log option\n");
	    }
        }
    }
    
#ifdef RENDER
    {
	if ((s = xf86GetOptValString(FlagOptions, FLAG_RENDER_COLORMAP_MODE))){
	    int policy = PictureParseCmapPolicy (s);
	    if (policy == PictureCmapPolicyInvalid)
		xf86Msg(X_WARNING, "Unknown colormap policy \"%s\"\n", s);
	    else
	    {
		xf86Msg(X_CONFIG, "Render colormap policy set to %s\n", s);
		PictureCmapPolicy = policy;
	    }
	}
    }
#endif
    {
	if ((s = xf86GetOptValString(FlagOptions, FLAG_HANDLE_SPECIAL_KEYS))) {
	    if (!xf86NameCmp(s,"always")) {
		xf86Msg(X_CONFIG, "Always handling special keys in DDX\n");
		xf86Info.ddxSpecialKeys = SKAlways;
	    } else if (!xf86NameCmp(s,"whenneeded")) {
		xf86Msg(X_CONFIG, "Special keys handled in DDX only if needed\n");
		xf86Info.ddxSpecialKeys = SKWhenNeeded;
	    } else if (!xf86NameCmp(s,"never")) {
		xf86Msg(X_CONFIG, "Never handling special keys in DDX\n");
		xf86Info.ddxSpecialKeys = SKNever;
	    } else {
		xf86Msg(X_WARNING,"Unknown HandleSpecialKeys option\n");
	    }
        }
    }
#ifdef RANDR
    xf86Info.disableRandR = FALSE;
    xf86Info.randRFrom = X_DEFAULT;
    if (xf86GetOptValBool(FlagOptions, FLAG_RANDR, &value)) {
	xf86Info.disableRandR = !value;
	xf86Info.randRFrom = X_CONFIG;
    }
#endif
    i = -1;
    xf86GetOptValInteger(FlagOptions, FLAG_ESTIMATE_SIZES_AGGRESSIVELY, &i);
    if (i >= 0)
	xf86Info.estimateSizesAggressively = i;
    else
	xf86Info.estimateSizesAggressively = 0;

    xf86Info.aiglx = TRUE;
    xf86Info.aiglxFrom = X_DEFAULT;
    if (xf86GetOptValBool(FlagOptions, FLAG_AIGLX, &value)) {
	xf86Info.aiglx = value;
	xf86Info.aiglxFrom = X_CONFIG;
    }

#ifdef GLXEXT
    xf86Info.glxVisuals = XF86_GlxVisualsTypical;
    xf86Info.glxVisualsFrom = X_DEFAULT;
    if ((s = xf86GetOptValString(FlagOptions, FLAG_GLX_VISUALS))) {
	if (!xf86NameCmp(s, "minimal")) {
	    xf86Info.glxVisuals = XF86_GlxVisualsMinimal;
	} else if (!xf86NameCmp(s, "typical")) {
	    xf86Info.glxVisuals = XF86_GlxVisualsTypical;
	} else if (!xf86NameCmp(s, "all")) {
	    xf86Info.glxVisuals = XF86_GlxVisualsAll;
	} else {
	    xf86Msg(X_WARNING,"Unknown HandleSpecialKeys option\n");
	}
    }

    if (xf86GetOptValBool(FlagOptions, FLAG_AIGLX, &value)) {
	xf86Info.aiglx = value;
	xf86Info.aiglxFrom = X_CONFIG;
    }
#endif

    /* AllowEmptyInput is automatically true if we're hotplugging */
    xf86Info.allowEmptyInput = (xf86Info.autoAddDevices && xf86Info.autoEnableDevices);
    xf86GetOptValBool(FlagOptions, FLAG_ALLOW_EMPTY_INPUT, &xf86Info.allowEmptyInput);

    xf86Info.useDefaultFontPath = TRUE;
    xf86Info.useDefaultFontPathFrom = X_DEFAULT;
    if (xf86GetOptValBool(FlagOptions, FLAG_USE_DEFAULT_FONT_PATH, &value)) {
	xf86Info.useDefaultFontPath = value;
	xf86Info.useDefaultFontPathFrom = X_CONFIG;
    }

/* Make sure that timers don't overflow CARD32's after multiplying */
#define MAX_TIME_IN_MIN (0x7fffffff / MILLI_PER_MIN)

    i = -1;
    xf86GetOptValInteger(FlagOptions, FLAG_SAVER_BLANKTIME, &i);
    if ((i >= 0) && (i < MAX_TIME_IN_MIN))
	ScreenSaverTime = defaultScreenSaverTime = i * MILLI_PER_MIN;
    else if (i != -1)
	xf86ConfigError("BlankTime value %d outside legal range of 0 - %d minutes",
			i, MAX_TIME_IN_MIN);

#ifdef DPMSExtension
    i = -1;
    xf86GetOptValInteger(FlagOptions, FLAG_DPMS_STANDBYTIME, &i);
    if ((i >= 0) && (i < MAX_TIME_IN_MIN))
	DPMSStandbyTime = defaultDPMSStandbyTime = i * MILLI_PER_MIN;
    else if (i != -1)
	xf86ConfigError("StandbyTime value %d outside legal range of 0 - %d minutes",
			i, MAX_TIME_IN_MIN);
    i = -1;
    xf86GetOptValInteger(FlagOptions, FLAG_DPMS_SUSPENDTIME, &i);
    if ((i >= 0) && (i < MAX_TIME_IN_MIN))
	DPMSSuspendTime = defaultDPMSSuspendTime = i * MILLI_PER_MIN;
    else if (i != -1)
	xf86ConfigError("SuspendTime value %d outside legal range of 0 - %d minutes",
			i, MAX_TIME_IN_MIN);
    i = -1;
    xf86GetOptValInteger(FlagOptions, FLAG_DPMS_OFFTIME, &i);
    if ((i >= 0) && (i < MAX_TIME_IN_MIN))
	DPMSOffTime = defaultDPMSOffTime = i * MILLI_PER_MIN;
    else if (i != -1)
	xf86ConfigError("OffTime value %d outside legal range of 0 - %d minutes",
			i, MAX_TIME_IN_MIN);
#endif

    i = -1;
    xf86GetOptValInteger(FlagOptions, FLAG_PIXMAP, &i);
    switch (i) {
    case 24:
	pix24 = Pix24Use24;
	break;
    case 32:
	pix24 = Pix24Use32;
	break;
    case -1:
	break;
    default:
	xf86ConfigError("Pixmap option's value (%d) must be 24 or 32\n", i);
	return FALSE;
    }
    if (xf86Pix24 != Pix24DontCare) {
	xf86Info.pixmap24 = xf86Pix24;
	xf86Info.pix24From = X_CMDLINE;
    } else if (pix24 != Pix24DontCare) {
	xf86Info.pixmap24 = pix24;
	xf86Info.pix24From = X_CONFIG;
    } else {
	xf86Info.pixmap24 = Pix24DontCare;
	xf86Info.pix24From = X_DEFAULT;
    }
#ifdef __i386__
    if (xf86GetOptValBool(FlagOptions, FLAG_PC98, &value)) {
	xf86Info.pc98 = value;
	if (value) {
	    xf86Msg(X_CONFIG, "Japanese PC98 architecture\n");
	}
    } else
	if (detectPC98()) {
	    xf86Info.pc98 = TRUE;
	    xf86Msg(X_PROBED, "Japanese PC98 architecture\n");
	}
#endif

#ifdef PANORAMIX
    from = X_DEFAULT;
    if (!noPanoramiXExtension)
      from = X_CMDLINE;
    else if (xf86GetOptValBool(FlagOptions, FLAG_XINERAMA, &value)) {
      noPanoramiXExtension = !value;
      from = X_CONFIG;
    }
    if (!noPanoramiXExtension)
      xf86Msg(from, "Xinerama: enabled\n");
#endif

#ifdef DRI2
    xf86Info.dri2 = FALSE;
    xf86Info.dri2From = X_DEFAULT;
    if (xf86GetOptValBool(FlagOptions, FLAG_DRI2, &value)) {
	xf86Info.dri2 = value;
	xf86Info.dri2From = X_CONFIG;
    }
#endif

    return TRUE;
}

Bool xf86DRI2Enabled(void)
{
    return xf86Info.dri2;
}

/*
 * Locate the core input devices.  These can be specified/located in
 * the following ways, in order of priority:
 *
 *  1. The InputDevices named by the -pointer and -keyboard command line
 *     options.
 *  2. The "CorePointer" and "CoreKeyboard" InputDevices referred to by
 *     the active ServerLayout.
 *  3. The first InputDevices marked as "CorePointer" and "CoreKeyboard".
 *  4. The first InputDevices that use the 'mouse' and 'keyboard' or 'kbd'
 *     drivers.
 *  5. Default devices with an empty (default) configuration.  These defaults
 *     will reference the 'mouse' and 'keyboard' drivers.
 */

static Bool
checkCoreInputDevices(serverLayoutPtr servlayoutp, Bool implicitLayout)
{
    IDevPtr corePointer = NULL, coreKeyboard = NULL;
    Bool foundPointer = FALSE, foundKeyboard = FALSE;
    const char *pointerMsg = NULL, *keyboardMsg = NULL;
    IDevPtr *devs, /* iterator */
            indp;
    IDevRec Pointer, Keyboard;
    XF86ConfInputPtr confInput;
    XF86ConfInputRec defPtr, defKbd;
    int count = 0;
    MessageType from = X_DEFAULT;
    int found = 0;

    /*
     * First check if a core pointer or core keyboard have been specified
     * in the active ServerLayout.  If more than one is specified for either,
     * remove the core attribute from the later ones.
     */
    for (devs = servlayoutp->inputs; devs && *devs; devs++) {
        indp = *devs;
	pointer opt1 = NULL, opt2 = NULL;
	if (indp->commonOptions &&
	    xf86CheckBoolOption(indp->commonOptions, "CorePointer", FALSE)) {
	    opt1 = indp->commonOptions;
	}
	if (indp->extraOptions &&
	    xf86CheckBoolOption(indp->extraOptions, "CorePointer", FALSE)) {
	    opt2 = indp->extraOptions;
	}
	if (opt1 || opt2) {
	    if (!corePointer) {
		corePointer = indp;
	    } else {
		if (opt1)
		    xf86ReplaceBoolOption(opt1, "CorePointer", FALSE);
		if (opt2)
		    xf86ReplaceBoolOption(opt2, "CorePointer", FALSE);
		xf86Msg(X_WARNING, "Duplicate core pointer devices.  "
			"Removing core pointer attribute from \"%s\"\n",
			indp->identifier);
	    }
	}
	opt1 = opt2 = NULL;
	if (indp->commonOptions &&
	    xf86CheckBoolOption(indp->commonOptions, "CoreKeyboard", FALSE)) {
	    opt1 = indp->commonOptions;
	}
	if (indp->extraOptions &&
	    xf86CheckBoolOption(indp->extraOptions, "CoreKeyboard", FALSE)) {
	    opt2 = indp->extraOptions;
	}
	if (opt1 || opt2) {
	    if (!coreKeyboard) {
		coreKeyboard = indp;
	    } else {
		if (opt1)
		    xf86ReplaceBoolOption(opt1, "CoreKeyboard", FALSE);
		if (opt2)
		    xf86ReplaceBoolOption(opt2, "CoreKeyboard", FALSE);
		xf86Msg(X_WARNING, "Duplicate core keyboard devices.  "
			"Removing core keyboard attribute from \"%s\"\n",
			indp->identifier);
	    }
	}
	count++;
    }

    confInput = NULL;

    /* 1. Check for the -pointer command line option. */
    if (xf86PointerName) {
	confInput = xf86findInput(xf86PointerName,
				  xf86configptr->conf_input_lst);
	if (!confInput) {
	    xf86Msg(X_ERROR, "No InputDevice section called \"%s\"\n",
		    xf86PointerName);
	    return FALSE;
	}
	from = X_CMDLINE;
	/*
	 * If one was already specified in the ServerLayout, it needs to be
	 * removed.
	 */
	if (corePointer) {
	    for (devs = servlayoutp->inputs; devs && *devs; devs++)
		if (*devs == corePointer)
                {
                    xfree(*devs);
                    *devs = (IDevPtr)0x1; /* ensure we dont skip next loop*/
		    break;
                }
	    for (; devs && *devs; devs++)
		devs[0] = devs[1];
	    count--;
	}
	corePointer = NULL;
	foundPointer = TRUE;
    }

    /* 2. ServerLayout-specified core pointer. */
    if (corePointer) {
	foundPointer = TRUE;
	from = X_CONFIG;
    }

    /* 3. First core pointer device. */
    if (!foundPointer && (!xf86Info.allowEmptyInput || implicitLayout)) {
	XF86ConfInputPtr p;

	for (p = xf86configptr->conf_input_lst; p; p = p->list.next) {
	    if (p->inp_option_lst &&
		xf86CheckBoolOption(p->inp_option_lst, "CorePointer", FALSE)) {
		confInput = p;
		foundPointer = TRUE;
		from = X_DEFAULT;
		pointerMsg = "first core pointer device";
		break;
	    }
	}
    }

    /* 4. First pointer with 'mouse' as the driver. */
    if (!foundPointer && (!xf86Info.allowEmptyInput || implicitLayout)) {
	confInput = xf86findInput(CONF_IMPLICIT_POINTER,
				  xf86configptr->conf_input_lst);
	if (!confInput) {
	    confInput = xf86findInputByDriver("mouse",
					      xf86configptr->conf_input_lst);
	}
	if (confInput) {
	    foundPointer = TRUE;
	    from = X_DEFAULT;
	    pointerMsg = "first mouse device";
	}
    }

    /* 5. Built-in default. */
    if (!foundPointer && !xf86Info.allowEmptyInput) {
	bzero(&defPtr, sizeof(defPtr));
	defPtr.inp_identifier = strdup("<default pointer>");
	defPtr.inp_driver = strdup("mouse");
	confInput = &defPtr;
	foundPointer = TRUE;
	from = X_DEFAULT;
	pointerMsg = "default mouse configuration";
    }

    /* Add the core pointer device to the layout, and set it to Core. */
    if (foundPointer && confInput) {
	foundPointer = configInput(&Pointer, confInput, from);
        if (foundPointer) {
	    count++;
	    devs = xnfrealloc(servlayoutp->inputs,
			      (count + 1) * sizeof(IDevPtr));
            devs[count - 1] = xnfalloc(sizeof(IDevRec));
	    *devs[count - 1] = Pointer;
	    devs[count - 1]->extraOptions =
				xf86addNewOption(NULL, xnfstrdup("CorePointer"), NULL);
	    devs[count] = NULL;
	    servlayoutp->inputs = devs;
	}
    }

    if (!foundPointer) {
	if (!xf86Info.allowEmptyInput) {
	    /* This shouldn't happen. */
	    xf86Msg(X_ERROR, "Cannot locate a core pointer device.\n");
	    return FALSE;
	} else {
	    xf86Msg(X_INFO, "Cannot locate a core pointer device.\n");
	}
    }

    /*
     * always synthesize a 'mouse' section configured to send core
     * events, unless a 'void' section is found, in which case the user
     * probably wants to run footless.
     *
     * If you're using an evdev keyboard and expect a default mouse
     * section ... deal.
     */
    for (devs = servlayoutp->inputs; devs && *devs; devs++) {
	if (!strcmp((*devs)->driver, "void") || !strcmp((*devs)->driver, "mouse") ||
            !strcmp((*devs)->driver, "vmmouse") || !strcmp((*devs)->driver, "evdev")) {
	    found = 1; break;
	}
    }
    if (!found && !xf86Info.allowEmptyInput) {
	xf86Msg(X_INFO, "No default mouse found, adding one\n");
	bzero(&defPtr, sizeof(defPtr));
	defPtr.inp_identifier = strdup("<default pointer>");
	defPtr.inp_driver = strdup("mouse");
	confInput = &defPtr;
	foundPointer = configInput(&Pointer, confInput, from);
        if (foundPointer) {
	    count++;
	    devs = xnfrealloc(servlayoutp->inputs,
			      (count + 1) * sizeof(IDevPtr));
            devs[count - 1] = xnfalloc(sizeof(IDevRec));
	    *devs[count - 1] = Pointer;
	    devs[count - 1]->extraOptions =
				xf86addNewOption(NULL, xnfstrdup("AlwaysCore"), NULL);
	    devs[count] = NULL;
	    servlayoutp->inputs = devs;
	}
    }

    confInput = NULL;

    /* 1. Check for the -keyboard command line option. */
    if (xf86KeyboardName) {
	confInput = xf86findInput(xf86KeyboardName,
				  xf86configptr->conf_input_lst);
	if (!confInput) {
	    xf86Msg(X_ERROR, "No InputDevice section called \"%s\"\n",
		    xf86KeyboardName);
	    return FALSE;
	}
	from = X_CMDLINE;
	/*
	 * If one was already specified in the ServerLayout, it needs to be
	 * removed.
	 */
	if (coreKeyboard) {
	    for (devs = servlayoutp->inputs; devs && *devs; devs++)
		if (*devs == coreKeyboard)
                {
                    xfree(*devs);
                    *devs = (IDevPtr)0x1; /* ensure we dont skip next loop */
		    break;
                }
	    for (; devs && *devs; devs++)
		devs[0] = devs[1];
	    count--;
	}
	coreKeyboard = NULL;
	foundKeyboard = TRUE;
    }

    /* 2. ServerLayout-specified core keyboard. */
    if (coreKeyboard) {
	foundKeyboard = TRUE;
	from = X_CONFIG;
    }

    /* 3. First core keyboard device. */
    if (!foundKeyboard && (!xf86Info.allowEmptyInput || implicitLayout)) {
	XF86ConfInputPtr p;

	for (p = xf86configptr->conf_input_lst; p; p = p->list.next) {
	    if (p->inp_option_lst &&
		xf86CheckBoolOption(p->inp_option_lst, "CoreKeyboard", FALSE)) {
		confInput = p;
		foundKeyboard = TRUE;
		from = X_DEFAULT;
		keyboardMsg = "first core keyboard device";
		break;
	    }
	}
    }

    /* 4. First keyboard with 'keyboard' or 'kbd' as the driver. */
    if (!foundKeyboard && (!xf86Info.allowEmptyInput || implicitLayout)) {
	confInput = xf86findInput(CONF_IMPLICIT_KEYBOARD,
				  xf86configptr->conf_input_lst);
	if (!confInput) {
	    confInput = xf86findInputByDriver("kbd",
					      xf86configptr->conf_input_lst);
	}
	if (confInput) {
	    foundKeyboard = TRUE;
	    from = X_DEFAULT;
	    keyboardMsg = "first keyboard device";
	}
    }

    /* 5. Built-in default. */
    if (!foundKeyboard && !xf86Info.allowEmptyInput) {
	bzero(&defKbd, sizeof(defKbd));
	defKbd.inp_identifier = strdup("<default keyboard>");
	defKbd.inp_driver = strdup("kbd");
	confInput = &defKbd;
	foundKeyboard = TRUE;
	keyboardMsg = "default keyboard configuration";
	from = X_DEFAULT;
    }

    /* Add the core keyboard device to the layout, and set it to Core. */
    if (foundKeyboard && confInput) {
	foundKeyboard = configInput(&Keyboard, confInput, from);
        if (foundKeyboard) {
	    count++;
	    devs = xnfrealloc(servlayoutp->inputs,
			      (count + 1) * sizeof(IDevPtr));
            devs[count - 1] = xnfalloc(sizeof(IDevRec));
	    *devs[count - 1] = Keyboard;
	    devs[count - 1]->extraOptions =
				xf86addNewOption(NULL, xnfstrdup("CoreKeyboard"), NULL);
	    devs[count] = NULL;
	    servlayoutp->inputs = devs;
	}
    }

    if (!foundKeyboard) {
	if (!xf86Info.allowEmptyInput) {
		/* This shouldn't happen. */
		xf86Msg(X_ERROR, "Cannot locate a core keyboard device.\n");
		return FALSE;
	} else {
		xf86Msg(X_INFO, "Cannot locate a core keyboard device.\n");
	}
    }

    if (pointerMsg) {
	if (implicitLayout)
	    xf86Msg(X_DEFAULT, "No Layout section. Using the %s.\n",
	            pointerMsg);
	else
	    xf86Msg(X_DEFAULT, "The core pointer device wasn't specified "
	            "explicitly in the layout.\n"
	            "\tUsing the %s.\n", pointerMsg);
    }

    if (keyboardMsg) {
	if (implicitLayout)
	    xf86Msg(X_DEFAULT, "No Layout section. Using the %s.\n",
	            keyboardMsg);
	else
	    xf86Msg(X_DEFAULT, "The core keyboard device wasn't specified "
	            "explicitly in the layout.\n"
	            "\tUsing the %s.\n", keyboardMsg);
    }

    if (xf86Info.allowEmptyInput && !(foundPointer && foundKeyboard)) {
	xf86Msg(X_INFO, "The server relies on HAL to provide the list of "
	                "input devices.\n\tIf no devices become available, "
	                "reconfigure HAL or disable AllowEmptyInput.\n");
    }

    return TRUE;
}

typedef enum {
    LAYOUT_ISOLATEDEVICE,
    LAYOUT_SINGLECARD
} LayoutValues;

static OptionInfoRec LayoutOptions[] = {
  { LAYOUT_ISOLATEDEVICE,      "IsolateDevice",        OPTV_STRING,
       {0}, FALSE },
  { LAYOUT_SINGLECARD,         "SingleCard",           OPTV_BOOLEAN,
       {0}, FALSE },
  { -1,                                NULL,                   OPTV_NONE,
       {0}, FALSE },
};

/*
 * figure out which layout is active, which screens are used in that layout,
 * which drivers and monitors are used in these screens
 */
static Bool
configLayout(serverLayoutPtr servlayoutp, XF86ConfLayoutPtr conf_layout,
	     char *default_layout)
{
    XF86ConfAdjacencyPtr adjp;
    XF86ConfInactivePtr idp;
    XF86ConfInputrefPtr irp;
    int count = 0;
    int scrnum;
    XF86ConfLayoutPtr l;
    MessageType from;
    screenLayoutPtr slp;
    GDevPtr gdp;
    IDevPtr* indp;
    int i = 0, j;

    if (!servlayoutp)
	return FALSE;

    /*
     * which layout section is the active one?
     *
     * If there is a -layout command line option, use that one, otherwise
     * pick the first one.
     */
    from = X_DEFAULT;
    if (xf86LayoutName != NULL)
	from = X_CMDLINE;
    else if (default_layout) {
	xf86LayoutName = default_layout;
	from = X_CONFIG;
    }
    if (xf86LayoutName != NULL) {
	if ((l = xf86findLayout(xf86LayoutName, conf_layout)) == NULL) {
	    xf86Msg(X_ERROR, "No ServerLayout section called \"%s\"\n",
		    xf86LayoutName);
	    return FALSE;
	}
	conf_layout = l;
    }
    xf86Msg(from, "ServerLayout \"%s\"\n", conf_layout->lay_identifier);
    adjp = conf_layout->lay_adjacency_lst;

    /*
     * we know that each screen is referenced exactly once on the left side
     * of a layout statement in the Layout section. So to allocate the right
     * size for the array we do a quick walk of the list to figure out how
     * many sections we have
     */
    while (adjp) {
        count++;
        adjp = (XF86ConfAdjacencyPtr)adjp->list.next;
    }
#ifdef DEBUG
    ErrorF("Found %d screens in the layout section %s",
           count, conf_layout->lay_identifier);
#endif
    slp = xnfcalloc(1, (count + 1) * sizeof(screenLayoutRec));
    slp[count].screen = NULL;
    /*
     * now that we have storage, loop over the list again and fill in our
     * data structure; at this point we do not fill in the adjacency
     * information as it is not clear if we need it at all
     */
    adjp = conf_layout->lay_adjacency_lst;
    count = 0;
    while (adjp) {
        slp[count].screen = xnfcalloc(1, sizeof(confScreenRec));
	if (adjp->adj_scrnum < 0)
	    scrnum = count;
	else
	    scrnum = adjp->adj_scrnum;
	if (!configScreen(slp[count].screen, adjp->adj_screen, scrnum,
			  X_CONFIG)) {
	    xfree(slp);
	    return FALSE;
	}
	slp[count].x = adjp->adj_x;
	slp[count].y = adjp->adj_y;
	slp[count].refname = adjp->adj_refscreen;
	switch (adjp->adj_where) {
	case CONF_ADJ_OBSOLETE:
	    slp[count].where = PosObsolete;
	    slp[count].topname = adjp->adj_top_str;
	    slp[count].bottomname = adjp->adj_bottom_str;
	    slp[count].leftname = adjp->adj_left_str;
	    slp[count].rightname = adjp->adj_right_str;
	    break;
	case CONF_ADJ_ABSOLUTE:
	    slp[count].where = PosAbsolute;
	    break;
	case CONF_ADJ_RIGHTOF:
	    slp[count].where = PosRightOf;
	    break;
	case CONF_ADJ_LEFTOF:
	    slp[count].where = PosLeftOf;
	    break;
	case CONF_ADJ_ABOVE:
	    slp[count].where = PosAbove;
	    break;
	case CONF_ADJ_BELOW:
	    slp[count].where = PosBelow;
	    break;
	case CONF_ADJ_RELATIVE:
	    slp[count].where = PosRelative;
	    break;
	}
        count++;
        adjp = (XF86ConfAdjacencyPtr)adjp->list.next;
    }

    /* XXX Need to tie down the upper left screen. */

    /* Fill in the refscreen and top/bottom/left/right values */
    for (i = 0; i < count; i++) {
	for (j = 0; j < count; j++) {
	    if (slp[i].refname &&
		strcmp(slp[i].refname, slp[j].screen->id) == 0) {
		slp[i].refscreen = slp[j].screen;
	    }
	    if (slp[i].topname &&
		strcmp(slp[i].topname, slp[j].screen->id) == 0) {
		slp[i].top = slp[j].screen;
	    }
	    if (slp[i].bottomname &&
		strcmp(slp[i].bottomname, slp[j].screen->id) == 0) {
		slp[i].bottom = slp[j].screen;
	    }
	    if (slp[i].leftname &&
		strcmp(slp[i].leftname, slp[j].screen->id) == 0) {
		slp[i].left = slp[j].screen;
	    }
	    if (slp[i].rightname &&
		strcmp(slp[i].rightname, slp[j].screen->id) == 0) {
		slp[i].right = slp[j].screen;
	    }
	}
	if (slp[i].where != PosObsolete
	    && slp[i].where != PosAbsolute
	    && !slp[i].refscreen) {
	    xf86Msg(X_ERROR,"Screen %s doesn't exist: deleting placement\n",
		     slp[i].refname);
	    slp[i].where = PosAbsolute;
	    slp[i].x = 0;
	    slp[i].y = 0;
	}
    }

#ifdef LAYOUT_DEBUG
    ErrorF("Layout \"%s\"\n", conf_layout->lay_identifier);
    for (i = 0; i < count; i++) {
	ErrorF("Screen: \"%s\" (%d):\n", slp[i].screen->id,
	       slp[i].screen->screennum);
	switch (slp[i].where) {
	case PosObsolete:
	    ErrorF("\tObsolete format: \"%s\" \"%s\" \"%s\" \"%s\"\n",
		   slp[i].top, slp[i].bottom, slp[i].left, slp[i].right);
	    break;
	case PosAbsolute:
	    if (slp[i].x == -1)
		if (slp[i].screen->screennum == 0)
		    ErrorF("\tImplicitly left-most\n");
		else
		    ErrorF("\tImplicitly right of screen %d\n",
			   slp[i].screen->screennum - 1);
	    else
		ErrorF("\t%d %d\n", slp[i].x, slp[i].y);
	    break;
	case PosRightOf:
	    ErrorF("\tRight of \"%s\"\n", slp[i].refscreen->id);
	    break;
	case PosLeftOf:
	    ErrorF("\tLeft of \"%s\"\n", slp[i].refscreen->id);
	    break;
	case PosAbove:
	    ErrorF("\tAbove \"%s\"\n", slp[i].refscreen->id);
	    break;
	case PosBelow:
	    ErrorF("\tBelow \"%s\"\n", slp[i].refscreen->id);
	    break;
	case PosRelative:
	    ErrorF("\t%d %d relative to \"%s\"\n", slp[i].x, slp[i].y,
		   slp[i].refscreen->id);
	    break;
	}
    }
#endif
    /*
     * Count the number of inactive devices.
     */
    count = 0;
    idp = conf_layout->lay_inactive_lst;
    while (idp) {
        count++;
        idp = (XF86ConfInactivePtr)idp->list.next;
    }
#ifdef DEBUG
    ErrorF("Found %d inactive devices in the layout section %s",
           count, conf_layout->lay_identifier);
#endif
    gdp = xnfalloc((count + 1) * sizeof(GDevRec));
    gdp[count].identifier = NULL;
    idp = conf_layout->lay_inactive_lst;
    count = 0;
    while (idp) {
	if (!configDevice(&gdp[count], idp->inactive_device, FALSE)) {
	    xfree(gdp);
	    return FALSE;
	}
        count++;
        idp = (XF86ConfInactivePtr)idp->list.next;
    }
    /*
     * Count the number of input devices.
     */
    count = 0;
    irp = conf_layout->lay_input_lst;
    while (irp) {
        count++;
        irp = (XF86ConfInputrefPtr)irp->list.next;
    }
#ifdef DEBUG
    ErrorF("Found %d input devices in the layout section %s",
           count, conf_layout->lay_identifier);
#endif
    indp = xnfcalloc((count + 1), sizeof(IDevPtr));
    indp[count] = NULL;
    irp = conf_layout->lay_input_lst;
    count = 0;
    while (irp) {
        indp[count] = xnfalloc(sizeof(IDevRec));
	if (!configInput(indp[count], irp->iref_inputdev, X_CONFIG)) {
            while(count--) 
                xfree(indp[count]);
            xfree(indp);
            return FALSE;
	}
	indp[count]->extraOptions = irp->iref_option_lst;
        count++;
        irp = (XF86ConfInputrefPtr)irp->list.next;
    }
    servlayoutp->id = conf_layout->lay_identifier;
    servlayoutp->screens = slp;
    servlayoutp->inactives = gdp;
    servlayoutp->inputs = indp;
    servlayoutp->options = conf_layout->lay_option_lst;
    from = X_DEFAULT;

    return TRUE;
}

/*
 * No layout section, so find the first Screen section and set that up as
 * the only active screen.
 */
static Bool
configImpliedLayout(serverLayoutPtr servlayoutp, XF86ConfScreenPtr conf_screen)
{
    MessageType from;
    XF86ConfScreenPtr s;
    screenLayoutPtr slp;
    IDevPtr *indp;

    if (!servlayoutp)
	return FALSE;

    /*
     * which screen section is the active one?
     *
     * If there is a -screen option, use that one, otherwise use the first
     * one.
     */

    from = X_CONFIG;
    if (xf86ScreenName != NULL) {
	if ((s = xf86findScreen(xf86ScreenName, conf_screen)) == NULL) {
	    xf86Msg(X_ERROR, "No Screen section called \"%s\"\n",
		    xf86ScreenName);
	    return FALSE;
	}
	conf_screen = s;
	from = X_CMDLINE;
    }

    /* We have exactly one screen */

    slp = xnfcalloc(1, 2 * sizeof(screenLayoutRec));
    slp[0].screen = xnfcalloc(1, sizeof(confScreenRec));
    slp[1].screen = NULL;
    if (!configScreen(slp[0].screen, conf_screen, 0, from)) {
	xfree(slp);
	return FALSE;
    }
    servlayoutp->id = "(implicit)";
    servlayoutp->screens = slp;
    servlayoutp->inactives = xnfcalloc(1, sizeof(GDevRec));
    servlayoutp->options = NULL;
    /* Set up an empty input device list, then look for some core devices. */
    indp = xnfalloc(sizeof(IDevPtr));
    *indp = NULL;
    servlayoutp->inputs = indp;
    
    return TRUE;
}

static Bool
configXvAdaptor(confXvAdaptorPtr adaptor, XF86ConfVideoAdaptorPtr conf_adaptor)
{
    int count = 0;
    XF86ConfVideoPortPtr conf_port;

    xf86Msg(X_CONFIG, "|   |-->VideoAdaptor \"%s\"\n",
	    conf_adaptor->va_identifier);
    adaptor->identifier = conf_adaptor->va_identifier;
    adaptor->options = conf_adaptor->va_option_lst;
    if (conf_adaptor->va_busid || conf_adaptor->va_driver) {
	xf86Msg(X_CONFIG, "|   | Unsupported device type, skipping entry\n");
	return FALSE;
    }

    /*
     * figure out how many videoport subsections there are and fill them in
     */
    conf_port = conf_adaptor->va_port_lst;
    while(conf_port) {
        count++;
        conf_port = (XF86ConfVideoPortPtr)conf_port->list.next;
    }
    adaptor->ports = xnfalloc((count) * sizeof(confXvPortRec));
    adaptor->numports = count;
    count = 0;
    conf_port = conf_adaptor->va_port_lst;
    while(conf_port) {
	adaptor->ports[count].identifier = conf_port->vp_identifier;
	adaptor->ports[count].options = conf_port->vp_option_lst;
        count++;
        conf_port = (XF86ConfVideoPortPtr)conf_port->list.next;
    }

    return TRUE;
}

static Bool
configScreen(confScreenPtr screenp, XF86ConfScreenPtr conf_screen, int scrnum,
	     MessageType from)
{
    int count = 0;
    XF86ConfDisplayPtr dispptr;
    XF86ConfAdaptorLinkPtr conf_adaptor;
    Bool defaultMonitor = FALSE;

    if (!conf_screen) {
        conf_screen = xnfcalloc(1, sizeof(XF86ConfScreenRec));
        conf_screen->scrn_identifier = "Default Screen Section";
        xf86Msg(X_DEFAULT, "No screen section available. Using defaults.\n");
    }

    xf86Msg(from, "|-->Screen \"%s\" (%d)\n", conf_screen->scrn_identifier,
	    scrnum);
    /*
     * now we fill in the elements of the screen
     */
    screenp->id         = conf_screen->scrn_identifier;
    screenp->screennum  = scrnum;
    screenp->defaultdepth = conf_screen->scrn_defaultdepth;
    screenp->defaultbpp = conf_screen->scrn_defaultbpp;
    screenp->defaultfbbpp = conf_screen->scrn_defaultfbbpp;
    screenp->monitor    = xnfcalloc(1, sizeof(MonRec));
    /* If no monitor is specified, create a default one. */
    if (!conf_screen->scrn_monitor) {
	XF86ConfMonitorRec defMon;

	bzero(&defMon, sizeof(defMon));
	defMon.mon_identifier = "<default monitor>";
	/*
	 * TARGET_REFRESH_RATE may be defined to effectively limit the
	 * default resolution to the largest that has a "good" refresh
	 * rate.
	 */
#ifdef TARGET_REFRESH_RATE
	defMon.mon_option_lst = xf86ReplaceRealOption(defMon.mon_option_lst,
						      "TargetRefresh",
						      TARGET_REFRESH_RATE);
#endif
	if (!configMonitor(screenp->monitor, &defMon))
	    return FALSE;
	defaultMonitor = TRUE;
    } else {
	if (!configMonitor(screenp->monitor,conf_screen->scrn_monitor))
	    return FALSE;
    }
    /* Configure the device. If there isn't one configured, attach to the
     * first inactive one that we can configure. If there's none that work,
     * set it to NULL so that the section can be autoconfigured later */
    screenp->device     = xnfcalloc(1, sizeof(GDevRec));
    if ((!conf_screen->scrn_device) && (xf86configptr->conf_device_lst)) {
        conf_screen->scrn_device = xf86configptr->conf_device_lst;
	xf86Msg(X_DEFAULT, "No device specified for screen \"%s\".\n"
		"\tUsing the first device section listed.\n", screenp->id);
    }
    if (configDevice(screenp->device,conf_screen->scrn_device, TRUE)) {
        screenp->device->myScreenSection = screenp;
    } else {
        screenp->device = NULL;
    }
    screenp->options = conf_screen->scrn_option_lst;
    
    /*
     * figure out how many display subsections there are and fill them in
     */
    dispptr = conf_screen->scrn_display_lst;
    while(dispptr) {
        count++;
        dispptr = (XF86ConfDisplayPtr)dispptr->list.next;
    }
    screenp->displays   = xnfalloc((count) * sizeof(DispRec));
    screenp->numdisplays = count;
    
    /* Fill in the default Virtual size, if any */
    if (conf_screen->scrn_virtualX && conf_screen->scrn_virtualY) {
	for (count = 0, dispptr = conf_screen->scrn_display_lst;
	     dispptr;
	     dispptr = (XF86ConfDisplayPtr)dispptr->list.next, count++) {
	    screenp->displays[count].virtualX = conf_screen->scrn_virtualX;
	    screenp->displays[count].virtualY = conf_screen->scrn_virtualY;
	}
    }

    /* Now do the per-Display Virtual sizes */
    count = 0;
    dispptr = conf_screen->scrn_display_lst;
    while(dispptr) {
        configDisplay(&(screenp->displays[count]),dispptr);
        count++;
        dispptr = (XF86ConfDisplayPtr)dispptr->list.next;
    }

    /*
     * figure out how many videoadaptor references there are and fill them in
     */
    conf_adaptor = conf_screen->scrn_adaptor_lst;
    while(conf_adaptor) {
        count++;
        conf_adaptor = (XF86ConfAdaptorLinkPtr)conf_adaptor->list.next;
    }
    screenp->xvadaptors = xnfalloc((count) * sizeof(confXvAdaptorRec));
    screenp->numxvadaptors = 0;
    conf_adaptor = conf_screen->scrn_adaptor_lst;
    while(conf_adaptor) {
        if (configXvAdaptor(&(screenp->xvadaptors[screenp->numxvadaptors]),
			    conf_adaptor->al_adaptor))
    	    screenp->numxvadaptors++;
        conf_adaptor = (XF86ConfAdaptorLinkPtr)conf_adaptor->list.next;
    }

    if (defaultMonitor) {
	xf86Msg(X_DEFAULT, "No monitor specified for screen \"%s\".\n"
		"\tUsing a default monitor configuration.\n", screenp->id);
    }
    return TRUE;
}

typedef enum {
    MON_REDUCEDBLANKING,
    MON_MAX_PIX_CLOCK,
} MonitorValues;

static OptionInfoRec MonitorOptions[] = {
  { MON_REDUCEDBLANKING,      "ReducedBlanking",        OPTV_BOOLEAN,
       {0}, FALSE },
  { MON_MAX_PIX_CLOCK,	      "MaxPixClock",		OPTV_FREQ,
       {0}, FALSE },
  { -1,                                NULL,                   OPTV_NONE,
       {0}, FALSE },
};

static Bool
configMonitor(MonPtr monitorp, XF86ConfMonitorPtr conf_monitor)
{
    int count;
    DisplayModePtr mode,last = NULL;
    XF86ConfModeLinePtr cmodep;
    XF86ConfModesPtr modes;
    XF86ConfModesLinkPtr modeslnk = conf_monitor->mon_modes_sect_lst;
    Gamma zeros = {0.0, 0.0, 0.0};
    float badgamma = 0.0;
    
    xf86Msg(X_CONFIG, "|   |-->Monitor \"%s\"\n",
	    conf_monitor->mon_identifier);
    monitorp->id = conf_monitor->mon_identifier;
    monitorp->vendor = conf_monitor->mon_vendor;
    monitorp->model = conf_monitor->mon_modelname;
    monitorp->Modes = NULL;
    monitorp->Last = NULL;
    monitorp->gamma = zeros;
    monitorp->widthmm = conf_monitor->mon_width;
    monitorp->heightmm = conf_monitor->mon_height;
    monitorp->reducedblanking = FALSE;
    monitorp->maxPixClock = 0;
    monitorp->options = conf_monitor->mon_option_lst;

    /*
     * fill in the monitor structure
     */    
    for( count = 0 ;
	 count < conf_monitor->mon_n_hsync && count < MAX_HSYNC;
	 count++) {
        monitorp->hsync[count].hi = conf_monitor->mon_hsync[count].hi;
        monitorp->hsync[count].lo = conf_monitor->mon_hsync[count].lo;
    }
    monitorp->nHsync = count;
    for( count = 0 ;
	 count < conf_monitor->mon_n_vrefresh && count < MAX_VREFRESH;
	 count++) {
        monitorp->vrefresh[count].hi = conf_monitor->mon_vrefresh[count].hi;
        monitorp->vrefresh[count].lo = conf_monitor->mon_vrefresh[count].lo;
    }
    monitorp->nVrefresh = count;

    /*
     * first we collect the mode lines from the UseModes directive
     */
    while(modeslnk)
    {
        modes = xf86findModes (modeslnk->ml_modes_str, 
			       xf86configptr->conf_modes_lst);
	modeslnk->ml_modes = modes;
	
	    
	/* now add the modes found in the modes
	   section to the list of modes for this
	   monitor unless it has been added before
	   because we are reusing the same section 
	   for another screen */
	if (xf86itemNotSublist(
			       (GenericListPtr)conf_monitor->mon_modeline_lst,
			       (GenericListPtr)modes->mon_modeline_lst)) {
	    conf_monitor->mon_modeline_lst = (XF86ConfModeLinePtr)
	        xf86addListItem(
				(GenericListPtr)conf_monitor->mon_modeline_lst,
				(GenericListPtr)modes->mon_modeline_lst);
	}
	modeslnk = modeslnk->list.next;
    }

    /*
     * we need to hook in the mode lines now
     * here both data structures use lists, only our internal one
     * is double linked
     */
    cmodep = conf_monitor->mon_modeline_lst;
    while( cmodep ) {
        mode = xnfcalloc(1, sizeof(DisplayModeRec));
	mode->type       = 0;
        mode->Clock      = cmodep->ml_clock;
        mode->HDisplay   = cmodep->ml_hdisplay;
        mode->HSyncStart = cmodep->ml_hsyncstart;
        mode->HSyncEnd   = cmodep->ml_hsyncend;
        mode->HTotal     = cmodep->ml_htotal;
        mode->VDisplay   = cmodep->ml_vdisplay;
        mode->VSyncStart = cmodep->ml_vsyncstart;
        mode->VSyncEnd   = cmodep->ml_vsyncend;
        mode->VTotal     = cmodep->ml_vtotal;
        mode->Flags      = cmodep->ml_flags;
        mode->HSkew      = cmodep->ml_hskew;
        mode->VScan      = cmodep->ml_vscan;
        mode->name       = xnfstrdup(cmodep->ml_identifier);
        if( last ) {
            mode->prev = last;
            last->next = mode;
        }
        else {
            /*
             * this is the first mode
             */
            monitorp->Modes = mode;
            mode->prev = NULL;
        }
        last = mode;
        cmodep = (XF86ConfModeLinePtr)cmodep->list.next;
    }
    if(last){
      last->next = NULL;
    }
    monitorp->Last = last;

    /* add the (VESA) default modes */
    if (! addDefaultModes(monitorp) )
	return FALSE;

    if (conf_monitor->mon_gamma_red > GAMMA_ZERO)
	monitorp->gamma.red = conf_monitor->mon_gamma_red;
    if (conf_monitor->mon_gamma_green > GAMMA_ZERO)
	monitorp->gamma.green = conf_monitor->mon_gamma_green;
    if (conf_monitor->mon_gamma_blue > GAMMA_ZERO)
	monitorp->gamma.blue = conf_monitor->mon_gamma_blue;
    
    /* Check that the gamma values are within range */
    if (monitorp->gamma.red > GAMMA_ZERO &&
	(monitorp->gamma.red < GAMMA_MIN ||
	 monitorp->gamma.red > GAMMA_MAX)) {
	badgamma = monitorp->gamma.red;
    } else if (monitorp->gamma.green > GAMMA_ZERO &&
	(monitorp->gamma.green < GAMMA_MIN ||
	 monitorp->gamma.green > GAMMA_MAX)) {
	badgamma = monitorp->gamma.green;
    } else if (monitorp->gamma.blue > GAMMA_ZERO &&
	(monitorp->gamma.blue < GAMMA_MIN ||
	 monitorp->gamma.blue > GAMMA_MAX)) {
	badgamma = monitorp->gamma.blue;
    }
    if (badgamma > GAMMA_ZERO) {
	xf86ConfigError("Gamma value %.f is out of range (%.2f - %.1f)\n",
			badgamma, GAMMA_MIN, GAMMA_MAX);
	    return FALSE;
    }

    xf86ProcessOptions(-1, monitorp->options, MonitorOptions);
    xf86GetOptValBool(MonitorOptions, MON_REDUCEDBLANKING,
                      &monitorp->reducedblanking);
    xf86GetOptValFreq(MonitorOptions, MON_MAX_PIX_CLOCK, OPTUNITS_KHZ,
		      &monitorp->maxPixClock);
    return TRUE;
}

static int
lookupVisual(const char *visname)
{
    int i;

    if (!visname || !*visname)
	return -1;

    for (i = 0; i <= DirectColor; i++) {
	if (!xf86nameCompare(visname, xf86VisualNames[i]))
	    break;
    }

    if (i <= DirectColor)
	return i;

    return -1;
}


static Bool
configDisplay(DispPtr displayp, XF86ConfDisplayPtr conf_display)
{
    int count = 0;
    XF86ModePtr modep;
    
    displayp->frameX0           = conf_display->disp_frameX0;
    displayp->frameY0           = conf_display->disp_frameY0;
    displayp->virtualX          = conf_display->disp_virtualX;
    displayp->virtualY          = conf_display->disp_virtualY;
    displayp->depth             = conf_display->disp_depth;
    displayp->fbbpp             = conf_display->disp_bpp;
    displayp->weight.red        = conf_display->disp_weight.red;
    displayp->weight.green      = conf_display->disp_weight.green;
    displayp->weight.blue       = conf_display->disp_weight.blue;
    displayp->blackColour.red   = conf_display->disp_black.red;
    displayp->blackColour.green = conf_display->disp_black.green;
    displayp->blackColour.blue  = conf_display->disp_black.blue;
    displayp->whiteColour.red   = conf_display->disp_white.red;
    displayp->whiteColour.green = conf_display->disp_white.green;
    displayp->whiteColour.blue  = conf_display->disp_white.blue;
    displayp->options           = conf_display->disp_option_lst;
    if (conf_display->disp_visual) {
	displayp->defaultVisual = lookupVisual(conf_display->disp_visual);
	if (displayp->defaultVisual == -1) {
	    xf86ConfigError("Invalid visual name: \"%s\"",
			    conf_display->disp_visual);
	    return FALSE;
	}
    } else {
	displayp->defaultVisual = -1;
    }
	
    /*
     * now hook in the modes
     */
    modep = conf_display->disp_mode_lst;
    while(modep) {
        count++;
        modep = (XF86ModePtr)modep->list.next;
    }
    displayp->modes = xnfalloc((count+1) * sizeof(char*));
    modep = conf_display->disp_mode_lst;
    count = 0;
    while(modep) {
        displayp->modes[count] = modep->mode_name;
        count++;
        modep = (XF86ModePtr)modep->list.next;
    }
    displayp->modes[count] = NULL;
    
    return TRUE;
}

static Bool
configDevice(GDevPtr devicep, XF86ConfDevicePtr conf_device, Bool active)
{
    int i;

    if (!conf_device) {
        return FALSE;
    }

    if (active)
	xf86Msg(X_CONFIG, "|   |-->Device \"%s\"\n",
		conf_device->dev_identifier);
    else
	xf86Msg(X_CONFIG, "|-->Inactive Device \"%s\"\n",
		conf_device->dev_identifier);

    devicep->identifier = conf_device->dev_identifier;
    devicep->vendor = conf_device->dev_vendor;
    devicep->board = conf_device->dev_board;
    devicep->chipset = conf_device->dev_chipset;
    devicep->ramdac = conf_device->dev_ramdac;
    devicep->driver = conf_device->dev_driver;
    devicep->active = active;
    devicep->videoRam = conf_device->dev_videoram;
    devicep->BiosBase = conf_device->dev_bios_base;
    devicep->MemBase = conf_device->dev_mem_base;
    devicep->IOBase = conf_device->dev_io_base;
    devicep->clockchip = conf_device->dev_clockchip;
    devicep->busID = conf_device->dev_busid;
    devicep->textClockFreq = conf_device->dev_textclockfreq;
    devicep->chipID = conf_device->dev_chipid;
    devicep->chipRev = conf_device->dev_chiprev;
    devicep->options = conf_device->dev_option_lst;
    devicep->irq = conf_device->dev_irq;
    devicep->screen = conf_device->dev_screen;

    for (i = 0; i < MAXDACSPEEDS; i++) {
	if (i < CONF_MAXDACSPEEDS)
            devicep->dacSpeeds[i] = conf_device->dev_dacSpeeds[i];
	else
	    devicep->dacSpeeds[i] = 0;
    }
    devicep->numclocks = conf_device->dev_clocks;
    if (devicep->numclocks > MAXCLOCKS)
	devicep->numclocks = MAXCLOCKS;
    for (i = 0; i < devicep->numclocks; i++) {
	devicep->clock[i] = conf_device->dev_clock[i];
    }
    devicep->claimed = FALSE;

    return TRUE;
}

#ifdef XF86DRI
static void
configDRI(XF86ConfDRIPtr drip)
{
    int                count = 0;
    XF86ConfBuffersPtr bufs;
    int                i;
    struct group       *grp;

    xf86ConfigDRI.group      = -1;
    xf86ConfigDRI.mode       = 0;
    xf86ConfigDRI.bufs_count = 0;
    xf86ConfigDRI.bufs       = NULL;

    if (drip) {
	if (drip->dri_group_name) {
	    if ((grp = getgrnam(drip->dri_group_name)))
		xf86ConfigDRI.group = grp->gr_gid;
	} else {
	    if (drip->dri_group >= 0)
		xf86ConfigDRI.group = drip->dri_group;
	}
	xf86ConfigDRI.mode = drip->dri_mode;
	for (bufs = drip->dri_buffers_lst; bufs; bufs = bufs->list.next)
	    ++count;
	
	xf86ConfigDRI.bufs_count = count;
	xf86ConfigDRI.bufs = xnfalloc(count * sizeof(*xf86ConfigDRI.bufs));
	
	for (i = 0, bufs = drip->dri_buffers_lst;
	     i < count;
	     i++, bufs = bufs->list.next) {
	    
	    xf86ConfigDRI.bufs[i].count = bufs->buf_count;
	    xf86ConfigDRI.bufs[i].size  = bufs->buf_size;
				/* FIXME: Flags not implemented.  These
                                   could be used, for example, to specify a
                                   contiguous block and/or write-combining
                                   cache policy. */
	    xf86ConfigDRI.bufs[i].flags = 0;
	}
    }
}
#endif

static void
configExtensions(XF86ConfExtensionsPtr conf_ext)
{
    XF86OptionPtr o;

    if (conf_ext && conf_ext->ext_option_lst) {
	for (o = conf_ext->ext_option_lst; o; o = xf86NextOption(o)) {
	    char *name   = xf86OptionName(o);
	    char *val    = xf86OptionValue(o);
	    char *n;
	    Bool  enable = TRUE;

	    /* Handle "No<ExtensionName>" */
	    n = xf86NormalizeName(name);
	    if (strncmp(n, "no", 2) == 0) {
		name += 2;
		enable = FALSE;
	    }

	    if (!val ||
		xf86NameCmp(val, "enable") == 0 ||
		xf86NameCmp(val, "enabled") == 0 ||
		xf86NameCmp(val, "on") == 0 ||
		xf86NameCmp(val, "1") == 0 ||
		xf86NameCmp(val, "yes") == 0 ||
		xf86NameCmp(val, "true") == 0) {
		/* NOTHING NEEDED -- enabling is handled below */
	    } else if (xf86NameCmp(val, "disable") == 0 ||
                       xf86NameCmp(val, "disabled") == 0 ||
		       xf86NameCmp(val, "off") == 0 ||
		       xf86NameCmp(val, "0") == 0 ||
		       xf86NameCmp(val, "no") == 0 ||
		       xf86NameCmp(val, "false") == 0) {
		enable = !enable;
	    } else {
		xf86Msg(X_WARNING, "Ignoring unrecognized value \"%s\"\n", val);
		xfree(n);
		continue;
	    }

	    if (EnableDisableExtension(name, enable)) {
		xf86Msg(X_CONFIG, "Extension \"%s\" is %s\n",
			name, enable ? "enabled" : "disabled");
	    } else {
		xf86Msg(X_WARNING, "Ignoring unrecognized extension \"%s\"\n",
                        name);
	    }
	    xfree(n);
	}
    }
}

static Bool
configInput(IDevPtr inputp, XF86ConfInputPtr conf_input, MessageType from)
{
    xf86Msg(from, "|-->Input Device \"%s\"\n", conf_input->inp_identifier);
    inputp->identifier = conf_input->inp_identifier;
    inputp->driver = conf_input->inp_driver;
    inputp->commonOptions = conf_input->inp_option_lst;
    inputp->extraOptions = NULL;

    return TRUE;
}

static Bool
modeIsPresent(DisplayModePtr mode, MonPtr monitorp)
{
    DisplayModePtr knownmodes = monitorp->Modes;

    /* all I can think of is a linear search... */
    while(knownmodes != NULL)
    {
	if(!strcmp(mode->name, knownmodes->name) &&
	   !(knownmodes->type & M_T_DEFAULT))
	    return TRUE;
	knownmodes = knownmodes->next;
    }
    return FALSE;
}

static Bool
addDefaultModes(MonPtr monitorp)
{
    DisplayModePtr mode;
    DisplayModePtr last = monitorp->Last;
    int i = 0;

    for (i = 0; i < xf86NumDefaultModes; i++)
    {
	mode = xf86DuplicateMode(&xf86DefaultModes[i]);
	if (!modeIsPresent(mode, monitorp))
	{
	    monitorp->Modes = xf86ModesAdd(monitorp->Modes, mode);
	    last = mode;
	} else {
	    xfree(mode);
	}
    }
    monitorp->Last = last;

    return TRUE;
}

static void
checkInput(serverLayoutPtr layout, Bool implicit_layout) {
    checkCoreInputDevices(layout, implicit_layout);
}

/*
 * load the config file and fill the global data structure
 */
ConfigStatus
xf86HandleConfigFile(Bool autoconfig)
{
    const char *filename;
    char *searchpath;
    MessageType from = X_DEFAULT;
    char *scanptr;
    Bool singlecard = 0;
    Bool implicit_layout = FALSE;

    if (!autoconfig) {
	if (getuid() == 0)
	    searchpath = ROOT_CONFIGPATH;
	else
	    searchpath = USER_CONFIGPATH;

	if (xf86ConfigFile)
	    from = X_CMDLINE;

	filename = xf86openConfigFile(searchpath, xf86ConfigFile, PROJECTROOT);
	if (filename) {
	    xf86MsgVerb(from, 0, "Using config file: \"%s\"\n", filename);
	    xf86ConfigFile = xnfstrdup(filename);
	} else {
	    xf86Msg(X_ERROR, "Unable to locate/open config file");
	    if (xf86ConfigFile)
		xf86ErrorFVerb(0, ": \"%s\"", xf86ConfigFile);
	    xf86ErrorFVerb(0, "\n");
	    return CONFIG_NOFILE;
	}
    }
     
    if ((xf86configptr = xf86readConfigFile ()) == NULL) {
	xf86Msg(X_ERROR, "Problem parsing the config file\n");
	return CONFIG_PARSE_ERROR;
    }
    xf86closeConfigFile ();

    /* Initialise a few things. */

    /*
     * now we convert part of the information contained in the parser
     * structures into our own structures.
     * The important part here is to figure out which Screen Sections
     * in the XF86Config file are active so that we can piece together
     * the modes that we need later down the road.
     * And while we are at it, we'll decode the rest of the stuff as well
     */

    /* First check if a layout section is present, and if it is valid. */

    if (xf86configptr->conf_layout_lst == NULL || xf86ScreenName != NULL) {
	if (xf86ScreenName == NULL) {
	    xf86Msg(X_DEFAULT,
		    "No Layout section.  Using the first Screen section.\n");
	}
	if (!configImpliedLayout(&xf86ConfigLayout,
				 xf86configptr->conf_screen_lst)) {
            xf86Msg(X_ERROR, "Unable to determine the screen layout\n");
	    return CONFIG_PARSE_ERROR;
	}
	implicit_layout = TRUE;
    } else {
	if (xf86configptr->conf_flags != NULL) {
	  char *dfltlayout = NULL;
 	  pointer optlist = xf86configptr->conf_flags->flg_option_lst;
	
	  if (optlist && xf86FindOption(optlist, "defaultserverlayout"))
	    dfltlayout = xf86SetStrOption(optlist, "defaultserverlayout", NULL);
	  if (!configLayout(&xf86ConfigLayout, xf86configptr->conf_layout_lst,
			  dfltlayout)) {
	    xf86Msg(X_ERROR, "Unable to determine the screen layout\n");
	    return CONFIG_PARSE_ERROR;
	  }
	} else {
	  if (!configLayout(&xf86ConfigLayout, xf86configptr->conf_layout_lst,
			  NULL)) {
	    xf86Msg(X_ERROR, "Unable to determine the screen layout\n");
	    return CONFIG_PARSE_ERROR;
	  }
	}
    }

    xf86ProcessOptions(-1, xf86ConfigLayout.options, LayoutOptions);

    if ((scanptr = xf86GetOptValString(LayoutOptions, LAYOUT_ISOLATEDEVICE))) {
       ; /* IsolateDevice specified; overrides SingleCard */
    } else {
       xf86GetOptValBool(LayoutOptions, LAYOUT_SINGLECARD, &singlecard);
       if (singlecard)
           scanptr = xf86ConfigLayout.screens->screen->device->busID;
    }
    if (scanptr) {
       int bus, device, func;
       if (strncmp(scanptr, "PCI:", 4) != 0) {
           xf86Msg(X_WARNING, "Bus types other than PCI not yet isolable.\n"
                              "\tIgnoring IsolateDevice option.\n");
       } else if (sscanf(scanptr, "PCI:%d:%d:%d", &bus, &device, &func) == 3) {
           xf86IsolateDevice.domain = PCI_DOM_FROM_BUS(bus);
           xf86IsolateDevice.bus = PCI_BUS_NO_DOMAIN(bus);
           xf86IsolateDevice.dev = device;
           xf86IsolateDevice.func = func;
           xf86Msg(X_INFO,
                   "Isolating PCI bus \"%d:%d:%d\"\n", bus, device, func);
       }
    }

    /* Now process everything else */
    if (!configServerFlags(xf86configptr->conf_flags,xf86ConfigLayout.options)){
             ErrorF ("Problem when converting the config data structures\n");
             return CONFIG_PARSE_ERROR;
    }

    configFiles(xf86configptr->conf_files);
    configExtensions(xf86configptr->conf_extensions);
#ifdef XF86DRI
    configDRI(xf86configptr->conf_dri);
#endif

    checkInput(&xf86ConfigLayout, implicit_layout);

    /*
     * Handle some command line options that can override some of the
     * ServerFlags settings.
     */
#ifdef XF86VIDMODE
    if (xf86VidModeDisabled)
	xf86Info.vidModeEnabled = FALSE;
    if (xf86VidModeAllowNonLocal)
	xf86Info.vidModeAllowNonLocal = TRUE;
#endif

#ifdef XF86MISC
    if (xf86MiscModInDevDisabled)
	xf86Info.miscModInDevEnabled = FALSE;
    if (xf86MiscModInDevAllowNonLocal)
	xf86Info.miscModInDevAllowNonLocal = TRUE;
#endif

    if (xf86AllowMouseOpenFail)
	xf86Info.allowMouseOpenFail = TRUE;

    return CONFIG_OK;
}

Bool
xf86PathIsSafe(const char *path)
{
    return (xf86pathIsSafe(path) != 0);
}
