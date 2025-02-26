/*
 * Geeqie
 * (C) 2006 John Ellis
 * Copyright (C) 2008 - 2012 The Geeqie Team
 *
 * Author: John Ellis
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#include <glib/gstdio.h>
#include <errno.h>

#include "main.h"
#include "rcfile.h"

#include "bar.h"
#include "bar_comment.h"
#include "bar_exif.h"
#include "bar_histogram.h"
#include "bar_keywords.h"
#include "bar_sort.h"
#include "editors.h"
#include "filefilter.h"
#include "misc.h"
#include "pixbuf-renderer.h"
#include "secure_save.h"
#include "slideshow.h"
#include "ui_fileops.h"
#include "layout.h"
#include "layout_util.h"
#include "bar.h"
#include "metadata.h"
#include "bar_gps.h"


/*
 *-----------------------------------------------------------------------------
 * line write/parse routines (public)
 *-----------------------------------------------------------------------------
 */

void write_indent(GString *str, gint indent)
{
	g_string_append_printf(str, "\n%*s", indent * 4, "");
}

void write_char_option(GString *str, gint indent, const gchar *label, const gchar *text)
{
	/* this is needed for overlay string, because g_markup_escape_text does not handle \n and such, 
	   ideas for improvement are welcome
	*/
	static const unsigned char no_quote_utf[] = {
		0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
		0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
		0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
		0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
		0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
		0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
		0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
		0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
		0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
		0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
		'"',  0 /* '"' is handled in g_markup_escape_text */
	};

	gchar *escval1 = g_strescape(text ? text : "", (gchar *) no_quote_utf); 
	gchar *escval2 = g_markup_escape_text(escval1, -1);
	g_string_append_printf(str, "%s = \"%s\" ", label, escval2);
	g_free(escval2);
	g_free(escval1);
}

gboolean read_char_option(const gchar *option, const gchar *label, const gchar *value, gchar **text)
{
	if (g_ascii_strcasecmp(option, label) != 0) return FALSE;
	if (!text) return FALSE;

	g_free(*text);
	*text = g_strcompress(value);
	return TRUE;
}

/* Since gdk_color_to_string() is only available since gtk 2.12
 * here is an equivalent stub function. */
static gchar *color_to_string(GdkColor *color)
{
	return g_strdup_printf("#%04X%04X%04X", color->red, color->green, color->blue);
}

void write_color_option(GString *str, gint indent, gchar *label, GdkColor *color)
{
	if (color)
		{
		gchar *colorstring = color_to_string(color);

		write_char_option(str, indent, label, colorstring);
		g_free(colorstring);
		}
	else
		write_char_option(str, indent, label, "");
}

gboolean read_color_option(const gchar *option, const gchar *label, const gchar *value, GdkColor *color)
{
	if (g_ascii_strcasecmp(option, label) != 0) return FALSE;
	if (!color) return FALSE;

	if (!*value) return FALSE;
	gdk_color_parse(value, color);
	return TRUE;
}

void write_int_option(GString *str, gint indent, const gchar *label, gint n)
{
	g_string_append_printf(str, "%s = \"%d\" ", label, n);
}

gboolean read_int_option(const gchar *option, const gchar *label, const gchar *value, gint *n)
{
	if (g_ascii_strcasecmp(option, label) != 0) return FALSE;
	if (!n) return FALSE;

	if (g_ascii_isdigit(value[0]) || (value[0] == '-' && g_ascii_isdigit(value[1])))
		{
		*n = strtol(value, NULL, 10);
		}
	else
		{
		if (g_ascii_strcasecmp(value, "true") == 0)
			*n = 1;
		else
			*n = 0;
		}

	return TRUE;
}

void write_uint_option(GString *str, gint indent, const gchar *label, guint n)
{
	g_string_append_printf(str, "%s = \"%u\" ", label, n);
}

gboolean read_uint_option(const gchar *option, const gchar *label, const gchar *value, guint *n)
{
	if (g_ascii_strcasecmp(option, label) != 0) return FALSE;
	if (!n) return FALSE;

	if (g_ascii_isdigit(value[0]))
		{
		*n = strtoul(value, NULL, 10);
		}
	else
		{
		if (g_ascii_strcasecmp(value, "true") == 0)
			*n = 1;
		else
			*n = 0;
		}
	
	return TRUE;
}

gboolean read_uint_option_clamp(const gchar *option, const gchar *label, const gchar *value, guint *n, guint min, guint max)
{
	gboolean ret;

	ret = read_uint_option(option, label, value, n);
	if (ret) *n = CLAMP(*n, min, max);

	return ret;
}


gboolean read_int_option_clamp(const gchar *option, const gchar *label, const gchar *value, gint *n, gint min, gint max)
{
	gboolean ret;

	ret = read_int_option(option, label, value, n);
	if (ret) *n = CLAMP(*n, min, max);

	return ret;
}

void write_int_unit_option(GString *str, gint indent, gchar *label, gint n, gint subunits)
{
	gint l, r;

	if (subunits > 0)
		{
		l = n / subunits;
		r = n % subunits;
		}
	else
		{
		l = n;
		r = 0;
		}

	g_string_append_printf(str, "%s = \"%d.%d\" ", label, l, r);
}

gboolean read_int_unit_option(const gchar *option, const gchar *label, const gchar *value, gint *n, gint subunits)
{
	gint l, r;
	gchar *ptr, *buf;

	if (g_ascii_strcasecmp(option, label) != 0) return FALSE;
	if (!n) return FALSE;

	buf = g_strdup(value);
	ptr = buf;
	while (*ptr != '\0' && *ptr != '.') ptr++;
	if (*ptr == '.')
		{
		*ptr = '\0';
		l = strtol(value, NULL, 10);
		*ptr = '.';
		ptr++;
		r = strtol(ptr, NULL, 10);
		}
	else
		{
		l = strtol(value, NULL, 10);
		r = 0;
		}

	*n = l * subunits + r;
	g_free(buf);
	
	return TRUE;
}

void write_bool_option(GString *str, gint indent, gchar *label, gint n)
{
	g_string_append_printf(str, "%s = \"%s\" ", label, n ? "true" : "false");
}

gboolean read_bool_option(const gchar *option, const gchar *label, const gchar *value, gint *n)
{
	if (g_ascii_strcasecmp(option, label) != 0) return FALSE;
	if (!n) return FALSE;

	if (g_ascii_strcasecmp(value, "true") == 0 || atoi(value) != 0)
		*n = TRUE;
	else
		*n = FALSE;

	return TRUE;
}

/*
 *-----------------------------------------------------------------------------
 * write fuctions for elements (private)
 *-----------------------------------------------------------------------------
 */

static void write_global_attributes(GString *outstr, gint indent)
{
//	WRITE_SUBTITLE("General Options");

	WRITE_NL(); WRITE_BOOL(*options, show_icon_names);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_BOOL(*options, tree_descend_subdirs);
	WRITE_NL(); WRITE_BOOL(*options, lazy_image_sync);
	WRITE_NL(); WRITE_BOOL(*options, update_on_time_change);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_BOOL(*options, progressive_key_scrolling);

	WRITE_NL(); WRITE_UINT(*options, duplicates_similarity_threshold);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_BOOL(*options, mousewheel_scrolls);
	WRITE_NL(); WRITE_INT(*options, open_recent_list_maxsize);
	WRITE_NL(); WRITE_INT(*options, dnd_icon_size);
	WRITE_NL(); WRITE_BOOL(*options, place_dialogs_under_mouse);

	WRITE_NL(); WRITE_BOOL(*options, save_window_positions);
	WRITE_NL(); WRITE_BOOL(*options, tools_restore_state);

//	WRITE_SUBTITLE("File operations Options");

	WRITE_NL(); WRITE_BOOL(*options, file_ops.enable_in_place_rename);
	WRITE_NL(); WRITE_BOOL(*options, file_ops.confirm_delete);
	WRITE_NL(); WRITE_BOOL(*options, file_ops.enable_delete_key);
	WRITE_NL(); WRITE_BOOL(*options, file_ops.safe_delete_enable);
	WRITE_NL(); WRITE_CHAR(*options, file_ops.safe_delete_path);
	WRITE_NL(); WRITE_INT(*options, file_ops.safe_delete_folder_maxsize);




//	WRITE_SUBTITLE("Properties dialog Options");
	WRITE_NL(); WRITE_CHAR(*options, properties.tabs_order);

//	WRITE_SUBTITLE("Image Options");

	WRITE_NL(); WRITE_UINT(*options, image.zoom_mode);

//	g_string_append_printf(outstr, "# image.zoom_mode possible values are:\n"
//			    "#   original\n"
//			    "#   fit\n"
//			    "#   dont_change\n");
//	g_string_append_printf(outstr, "image.zoom_mode: ");
//	switch (options->image.zoom_mode)
//	{
//	case ZOOM_RESET_ORIGINAL: g_string_append_printf(outstr, "original\n"); break;
//	case ZOOM_RESET_FIT_WINDOW: g_string_append_printf(outstr, "fit\n"); break;
//	case ZOOM_RESET_NONE: g_string_append_printf(outstr, "dont_change\n"); break;
//	}
	WRITE_SEPARATOR();
	WRITE_NL(); WRITE_BOOL(*options, image.zoom_2pass);
	WRITE_NL(); WRITE_BOOL(*options, image.zoom_to_fit_allow_expand);
	WRITE_NL(); WRITE_UINT(*options, image.zoom_quality);
	WRITE_NL(); WRITE_INT(*options, image.zoom_increment);
	WRITE_NL(); WRITE_BOOL(*options, image.fit_window_to_image);
	WRITE_NL(); WRITE_BOOL(*options, image.limit_window_size);
	WRITE_NL(); WRITE_INT(*options, image.max_window_size);
	WRITE_NL(); WRITE_BOOL(*options, image.limit_autofit_size);
	WRITE_NL(); WRITE_INT(*options, image.max_autofit_size);
	WRITE_NL(); WRITE_UINT(*options, image.scroll_reset_method);
	WRITE_NL(); WRITE_INT(*options, image.tile_cache_max);
	WRITE_NL(); WRITE_INT(*options, image.image_cache_max);
	WRITE_NL(); WRITE_UINT(*options, image.dither_quality);
	WRITE_NL(); WRITE_BOOL(*options, image.enable_read_ahead);
	WRITE_NL(); WRITE_BOOL(*options, image.exif_rotate_enable);
	WRITE_NL(); WRITE_BOOL(*options, image.use_custom_border_color);
	WRITE_NL(); WRITE_BOOL(*options, image.use_custom_border_color_in_fullscreen);
	WRITE_NL(); WRITE_COLOR(*options, image.border_color);

//	WRITE_SUBTITLE("Thumbnails Options");

	WRITE_NL(); WRITE_INT(*options, thumbnails.max_width);
	WRITE_NL(); WRITE_INT(*options, thumbnails.max_height);
	WRITE_NL(); WRITE_BOOL(*options, thumbnails.enable_caching);
	WRITE_NL(); WRITE_BOOL(*options, thumbnails.cache_into_dirs);
	WRITE_NL(); WRITE_BOOL(*options, thumbnails.use_xvpics);
	WRITE_NL(); WRITE_BOOL(*options, thumbnails.spec_standard);
	WRITE_NL(); WRITE_UINT(*options, thumbnails.quality);
	WRITE_NL(); WRITE_BOOL(*options, thumbnails.use_exif);


//	WRITE_SUBTITLE("File sorting Options");

	WRITE_NL(); WRITE_INT(*options, file_sort.method);
	WRITE_NL(); WRITE_BOOL(*options, file_sort.ascending);
	WRITE_NL(); WRITE_BOOL(*options, file_sort.case_sensitive);


//	WRITE_SUBTITLE("Fullscreen Options");

	WRITE_NL(); WRITE_INT(*options, fullscreen.screen);
	WRITE_NL(); WRITE_BOOL(*options, fullscreen.clean_flip);
	WRITE_NL(); WRITE_BOOL(*options, fullscreen.disable_saver);
	WRITE_NL(); WRITE_BOOL(*options, fullscreen.above);

	WRITE_SEPARATOR();

//	WRITE_SUBTITLE("Image Overlay Options");
	WRITE_NL(); WRITE_CHAR(*options, image_overlay.template_string);

//	g_string_append_printf(outstr, "# these are relative positions:\n");
//	g_string_append_printf(outstr, "# x >= 0: |x| pixels from left border\n");
//	g_string_append_printf(outstr, "# x < 0 : |x| pixels from right border\n");
//	g_string_append_printf(outstr, "# y >= 0: |y| pixels from top border\n");
//	g_string_append_printf(outstr, "# y < 0 : |y| pixels from bottom border\n");
	WRITE_NL(); WRITE_INT(*options, image_overlay.x);
	WRITE_NL(); WRITE_INT(*options, image_overlay.y);


//	WRITE_SUBTITLE("Slideshow Options");

	WRITE_NL(); WRITE_INT_UNIT(*options, slideshow.delay, SLIDESHOW_SUBSECOND_PRECISION);
	WRITE_NL(); WRITE_BOOL(*options, slideshow.random);
	WRITE_NL(); WRITE_BOOL(*options, slideshow.repeat);


//	WRITE_SUBTITLE("Collection Options");

	WRITE_NL(); WRITE_BOOL(*options, collections.rectangular_selection);


//	WRITE_SUBTITLE("Filtering Options");

	WRITE_NL(); WRITE_BOOL(*options, file_filter.show_hidden_files);
	WRITE_NL(); WRITE_BOOL(*options, file_filter.show_dot_directory);
	WRITE_NL(); WRITE_BOOL(*options, file_filter.disable);
	WRITE_SEPARATOR();


//	WRITE_SUBTITLE("Sidecars Options");

	WRITE_NL(); WRITE_CHAR(*options, sidecar.ext);



//	WRITE_SUBTITLE("Shell command");
	WRITE_NL(); WRITE_CHAR(*options, shell.path);
	WRITE_NL(); WRITE_CHAR(*options, shell.options);


//	WRITE_SUBTITLE("Helpers");
//	g_string_append_printf(outstr, "# Html browser\n");
//	g_string_append_printf(outstr, "# command_name is: the binary's name to look for in the path\n");
//	g_string_append_printf(outstr, "# If command_name is empty, the program will try various common html browsers\n");
//	g_string_append_printf(outstr, "# command_line is:\n");
//	g_string_append_printf(outstr, "# \"\" (empty string)  = execute binary with html file path as command line\n");
//	g_string_append_printf(outstr, "# \"string\"           = execute string and use results for command line\n");
//	g_string_append_printf(outstr, "# \"!string\"          = use text following ! as command line, replacing optional %%s with html file path\n");
	WRITE_NL(); WRITE_CHAR(*options, helpers.html_browser.command_name);
	WRITE_NL(); WRITE_CHAR(*options, helpers.html_browser.command_line);

/* FIXME:
	WRITE_SUBTITLE("Exif Options");
	g_string_append_printf(outstr, "# Display: 0: never\n"
			    "#          1: if set\n"
			    "#          2: always\n\n");
	for (i = 0; ExifUIList[i].key; i++)
		{
		g_string_append_printf(outstr, "exif.display.");
		write_int_option(outstr, 2, (gchar *)ExifUIList[i].key, ExifUIList[i].current);
		}
*/

//	WRITE_SUBTITLE("Metadata Options");
	WRITE_NL(); WRITE_BOOL(*options, metadata.enable_metadata_dirs);
	WRITE_NL(); WRITE_BOOL(*options, metadata.save_in_image_file); 
	WRITE_NL(); WRITE_BOOL(*options, metadata.save_legacy_IPTC);
	WRITE_NL(); WRITE_BOOL(*options, metadata.warn_on_write_problems);
	WRITE_NL(); WRITE_BOOL(*options, metadata.save_legacy_format);
	WRITE_NL(); WRITE_BOOL(*options, metadata.sync_grouped_files);
	WRITE_NL(); WRITE_BOOL(*options, metadata.confirm_write);
	WRITE_NL(); WRITE_INT(*options, metadata.confirm_timeout);
	WRITE_NL(); WRITE_BOOL(*options, metadata.confirm_after_timeout);
	WRITE_NL(); WRITE_BOOL(*options, metadata.confirm_on_image_change);
	WRITE_NL(); WRITE_BOOL(*options, metadata.confirm_on_dir_change);
	WRITE_NL(); WRITE_BOOL(*options, metadata.keywords_case_sensitive);
	WRITE_NL(); WRITE_BOOL(*options, metadata.write_orientation);

	WRITE_NL(); WRITE_UINT(*options, stereo.mode);
	WRITE_NL(); WRITE_UINT(*options, stereo.fsmode);
	WRITE_NL(); WRITE_BOOL(*options, stereo.enable_fsmode);
	WRITE_NL(); WRITE_UINT(*options, stereo.fixed_w);
	WRITE_NL(); WRITE_UINT(*options, stereo.fixed_h);
	WRITE_NL(); WRITE_UINT(*options, stereo.fixed_x1);
	WRITE_NL(); WRITE_UINT(*options, stereo.fixed_y1);
	WRITE_NL(); WRITE_UINT(*options, stereo.fixed_x2);
	WRITE_NL(); WRITE_UINT(*options, stereo.fixed_y2);
}

static void write_color_profile(GString *outstr, gint indent)
{
	gint i;
#ifndef HAVE_LCMS
	g_string_append_printf(outstr, "<!-- NOTICE: %s was not built with support for color profiles,\n"
			    "         color profile options will have no effect.\n-->\n", GQ_APPNAME);
#endif

	WRITE_NL(); WRITE_STRING("<color_profiles ");
	WRITE_CHAR(options->color_profile, screen_file);
	WRITE_BOOL(options->color_profile, enabled);
	WRITE_BOOL(options->color_profile, use_image);
	WRITE_INT(options->color_profile, input_type);
	WRITE_BOOL(options->color_profile, use_x11_screen_profile);
	WRITE_STRING(">");

	indent++;
	for (i = 0; i < COLOR_PROFILE_INPUTS; i++)
		{
		WRITE_NL(); WRITE_STRING("<profile ");
		write_char_option(outstr, indent, "input_file", options->color_profile.input_file[i]);
		write_char_option(outstr, indent, "input_name", options->color_profile.input_name[i]);
		WRITE_STRING("/>");
		}
	indent--;
	WRITE_NL(); WRITE_STRING("</color_profiles>");
}


/*
 *-----------------------------------------------------------------------------
 * save configuration (public)
 *-----------------------------------------------------------------------------
 */

gboolean save_config_to_file(const gchar *utf8_path, ConfOptions *options)
{
	SecureSaveInfo *ssi;
	gchar *rc_pathl;
	GString *outstr;
	gint indent = 0;
	GList *work;
	
	rc_pathl = path_from_utf8(utf8_path);
	ssi = secure_open(rc_pathl);
	g_free(rc_pathl);
	if (!ssi)
		{
		log_printf(_("error saving config file: %s\n"), utf8_path);
		return FALSE;
		}

	outstr = g_string_new("");
	g_string_append_printf(outstr, "<!--\n");
	g_string_append_printf(outstr, "######################################################################\n");
	g_string_append_printf(outstr, "# %30s config file      version %-10s #\n", GQ_APPNAME, VERSION);
	g_string_append_printf(outstr, "######################################################################\n");
	WRITE_SEPARATOR();

	WRITE_STRING("# Note: This file is autogenerated. Options can be changed here,\n");
	WRITE_STRING("#       but user comments and formatting will be lost.\n");
	WRITE_SEPARATOR();
	WRITE_STRING("-->\n");
	WRITE_SEPARATOR();

	WRITE_STRING("<gq>\n");
	indent++;
	
	WRITE_NL(); WRITE_STRING("<global\n");
	indent++;
	write_global_attributes(outstr, indent + 1);
	indent--;
	WRITE_STRING(">\n");

	indent++;

	write_color_profile(outstr, indent);

	WRITE_SEPARATOR();
	filter_write_list(outstr, indent);

	WRITE_SEPARATOR();
	keyword_tree_write_config(outstr, indent);
	indent--;
	WRITE_NL(); WRITE_STRING("</global>\n");

	WRITE_SEPARATOR();
	WRITE_SUBTITLE("Layout Options");

	work = layout_window_list;
	while (work)
		{
		LayoutWindow *lw = work->data;
		layout_write_config(lw, outstr, indent);
		work = work->next;
		}

	indent--;
	WRITE_NL(); WRITE_STRING("</gq>\n");
	WRITE_SEPARATOR();

	secure_fputs(ssi, outstr->str);
	g_string_free(outstr, TRUE);

	if (secure_close(ssi))
		{
		log_printf(_("error saving config file: %s\nerror: %s\n"), utf8_path,
			   secsave_strerror(secsave_errno));
		return FALSE;
		}

	return TRUE;
}

/*
 *-----------------------------------------------------------------------------
 * loading attributes for elements (private)
 *-----------------------------------------------------------------------------
 */


static gboolean load_global_params(const gchar **attribute_names, const gchar **attribute_values)
{
	while (*attribute_names)
		{
		const gchar *option = *attribute_names++;
		const gchar *value = *attribute_values++;


		/* general options */
		if (READ_BOOL(*options, show_icon_names)) continue;

		if (READ_BOOL(*options, tree_descend_subdirs)) continue;
		if (READ_BOOL(*options, lazy_image_sync)) continue;
		if (READ_BOOL(*options, update_on_time_change)) continue;

		if (READ_UINT_CLAMP(*options, duplicates_similarity_threshold, 0, 100)) continue;

		if (READ_BOOL(*options, progressive_key_scrolling)) continue;

		if (READ_BOOL(*options, mousewheel_scrolls)) continue;

		if (READ_INT(*options, open_recent_list_maxsize)) continue;
		if (READ_INT(*options, dnd_icon_size)) continue;
		if (READ_BOOL(*options, place_dialogs_under_mouse)) continue;

		if (READ_BOOL(*options, save_window_positions)) continue;
		if (READ_BOOL(*options, tools_restore_state)) continue;

		/* properties dialog options */
		if (READ_CHAR(*options, properties.tabs_order)) continue;

		/* image options */
		if (READ_UINT_CLAMP(*options, image.zoom_mode, 0, ZOOM_RESET_NONE)) continue;
		if (READ_BOOL(*options, image.zoom_2pass)) continue;
		if (READ_BOOL(*options, image.zoom_to_fit_allow_expand)) continue;
		if (READ_BOOL(*options, image.fit_window_to_image)) continue;
		if (READ_BOOL(*options, image.limit_window_size)) continue;
		if (READ_INT(*options, image.max_window_size)) continue;
		if (READ_BOOL(*options, image.limit_autofit_size)) continue;
		if (READ_INT(*options, image.max_autofit_size)) continue;
		if (READ_UINT_CLAMP(*options, image.scroll_reset_method, 0, PR_SCROLL_RESET_COUNT - 1)) continue;
		if (READ_INT(*options, image.tile_cache_max)) continue;
		if (READ_INT(*options, image.image_cache_max)) continue;
		if (READ_UINT_CLAMP(*options, image.zoom_quality, GDK_INTERP_NEAREST, GDK_INTERP_HYPER)) continue;
		if (READ_UINT_CLAMP(*options, image.dither_quality, GDK_RGB_DITHER_NONE, GDK_RGB_DITHER_MAX)) continue;
		if (READ_INT(*options, image.zoom_increment)) continue;
		if (READ_BOOL(*options, image.enable_read_ahead)) continue;
		if (READ_BOOL(*options, image.exif_rotate_enable)) continue;
		if (READ_BOOL(*options, image.use_custom_border_color)) continue;
		if (READ_BOOL(*options, image.use_custom_border_color_in_fullscreen)) continue;
		if (READ_COLOR(*options, image.border_color)) continue;

		/* thumbnails options */
		if (READ_INT_CLAMP(*options, thumbnails.max_width, 16, 512)) continue;
		if (READ_INT_CLAMP(*options, thumbnails.max_height, 16, 512)) continue;

		if (READ_BOOL(*options, thumbnails.enable_caching)) continue;
		if (READ_BOOL(*options, thumbnails.cache_into_dirs)) continue;
		if (READ_BOOL(*options, thumbnails.use_xvpics)) continue;
		if (READ_BOOL(*options, thumbnails.spec_standard)) continue;
		if (READ_UINT_CLAMP(*options, thumbnails.quality, GDK_INTERP_NEAREST, GDK_INTERP_HYPER)) continue;
		if (READ_BOOL(*options, thumbnails.use_exif)) continue;

		/* file sorting options */
		if (READ_UINT(*options, file_sort.method)) continue;
		if (READ_BOOL(*options, file_sort.ascending)) continue;
		if (READ_BOOL(*options, file_sort.case_sensitive)) continue;

		/* file operations *options */
		if (READ_BOOL(*options, file_ops.enable_in_place_rename)) continue;
		if (READ_BOOL(*options, file_ops.confirm_delete)) continue;
		if (READ_BOOL(*options, file_ops.enable_delete_key)) continue;
		if (READ_BOOL(*options, file_ops.safe_delete_enable)) continue;
		if (READ_CHAR(*options, file_ops.safe_delete_path)) continue;
		if (READ_INT(*options, file_ops.safe_delete_folder_maxsize)) continue;

		/* fullscreen options */
		if (READ_INT(*options, fullscreen.screen)) continue;
		if (READ_BOOL(*options, fullscreen.clean_flip)) continue;
		if (READ_BOOL(*options, fullscreen.disable_saver)) continue;
		if (READ_BOOL(*options, fullscreen.above)) continue;

		/* image overlay */
		if (READ_CHAR(*options, image_overlay.template_string)) continue;
		if (READ_INT(*options, image_overlay.x)) continue;
		if (READ_INT(*options, image_overlay.y)) continue;


		/* slideshow options */
		if (READ_INT_UNIT(*options, slideshow.delay, SLIDESHOW_SUBSECOND_PRECISION)) continue;
		if (READ_BOOL(*options, slideshow.random)) continue;
		if (READ_BOOL(*options, slideshow.repeat)) continue;

		/* collection options */

		if (READ_BOOL(*options, collections.rectangular_selection)) continue;

		/* filtering options */

		if (READ_BOOL(*options, file_filter.show_hidden_files)) continue;
		if (READ_BOOL(*options, file_filter.show_dot_directory)) continue;
		if (READ_BOOL(*options, file_filter.disable)) continue;
		if (READ_CHAR(*options, sidecar.ext)) continue;

		/* Color Profiles */

		/* Shell command */
		if (READ_CHAR(*options, shell.path)) continue;
		if (READ_CHAR(*options, shell.options)) continue;

		/* Helpers */
		if (READ_CHAR(*options, helpers.html_browser.command_name)) continue;
		if (READ_CHAR(*options, helpers.html_browser.command_line)) continue;
		/* Exif */
/*
		if (0 == g_ascii_strncasecmp(option, "exif.display.", 13))
			{
			for (i = 0; ExifUIList[i].key; i++)
				if (0 == g_ascii_strcasecmp(option + 13, ExifUIList[i].key))
					ExifUIList[i].current = strtol(value, NULL, 10);
			continue;
			}
*/
		/* metadata */		
		if (READ_BOOL(*options, metadata.enable_metadata_dirs)) continue;
		if (READ_BOOL(*options, metadata.save_in_image_file)) continue;
		if (READ_BOOL(*options, metadata.save_legacy_IPTC)) continue;
		if (READ_BOOL(*options, metadata.warn_on_write_problems)) continue;
		if (READ_BOOL(*options, metadata.save_legacy_format)) continue;
		if (READ_BOOL(*options, metadata.sync_grouped_files)) continue;
		if (READ_BOOL(*options, metadata.confirm_write)) continue;
		if (READ_BOOL(*options, metadata.confirm_after_timeout)) continue;
		if (READ_INT(*options, metadata.confirm_timeout)) continue;
		if (READ_BOOL(*options, metadata.confirm_on_image_change)) continue;
		if (READ_BOOL(*options, metadata.confirm_on_dir_change)) continue;
		if (READ_BOOL(*options, metadata.keywords_case_sensitive)) continue;
		if (READ_BOOL(*options, metadata.write_orientation)) continue;

		if (READ_UINT(*options, stereo.mode)) continue;
		if (READ_UINT(*options, stereo.fsmode)) continue;
		if (READ_BOOL(*options, stereo.enable_fsmode)) continue;
		if (READ_UINT(*options, stereo.fixed_w)) continue;
		if (READ_UINT(*options, stereo.fixed_h)) continue;
		if (READ_UINT(*options, stereo.fixed_x1)) continue;
		if (READ_UINT(*options, stereo.fixed_y1)) continue;
		if (READ_UINT(*options, stereo.fixed_x2)) continue;
		if (READ_UINT(*options, stereo.fixed_y2)) continue;

		log_printf("unknown attribute %s = %s\n", option, value);
		}

	return TRUE;
}

static void options_load_color_profiles(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	while (*attribute_names)
		{
		const gchar *option = *attribute_names++;
		const gchar *value = *attribute_values++;

		if (READ_BOOL(options->color_profile, enabled)) continue;
		if (READ_BOOL(options->color_profile, use_image)) continue;
		if (READ_INT(options->color_profile, input_type)) continue;
		if (READ_CHAR(options->color_profile, screen_file)) continue;
		if (READ_BOOL(options->color_profile, use_x11_screen_profile)) continue;

		log_printf("unknown attribute %s = %s\n", option, value);
		}

}

static void options_load_profile(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	gint i = GPOINTER_TO_INT(data);
	if (i < 0 || i >= COLOR_PROFILE_INPUTS) return;
	while (*attribute_names)
		{
		const gchar *option = *attribute_names++;
		const gchar *value = *attribute_values++;

		if (READ_CHAR_FULL("input_file", options->color_profile.input_file[i])) continue;
		if (READ_CHAR_FULL("input_name", options->color_profile.input_name[i])) continue;

		log_printf("unknown attribute %s = %s\n", option, value);
		}
	i++;
	options_parse_func_set_data(parser_data, GINT_TO_POINTER(i));

}



/*
 *-----------------------------------------------------------------------------
 * xml file structure (private)
 *-----------------------------------------------------------------------------
 */
struct _GQParserData
{
	GList *parse_func_stack;
	gboolean startup; /* reading config for the first time - add commandline and defaults */
};

static const gchar *options_get_id(const gchar **attribute_names, const gchar **attribute_values)
{
	while (*attribute_names)
		{
		const gchar *option = *attribute_names++;
		const gchar *value = *attribute_values++;
		
		if (strcmp(option, "id") == 0) return value;

		}
	return NULL;
}


void options_parse_leaf(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	log_printf("unexpected: %s\n", element_name);
	options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
}

static void options_parse_color_profiles(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	if (g_ascii_strcasecmp(element_name, "profile") == 0)
		{
		options_load_profile(parser_data, context, element_name, attribute_names, attribute_values, data, error);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <profile>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_filter(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	if (g_ascii_strcasecmp(element_name, "file_type") == 0)
		{
		filter_load_file_type(attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <filter>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_filter_end(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, gpointer data, GError **error)
{
	if (parser_data->startup) filter_add_defaults();
	filter_rebuild(); 
}

static void options_parse_keyword_end(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, gpointer data, GError **error)
{
	GtkTreeIter *iter_ptr = data;
	gtk_tree_iter_free(iter_ptr);
}


static void options_parse_keyword(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	GtkTreeIter *iter_ptr = data;
	if (g_ascii_strcasecmp(element_name, "keyword") == 0)
		{
		GtkTreeIter *child = keyword_add_from_config(keyword_tree, iter_ptr, attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_keyword, options_parse_keyword_end, child);
		}
	else
		{
		log_printf("unexpected in <keyword>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}



static void options_parse_keyword_tree(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	if (g_ascii_strcasecmp(element_name, "keyword") == 0)
		{
		GtkTreeIter *iter_ptr = keyword_add_from_config(keyword_tree, NULL, attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_keyword, options_parse_keyword_end, iter_ptr);
		}
	else
		{
		log_printf("unexpected in <keyword_tree>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}


static void options_parse_global(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	if (g_ascii_strcasecmp(element_name, "color_profiles") == 0)
		{
		options_load_color_profiles(parser_data, context, element_name, attribute_names, attribute_values, data, error);
		options_parse_func_push(parser_data, options_parse_color_profiles, NULL, GINT_TO_POINTER(0));
		}
	else if (g_ascii_strcasecmp(element_name, "filter") == 0)
		{
		options_parse_func_push(parser_data, options_parse_filter, options_parse_filter_end, NULL);
		}
	else if (g_ascii_strcasecmp(element_name, "keyword_tree") == 0)
		{
		if (!keyword_tree) keyword_tree_new();
		options_parse_func_push(parser_data, options_parse_keyword_tree, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <global>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_global_end(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, gpointer data, GError **error)
{
#ifndef HAVE_EXIV2
	/* some options do not work without exiv2 */
	options->metadata.save_in_image_file = FALSE;
	options->metadata.save_legacy_format = TRUE;
	options->metadata.write_orientation = FALSE;
	DEBUG_1("compiled without Exiv2 - disabling XMP write support");
#endif
}

static void options_parse_pane_exif(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	GtkWidget *pane = data;
	if (g_ascii_strcasecmp(element_name, "entry") == 0)
		{
		bar_pane_exif_entry_add_from_config(pane, attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <pane_exif>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_bar(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	GtkWidget *bar = data;
	if (g_ascii_strcasecmp(element_name, "pane_comment") == 0)
		{
		GtkWidget *pane = bar_find_pane_by_id(bar, PANE_COMMENT, options_get_id(attribute_names, attribute_values));
		if (pane)
			{
			bar_pane_comment_update_from_config(pane, attribute_names, attribute_values);
			}
		else
			{
			pane = bar_pane_comment_new_from_config(attribute_names, attribute_values);
			bar_add(bar, pane);
			}
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
#ifdef HAVE_LIBCHAMPLAIN
#ifdef HAVE_LIBCHAMPLAIN_GTK
	else if (g_ascii_strcasecmp(element_name, "pane_gps") == 0)
		{
		GtkWidget *pane = bar_find_pane_by_id(bar, PANE_GPS, options_get_id(attribute_names, attribute_values));
		if (pane)
			{
			bar_pane_gps_update_from_config(pane, attribute_names, attribute_values);
			}
		else
			{
			pane = bar_pane_gps_new_from_config(attribute_names, attribute_values);
			bar_add(bar, pane);
			}
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
#endif
#endif
	else if (g_ascii_strcasecmp(element_name, "pane_exif") == 0)
		{
		GtkWidget *pane = bar_find_pane_by_id(bar, PANE_EXIF, options_get_id(attribute_names, attribute_values));
		if (pane)
			{
			bar_pane_exif_update_from_config(pane, attribute_names, attribute_values);
			}
		else
			{
			pane = bar_pane_exif_new_from_config(attribute_names, attribute_values);
			bar_add(bar, pane);
			}
		options_parse_func_push(parser_data, options_parse_pane_exif, NULL, pane);
		}
	else if (g_ascii_strcasecmp(element_name, "pane_histogram") == 0)
		{
		GtkWidget *pane = bar_find_pane_by_id(bar, PANE_HISTOGRAM, options_get_id(attribute_names, attribute_values));
		if (pane)
			{
			bar_pane_histogram_update_from_config(pane, attribute_names, attribute_values);
			}
		else
			{
			pane = bar_pane_histogram_new_from_config(attribute_names, attribute_values);
			bar_add(bar, pane);
			}
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else if (g_ascii_strcasecmp(element_name, "pane_keywords") == 0)
		{
		GtkWidget *pane = bar_find_pane_by_id(bar, PANE_KEYWORDS, options_get_id(attribute_names, attribute_values));
		if (pane)
			{
			bar_pane_keywords_update_from_config(pane, attribute_names, attribute_values);
			}
		else
			{
			pane = bar_pane_keywords_new_from_config(attribute_names, attribute_values);
			bar_add(bar, pane);
			}
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else if (g_ascii_strcasecmp(element_name, "clear") == 0)
		{
		bar_clear(bar);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <bar>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_toolbar(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	LayoutWindow *lw = data;
	if (g_ascii_strcasecmp(element_name, "toolitem") == 0)
		{
		layout_toolbar_add_from_config(lw, TOOLBAR_MAIN, attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else if (g_ascii_strcasecmp(element_name, "clear") == 0)
		{
		layout_toolbar_clear(lw, TOOLBAR_MAIN);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <toolbar>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_statusbar(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	LayoutWindow *lw = data;
	if (g_ascii_strcasecmp(element_name, "toolitem") == 0)
		{
		layout_toolbar_add_from_config(lw, TOOLBAR_STATUS, attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else if (g_ascii_strcasecmp(element_name, "clear") == 0)
		{
		layout_toolbar_clear(lw, TOOLBAR_STATUS);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else
		{
		log_printf("unexpected in <statusbar>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_layout(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	LayoutWindow *lw = data;
	if (g_ascii_strcasecmp(element_name, "bar") == 0)
		{
		if (!lw->bar)
			{
			GtkWidget *bar = bar_new_from_config(lw, attribute_names, attribute_values);
			layout_bar_set(lw, bar);
			}
		else
			{
			bar_update_from_config(lw->bar, attribute_names, attribute_values);
			}
			
		options_parse_func_push(parser_data, options_parse_bar, NULL, lw->bar);
		}
	else if (g_ascii_strcasecmp(element_name, "bar_sort") == 0)
		{
		GtkWidget *bar = bar_sort_new_from_config(lw, attribute_names, attribute_values);
		layout_bar_sort_set(lw, bar);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
	else if (g_ascii_strcasecmp(element_name, "toolbar") == 0)
		{
		options_parse_func_push(parser_data, options_parse_toolbar, NULL, lw);
		}
	else if (g_ascii_strcasecmp(element_name, "statusbar") == 0)
		{
		options_parse_func_push(parser_data, options_parse_statusbar, NULL, lw);
		}
	else
		{
		log_printf("unexpected in <layout>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}

static void options_parse_layout_end(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, gpointer data, GError **error)
{
	LayoutWindow *lw = data;
	layout_util_sync(lw);
}

static void options_parse_toplevel(GQParserData *parser_data, GMarkupParseContext *context, const gchar *element_name, const gchar **attribute_names, const gchar **attribute_values, gpointer data, GError **error)
{
	if (g_ascii_strcasecmp(element_name, "gq") == 0)
		{
		/* optional top-level node */
		options_parse_func_push(parser_data, options_parse_toplevel, NULL, NULL);
		return;
		}
	if (g_ascii_strcasecmp(element_name, "global") == 0)
		{
		load_global_params(attribute_names, attribute_values);
		options_parse_func_push(parser_data, options_parse_global, options_parse_global_end, NULL);
		return;
		}
	
	if (g_ascii_strcasecmp(element_name, "layout") == 0)
		{
		LayoutWindow *lw;
		lw = layout_find_by_layout_id(options_get_id(attribute_names, attribute_values));
		if (lw) 
			{
			layout_update_from_config(lw, attribute_names, attribute_values);
			}
		else
			{
			lw = layout_new_from_config(attribute_names, attribute_values, parser_data->startup);
			}
		options_parse_func_push(parser_data, options_parse_layout, options_parse_layout_end, lw);
		}
	else
		{
		log_printf("unexpected in <toplevel>: <%s>\n", element_name);
		options_parse_func_push(parser_data, options_parse_leaf, NULL, NULL);
		}
}





/*
 *-----------------------------------------------------------------------------
 * parser
 *-----------------------------------------------------------------------------
 */


struct _GQParserFuncData
{
	GQParserStartFunc start_func;
	GQParserEndFunc end_func;
//	GQParserTextFunc text_func;
	gpointer data;
};

void options_parse_func_push(GQParserData *parser_data, GQParserStartFunc start_func, GQParserEndFunc end_func, gpointer data)
{
	GQParserFuncData *func_data = g_new0(GQParserFuncData, 1);
	func_data->start_func = start_func;
	func_data->end_func = end_func;
	func_data->data = data;
	
	parser_data->parse_func_stack = g_list_prepend(parser_data->parse_func_stack, func_data);
}

void options_parse_func_pop(GQParserData *parser_data)
{
	g_free(parser_data->parse_func_stack->data);
	parser_data->parse_func_stack = g_list_delete_link(parser_data->parse_func_stack, parser_data->parse_func_stack);
}

void options_parse_func_set_data(GQParserData *parser_data, gpointer data)
{
	GQParserFuncData *func = parser_data->parse_func_stack->data;
	func->data = data;
}


static void start_element(GMarkupParseContext *context,
			  const gchar *element_name,
			  const gchar **attribute_names,
			  const gchar **attribute_values,
			  gpointer user_data,
			  GError **error) 
{
	GQParserData *parser_data = user_data;
	GQParserFuncData *func = parser_data->parse_func_stack->data; 
	DEBUG_2("start %s", element_name);
	
	if (func->start_func)
		func->start_func(parser_data, context, element_name, attribute_names, attribute_values, func->data, error);
}

static void end_element(GMarkupParseContext *context,
			  const gchar *element_name,
			  gpointer user_data,
			  GError **error) 
{
	GQParserData *parser_data = user_data;
	GQParserFuncData *func = parser_data->parse_func_stack->data; 
	DEBUG_2("end %s", element_name);

	if (func->end_func)
		func->end_func(parser_data, context, element_name, func->data, error);

	options_parse_func_pop(parser_data);
}

static GMarkupParser parser = {
	start_element,
	end_element,
	NULL,
	NULL,
	NULL
};

/*
 *-----------------------------------------------------------------------------
 * load configuration (public)
 *-----------------------------------------------------------------------------
 */

gboolean load_config_from_buf(const gchar *buf, gsize size, gboolean startup)
{
	GMarkupParseContext *context;
	gboolean ret = TRUE;
	GQParserData *parser_data;

	parser_data = g_new0(GQParserData, 1);
	
	parser_data->startup = startup;
	options_parse_func_push(parser_data, options_parse_toplevel, NULL, NULL);
	
	context = g_markup_parse_context_new(&parser, 0, parser_data, NULL);

	if (g_markup_parse_context_parse(context, buf, size, NULL) == FALSE)
		{
		ret = FALSE;
		DEBUG_1("Parse failed");
		}
		
	g_free(parser_data);

	g_markup_parse_context_free(context);
	return ret;
}

gboolean load_config_from_file(const gchar *utf8_path, gboolean startup)
{
	gsize size;
	gchar *buf;
	gboolean ret = TRUE;

	if (g_file_get_contents(utf8_path, &buf, &size, NULL) == FALSE) 
		{
		return FALSE;
		}
	ret = load_config_from_buf(buf, size, startup);
	g_free(buf);
	return ret;
}
	


/* vim: set shiftwidth=8 softtabstop=0 cindent cinoptions={1s: */
