/*--------------------------------*-H-*---------------------------------*
 * File:	feature.h
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1997-1998   Oezguer Kesim <kesim@math.fu-berlin.de>
 * Copyright (c) 2005        William P. Y. Hadisoeseno <	 williampoetra@users.sourceforge.net>
 * Copyright (c) 2004-2005   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *----------------------------------------------------------------------*/
/*
** $Id: feature.h,v 1.28 2005/06/24 04:26:55 cvs Exp $
*/

#ifndef __FEATURE_H__
#define __FEATURE_H__

#ifndef X11USRLIBDIR
# define X11USRLIBDIR		"/usr/X11R6/lib"
#endif
#ifndef X11LIBDIR
# define X11LIBDIR		X11USRLIBDIR "/X11"
#endif
#ifndef XAPPLOADDIR
# define XAPPLOADDIR		X11LIBDIR "/app-defaults" 
# define XAPPLOADDIRLOCALE	X11LIBDIR "/%-.*s/app-defaults"
#endif

/* Check ~/.Xdefaults and ~/.Xresources as well */
#define HAVE_EXTRA_XRESOURCE_FILES


/*-----------------------SCREEN OPTIONS AND COLOURS---------------------*/
/*
 * Define the name of the environment variable to be used in
 * addition to the "PATH" environment and the `path' resource.
 * Usually it should point to where you keep your background pixmaps and/or
 * your menu files
 */
#define PATH_ENV	"RXVTPATH"

/*
 * Avoid enabling the colour cursor (-cr, cursorColor, cursorColor2)
 */
/* #define NO_CURSORCOLOR */

/*
 * Suppress use of BOLD and BLINK attributes for setting bright foreground
 * and background, respectively.  Simulate BOLD using colorBD, boldFont or
 * overstrike characters.
 */
/* #define NO_BRIGHTCOLOR */

/*
 * Make colours match xterm colours instead of `traditional' rxvt colours
 */
#define XTERM_COLORS

/*
 * Disable separate colours for bold, underline and reverse video
 */
/* #define NO_BOLD_UNDERLINE_REVERSE */

/*
 * Disable using simulated bold using overstrike.  You can also turn off
 * overstrike just for multi char fonts
 * Note: NO_BOLDOVERSTRIKE implies NO_BOLDOVERSTRIKE_MULTI
 */
/* #define NO_BOLDOVERSTRIKE */
/* #define NO_BOLDOVERSTRIKE_MULTI */

/*
 * Don't bother cleaning up pixel droppings.  If you don't use bold
 * overstrike then you don't usually need this
 */
/* #define NO_PIXEL_DROPPING_AVOIDANCE */

/*
 * Compile without support for real bold fonts
 */
/* #define NO_BOLDFONT */

/*
 * Add support for '-hc colour' for background highlight of selection
 */
/* #define OPTION_HC */

/*
 * Define maximum possible columns and rows
 */
#define MAX_COLS	1000
#define MAX_ROWS	1000

/*
 * Define default colours for certain items.  If you have a low colour
 * display, then consider using colours which are already pre-allocated:
 *   Black		(#000000)
 *   Red3		(#CD0000)	+ these
 *   Green3		(#00CD00)	+ colours
 *   Yellow3		(#CDCD00)	+ are
 *   Blue3		(#0000CD)	+ not
 *   Magenta3		(#CD00CD)	+ pre-allocated
 *   Cyan3		(#00CDCD)	+ if
 *   AntiqueWhite	(#FAEBD7)	+ NO_BRIGHTCOLOR
 *   Grey25		(#404040)	+ defined
 *   Red		(#FF0000)
 *   Green		(#00FF00)
 *   Yellow		(#FFFF00)
 *   Blue		(#0000FF)
 *   Magenta		(#FF00FF)
 *   Cyan		(#00FFFF)
 *   White		(#FFFFFF)
 */
/* These colours MUST be defined */
#define COLOR_FOREGROUND	"Black"
#define COLOR_BACKGROUND	"White"
#define COLOR_SCROLLBAR		"#B2B2B2"	/* scrollColor match Netscape */
#define COLOR_SCROLLTROUGH	"#969696"
/*
 * The cursor colours are special.  Be very careful about setting these:
 * foreground/background colours may be modified by command line or resources
 * prior to this allocation.  Also, they are not valid if NO_CURSORCOLOR is
 * defined
 */
#define COLOR_CURSOR_FOREGROUND	NULL	/* if NULL, use background colour */
#define COLOR_CURSOR_BACKGROUND	NULL	/* if NULL, use foreground colour */

/*
 * Define to remove support for XCopyArea() support.  XCopyArea() is useful
 * for scrolling on non-local X displays
 */
/* #define NO_SLOW_LINK_SUPPORT */

/*
 * Printer pipe which will be used for emulation of attached vt100 printer
 */
#define PRINTPIPE	"lpr"

/*
 * Allow 80/132 mode switching on startup
 */
/* #define ALLOW_132_MODE */
 
/*------------------------------RESOURCES-------------------------------*/
/*
 * Define to find installed application defaults for rxvt
 * Only if USE_XGETDEFAULT is not defined.
 */
/* #define USE_XAPPLOADDIR */

/*
 * Add support for the Offix DND (Drag 'n' Drop) protocol
 */
/* #define OFFIX_DND */

/*---------------------------------KEYS---------------------------------*/

/*
 * Define defaults for backspace and delete keys - unless they have been
 * configured out with --disable-backspace-key / --disable-delete-key
 */
/* #define DEFAULT_BACKSPACE	"DEC"		*/ /* SPECIAL */
/* #define DEFAULT_BACKSPACE	"\177"		*/
/* #define DEFAULT_DELETE	"\033[3~"	*/

/*
 * Choose one of these values to be the `hotkey' for changing font.
 * This has been superceded and is only for you older users
 */
/* #define HOTKEY_CTRL */
/* #define HOTKEY_META */

/*
 * Enable the keysym resource which allows you to define strings associated
 * with various KeySyms (0xFF00 - 0xFFFF).
 * Only works with the default hand-rolled resources.
 */
#ifndef NO_RESOURCES
# define KEYSYM_RESOURCE
#endif

/*
 * Modifier/s to use to allow up/down arrows and Priot/Next keys
 * to scroll single or page-fulls
 */
#define SCROLL_ON_SHIFT
/* #define SCROLL_ON_CTRL */
/* #define SCROLL_ON_META */

/*
 * Allow scrolling with modifier+Up/Down keys, in addition
 * to modifier+Prior/Next? (modifier is controlled with
 * SCROLL_ON_* defines above.). 
 * Also for modifier+Home/End keys to move to top/bottom
 */
/* #define SCROLL_ON_UPDOWN_KEYS */
/* #define SCROLL_ON_HOMEEND_KEYS */

/*
 * Allow unshifted Next/Prior keys to scroll forward/back
 * (in addition to shift+Next/shift+Prior)       --pjh
 */
/* #define UNSHIFTED_SCROLLKEYS */

/* (Hops) Set to choose a number of lines of context between pages
 *      (rather than a proportion (1/5) of savedlines buffer)
 *      when paging the savedlines with SHIFT-{Prior,Next} keys.
 */
#define PAGING_CONTEXT_LINES 1 /* */

/*
 * Have either Ctrl+Tab or Mod4+Tab emit \e\t
 * Useful when window manager grabs Alt+Tab   -- mg
 */
/* #define CTRL_TAB_MAKES_META */
/* #define MOD4_TAB_MAKES_META */

/*
 * default mode switch when greek keyboard is enabled (i.e. GREEK_SUPPORT)
 */
#ifndef GREEK_KEYBOARD_MODESWITCH
# define GREEK_KEYBOARD_MODESWITCH XK_Mode_switch
#endif

/*--------------------------------MOUSE---------------------------------*/
/*
 *  Enable jump scrolling when use mouse wheel
 */
#ifdef MOUSE_WHEEL
# define JUMP_MOUSE_WHEEL
#endif

/*
 * Disable sending escape sequences (up, down, page up/down)
 * from the scrollbar when XTerm mouse reporting is enabled
 */
/* #define NO_SCROLLBAR_REPORT */

/*
 * Default separating chars for multiple-click selection
 * Space and tab are separate separating characters and are not settable
 */
#define CUTCHARS	"\"&'()*,;<=>?@[\\]^`{|}~"

/*
 * Add run-time support for changing the cutchars for double click selection
 */
#define CUTCHAR_RESOURCE

/*
 * Have mouse reporting include double-click info for button1
 */
/* #define MOUSE_REPORT_DOUBLECLICK */

/*
 * Set delay between multiple click events [default: 500 milliseconds]
 */
/* #define MULTICLICK_TIME 500 */

/*
 * Time factor to slow down a `jumpy' mouse.  Motion isn't recognised until
 * this long after the last mouse click [default: 50 milliseconds]
 */
#define MOUSE_THRESHOLD		50

/*
 * If mouse wheel is defined, then scrolling is by 5 lines (or 1 line
 * if the shift key is down).  Scrolling can be smooth or jump scrolling
 */
/* #define JUMP_MOUSE_WHEEL */

/*
 * Set delay periods for continuous scrolling with scrollbar buttons
 */
/* #define SCROLLBAR_INITIAL_DELAY 40 */
/* #define SCROLLBAR_CONTINUOUS_DELAY 2 */

/*
 * The speed of selection scrolling is proportional to the distance
 * the mouse pointer is out of the text window.  This is the max
 * number of lines to scroll at a time.
 */
#define SELECTION_SCROLL_MAX_LINES 2

/*
 * The number of lines (measured in character's heights, not pixels)
 * the pointer must be out of the window for each increase in the
 * number of lines scrolled.
 */
#define SELECTION_SCROLL_LINE_SPEEDUP 4

/*--------------------------------MISC----------------------------------*/
/*
 * Disable to reset tty device to pre-incovation state
 */
#define RESET_TTY_TO_COMMON_DEFAULTS

/*
 * Only log in wtmp file when we're a login shell (-ls option)
 */
#define WTMP_ONLY_ON_LOGIN

/*
 * When pasting ACS characters, convert to similar normal characters
 */
/* #define ACS_ASCII */

/*
 * Default ascii characters to which ACS symbols are converted
 */
#ifdef ACS_ASCII
# define ACS_CHARS ".#    o+  ----+---__++--|<> =#. "
#endif

/*--------------------------------BELL----------------------------------*/
/*
 * Disable all bell indications
 */
/* #define NO_BELL */

/*
 * Disable automatic de-iconify when a bell is received
 */
/* #define NO_MAPALERT */

/*
 * Have mapAlert behaviour selectable with mapAlert resource
 */
#define MAPALERT_OPTION

/*-----------------------------SCROLL BAR-------------------------------*/
/*
 * Choose the rxvt style scrollbar width
 * - should be an even number [default: 10]
 */
/* #define SB_WIDTH_RXVT 10 */

/*
 * Minimum and maximum widths of the scrollbar (all styles)
 */
#define SB_WIDTH_MINIMUM 	5
#define SB_WIDTH_MAXIMUM 	100

/*
 * When using Rxvt scrollbar, clicking above or below the slider will move
 * 1/4 of the screen height, if possible.  Setting RXVT_SCROLL_FULL will move
 * it one screen height less one line, if possible
 */
#define RXVT_SCROLL_FULL 1

/*
 * (Hops) draw an internal border line on inside edge of the scrollbar
 */
/* #define SB_BORDER */

/*------------------------------MENU BAR--------------------------------*/
/*
 * Choose how many of (experimental) menuBars you want to be able to stack at
 * one time.
 *  A value of 1 disables menuBar stacking.
 *  A value of 0 disables menuBar all together.
 *  Note that the amount of memory overhead is the same for any value >= 2.
 */
#define MENUBAR_MAX 8

/*
 * Change the default shadow style
 */
/* #define MENUBAR_SHADOW_IN */

/*
 * Change the default shadow style
 */
#define MENU_SHADOW_IN

/*---------------------------MULTILINGUAL-------------------------------*/
/*
 * Allow run-time selection of Meta (Alt) to set the 8th bit on
 */
#define META8_OPTION

/*---------------------------DISPLAY OPTIONS----------------------------*/
/*
 * Force local connection to be socket (or other local) communication
 */
/* #define LOCAL_X_IS_UNIX */

/*
 * Have DISPLAY environment variable & "\E[7n" transmit display with IP number
 */
/* #define DISPLAY_IS_IP */

/*
 * Have "\E[7n" transmit the display name.
 * This has been cited as a potential security hole.
 */
/* #define ENABLE_DISPLAY_ANSWER */

/*
 * Change what ESC Z transmits instead of the default "\E[?1;2c"
 */
/* #define ESCZ_ANSWER	"\033[?1;2C" */

/*
 * Check the current value of the window-time/icon-name and avoid
 * re-setting it to the same value -- avoids unnecessary window refreshes
 */
#define SMART_WINDOW_TITLE

/*
 * Allow foreground/background colour to be changed with an
 * xterm escape sequence "\E]39;colour^G" -- still experimental
 */
#define XTERM_COLOR_CHANGE

/*
 * Remove secondary screen's independent cursor position, a la xterm
 */
/* #define NO_SECONDARY_SCREEN_CURSOR */

/*
 * If no secondary screen is available (NO_SECONDARY_SCREEN in config.h),
 * then scroll the text up on a window change request
 */
/* #define SCROLL_ON_NO_SECONDARY */

/* Width of the term internal border */
#define DEFAULT_INTERNALBORDERWIDTH	(0)
#define MAX_INTERNALBORDERWIDTH		(10)

/* Width of the term external border */
#define DEFAULT_EXTERNALBORDERWIDTH	(0)
#define MAX_EXTERNALBORDERWIDTH		(10)

/* Default number of extra dots between lines */
#define DEFAULT_LINESPACE			(0)
#define MAX_LINESPACE				(20)

/* Default seconds of pointer blank delay */
#define DEFAULT_BLANKDELAY			(2)
#define MAX_BLANKDELAY				(20)

/* Default number of lines in the scrollback buffer */
#define DEFAULT_SAVELINES			(100)
#define MAX_SAVELINES				(0xffff)

/* Default bell throttle interval */
/*
#define THROTTLE_BELL_MSEC			(500)
#define THROTTLE_BELL_COUNT			(3)
*/

/* Default tabstop size */
#define TABSTOP_SIZE				(8)

/* Maximal length and display length of tab title */
#define MAX_DISPLAY_TAB_TXT			(27)
#define MAX_TAB_TXT					(507)

/* Default XFT font pixel size */
#define MIN_XFT_FONT_SIZE			(8)
#define DEFAULT_XFT_FONT_SIZE		(12)

/* Default cursor blinking time (ms) */
#define MIN_BLINK_TIME				(300)
#define DEFAULT_BLINK_TIME			(500)
#define MAX_BLINK_TIME				(5000)

/* Make text cursor highlighting be the same as color of char
** that the cursor is currently on */
/*
#define CURSOR_COLOR_IS_RENDITION_COLOR
#define SIMULATE_LINUX_CONSOLE_CURSOR_COLOR
*/

/* Set tab title on Xterm escape sequence to change window title */
#define SET_TAB_TITLE_ON_XTERM_SEQUENCE
/* Set tab title on Xterm escape sequence but not change window title */
#define SET_TAB_TITLE_NOT_WIN_TITLE


/* Enable dead key. To do so, set option "XkbLayout" to "us_intl"
** in the keyboard device section. Then disable multichar encoding
(-km noenc) and use ISO8859-X fonts. */
#define USE_DEADKEY

/* Strip trailing spaces from selection */
#define DONT_SELECT_TRAILING_SPACES

/*
 * Provide termcap/terminfo bw support
 */
/* #define TERMCAP_HAS_BW */


#endif	/* __FEATURE_H__ */
/*----------------------- end-of-file (H source) -----------------------*/
