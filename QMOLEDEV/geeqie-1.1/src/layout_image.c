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

#include "main.h"
#include "layout_image.h"

#include "collect.h"
#include "color-man.h"
#include "dnd.h"
#include "editors.h"
#include "filedata.h"
#include "fullscreen.h"
#include "image.h"
#include "image-overlay.h"
#include "img-view.h"
#include "layout.h"
#include "layout_util.h"
#include "menu.h"
#include "misc.h"
#include "pixbuf_util.h"
#include "pixbuf-renderer.h"
#include "slideshow.h"
#include "ui_fileops.h"
#include "ui_menu.h"
#include "uri_utils.h"
#include "utilops.h"
#include "view_file.h"

#include <gdk/gdkkeysyms.h> /* for keyboard values */


static GtkWidget *layout_image_pop_menu(LayoutWindow *lw);
static void layout_image_set_buttons(LayoutWindow *lw);

/*
 *----------------------------------------------------------------------------
 * full screen overlay
 *----------------------------------------------------------------------------
 */

void layout_image_overlay_toggle(LayoutWindow *lw)
{
	if (!lw) return;
	image_osd_toggle(lw->image);
}

/*
 *----------------------------------------------------------------------------
 * full screen
 *----------------------------------------------------------------------------
 */

static void layout_image_full_screen_stop_func(FullScreenData *fs, gpointer data)
{
	LayoutWindow *lw = data;

	/* restore image window */
	lw->image = fs->normal_imd;

	lw->full_screen = NULL;
}

void layout_image_full_screen_start(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (lw->full_screen) return;

	lw->full_screen = fullscreen_start(lw->window, lw->image,
					   layout_image_full_screen_stop_func, lw);

	/* set to new image window */
	lw->image = lw->full_screen->imd;

	layout_image_set_buttons(lw);

	g_signal_connect(G_OBJECT(lw->full_screen->window), "key_press_event",
			 G_CALLBACK(layout_key_press_cb), lw);

	layout_actions_add_window(lw, lw->full_screen->window);
#if 0
	gtk_widget_set_sensitive(lw->window, FALSE);
	if (lw->tools) gtk_widget_set_sensitive(lw->tools, FALSE);
#endif

	image_osd_copy_status(lw->full_screen->normal_imd, lw->image);
}

void layout_image_full_screen_stop(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;
	if (!lw->full_screen) return;

	image_osd_copy_status(lw->image, lw->full_screen->normal_imd);

	fullscreen_stop(lw->full_screen);

#if 0
	gtk_widget_set_sensitive(lw->window, TRUE);
	if (lw->tools) gtk_widget_set_sensitive(lw->tools, TRUE);
#endif
}

void layout_image_full_screen_toggle(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;
	if (lw->full_screen)
		{
		layout_image_full_screen_stop(lw);
		}
	else
		{
		layout_image_full_screen_start(lw);
		}
}

gboolean layout_image_full_screen_active(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return (lw->full_screen != NULL);
}

/*
 *----------------------------------------------------------------------------
 * slideshow
 *----------------------------------------------------------------------------
 */

static void layout_image_slideshow_next(LayoutWindow *lw)
{
	if (lw->slideshow) slideshow_next(lw->slideshow);
}

static void layout_image_slideshow_prev(LayoutWindow *lw)
{
	if (lw->slideshow) slideshow_prev(lw->slideshow);
}

static void layout_image_slideshow_stop_func(SlideShowData *ss, gpointer data)
{
	LayoutWindow *lw = data;

	lw->slideshow = NULL;
	layout_status_update_info(lw, NULL);
}

void layout_image_slideshow_start(LayoutWindow *lw)
{
	CollectionData *cd;
	CollectInfo *info;

	if (!layout_valid(&lw)) return;
	if (lw->slideshow) return;

	cd = image_get_collection(lw->image, &info);

	if (cd && info)
		{
		lw->slideshow = slideshow_start_from_collection(lw, NULL, cd,
				layout_image_slideshow_stop_func, lw, info);
		}
	else
		{
		lw->slideshow = slideshow_start(lw,
				layout_list_get_index(lw, layout_image_get_fd(lw)),
				layout_image_slideshow_stop_func, lw);
		}

	layout_status_update_info(lw, NULL);
}

/* note that slideshow will take ownership of the list, do not free it */
void layout_image_slideshow_start_from_list(LayoutWindow *lw, GList *list)
{
	if (!layout_valid(&lw)) return;

	if (lw->slideshow || !list)
		{
		filelist_free(list);
		return;
		}

	lw->slideshow = slideshow_start_from_filelist(lw, NULL, list,
						       layout_image_slideshow_stop_func, lw);

	layout_status_update_info(lw, NULL);
}

void layout_image_slideshow_stop(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (!lw->slideshow) return;

	slideshow_free(lw->slideshow);
	/* the stop_func sets lw->slideshow to NULL for us */
}

void layout_image_slideshow_toggle(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (lw->slideshow)
		{
		layout_image_slideshow_stop(lw);
		}
	else
		{
		layout_image_slideshow_start(lw);
		}
}

gboolean layout_image_slideshow_active(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return (lw->slideshow != NULL);
}

gboolean layout_image_slideshow_pause_toggle(LayoutWindow *lw)
{
	gboolean ret;

	if (!layout_valid(&lw)) return FALSE;

	ret = slideshow_pause_toggle(lw->slideshow);

	layout_status_update_info(lw, NULL);

	return ret;
}

gboolean layout_image_slideshow_paused(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return (slideshow_paused(lw->slideshow));
}

static gboolean layout_image_slideshow_continue_check(LayoutWindow *lw)
{
	if (!lw->slideshow) return FALSE;

	if (!slideshow_should_continue(lw->slideshow))
		{
		layout_image_slideshow_stop(lw);
		return FALSE;
		}

	return TRUE;
}

/*
 *----------------------------------------------------------------------------
 * pop-up menus
 *----------------------------------------------------------------------------
 */

static void li_pop_menu_zoom_in_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_zoom_adjust(lw, get_zoom_increment(), FALSE);
}

static void li_pop_menu_zoom_out_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;
	layout_image_zoom_adjust(lw, -get_zoom_increment(), FALSE);
}

static void li_pop_menu_zoom_1_1_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_zoom_set(lw, 1.0, FALSE);
}

static void li_pop_menu_zoom_fit_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_zoom_set(lw, 0.0, FALSE);
}

static void li_pop_menu_edit_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw;
	const gchar *key = data;

	lw = submenu_item_get_data(widget);

	if (!editor_window_flag_set(key))
		{
		layout_image_full_screen_stop(lw);
		}
	file_util_start_editor_from_file(key, layout_image_get_fd(lw), lw->window);
}

static void li_pop_menu_wallpaper_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_to_root(lw);
}

static void li_pop_menu_alter_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;
	AlterType type;

	lw = submenu_item_get_data(widget);
	type = (AlterType)GPOINTER_TO_INT(data);

	image_alter_orientation(lw->image, type);
}

static void li_pop_menu_new_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	view_window_new(layout_image_get_fd(lw));
}

static GtkWidget *li_pop_menu_click_parent(GtkWidget *widget, LayoutWindow *lw)
{
	GtkWidget *menu;
	GtkWidget *parent;

	menu = gtk_widget_get_toplevel(widget);
	if (!menu) return NULL;

	parent = g_object_get_data(G_OBJECT(menu), "click_parent");

	if (!parent && lw->full_screen)
		{
		parent = lw->full_screen->imd->widget;
		}

	return parent;
}

static void li_pop_menu_copy_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	file_util_copy(layout_image_get_fd(lw), NULL, NULL,
		       li_pop_menu_click_parent(widget, lw));
}

static void li_pop_menu_copy_path_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	file_util_copy_path_to_clipboard(layout_image_get_fd(lw));
}

static void li_pop_menu_move_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	file_util_move(layout_image_get_fd(lw), NULL, NULL,
		       li_pop_menu_click_parent(widget, lw));
}

static void li_pop_menu_rename_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	file_util_rename(layout_image_get_fd(lw), NULL,
			 li_pop_menu_click_parent(widget, lw));
}

static void li_pop_menu_delete_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	file_util_delete(layout_image_get_fd(lw), NULL,
			 li_pop_menu_click_parent(widget, lw));
}

static void li_pop_menu_slide_start_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_slideshow_start(lw);
}

static void li_pop_menu_slide_stop_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_slideshow_stop(lw);
}

static void li_pop_menu_slide_pause_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_slideshow_pause_toggle(lw);
}

static void li_pop_menu_full_screen_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_image_full_screen_toggle(lw);
}

static void li_pop_menu_hide_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_tools_hide_toggle(lw);
}

static void li_set_layout_path_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;
	FileData *fd;

	if (!layout_valid(&lw)) return;

	fd = layout_image_get_fd(lw);
	if (fd) layout_set_fd(lw, fd);
}

static gboolean li_check_if_current_path(LayoutWindow *lw, const gchar *path)
{
	gchar *dirname;
	gboolean ret;

	if (!path || !layout_valid(&lw) || !lw->dir_fd) return FALSE;

	dirname = g_path_get_dirname(path);
	ret = (strcmp(lw->dir_fd->path, dirname) == 0);
	g_free(dirname);
	return ret;
}

static void layout_image_popup_menu_destroy_cb(GtkWidget *widget, gpointer data)
{
	GList *editmenu_fd_list = data;

	filelist_free(editmenu_fd_list);
}

static GList *layout_image_get_fd_list(LayoutWindow *lw)
{
	GList *list = NULL;
	FileData *fd = layout_image_get_fd(lw);

	if (fd)
		{
		if (lw->vf)
			/* optionally include sidecars if the filelist entry is not expanded */
			list = vf_selection_get_one(lw->vf, fd);
		else
			list = g_list_append(NULL, file_data_ref(fd));
		}
	
	return list;
}

static GtkWidget *layout_image_pop_menu(LayoutWindow *lw)
{
	GtkWidget *menu;
	GtkWidget *item;
	GtkWidget *submenu;
	const gchar *path;
	gboolean fullscreen;
	GList *editmenu_fd_list;

	path = layout_image_get_path(lw);
	fullscreen = layout_image_full_screen_active(lw);

	menu = popup_menu_short_lived();

	menu_item_add_stock(menu, _("Zoom _in"), GTK_STOCK_ZOOM_IN, G_CALLBACK(li_pop_menu_zoom_in_cb), lw);
	menu_item_add_stock(menu, _("Zoom _out"), GTK_STOCK_ZOOM_OUT, G_CALLBACK(li_pop_menu_zoom_out_cb), lw);
	menu_item_add_stock(menu, _("Zoom _1:1"), GTK_STOCK_ZOOM_100, G_CALLBACK(li_pop_menu_zoom_1_1_cb), lw);
	menu_item_add_stock(menu, _("Fit image to _window"), GTK_STOCK_ZOOM_FIT, G_CALLBACK(li_pop_menu_zoom_fit_cb), lw);
	menu_item_add_divider(menu);

	editmenu_fd_list = layout_image_get_fd_list(lw);
	g_signal_connect(G_OBJECT(menu), "destroy",
			 G_CALLBACK(layout_image_popup_menu_destroy_cb), editmenu_fd_list);
	submenu = submenu_add_edit(menu, &item, G_CALLBACK(li_pop_menu_edit_cb), lw, editmenu_fd_list);
	if (!path) gtk_widget_set_sensitive(item, FALSE);
	menu_item_add_divider(submenu);
	menu_item_add(submenu, _("Set as _wallpaper"), G_CALLBACK(li_pop_menu_wallpaper_cb), lw);

	item = submenu_add_alter(menu, G_CALLBACK(li_pop_menu_alter_cb), lw);

	item = menu_item_add_stock(menu, _("View in _new window"), GTK_STOCK_NEW, G_CALLBACK(li_pop_menu_new_cb), lw);
	if (!path || fullscreen) gtk_widget_set_sensitive(item, FALSE);

	item = menu_item_add(menu, _("_Go to directory view"), G_CALLBACK(li_set_layout_path_cb), lw);
	if (!path || li_check_if_current_path(lw, path)) gtk_widget_set_sensitive(item, FALSE);

	menu_item_add_divider(menu);

	item = menu_item_add_stock(menu, _("_Copy..."), GTK_STOCK_COPY, G_CALLBACK(li_pop_menu_copy_cb), lw);
	if (!path) gtk_widget_set_sensitive(item, FALSE);
	item = menu_item_add(menu, _("_Move..."), G_CALLBACK(li_pop_menu_move_cb), lw);
	if (!path) gtk_widget_set_sensitive(item, FALSE);
	item = menu_item_add(menu, _("_Rename..."), G_CALLBACK(li_pop_menu_rename_cb), lw);
	if (!path) gtk_widget_set_sensitive(item, FALSE);
	item = menu_item_add_stock(menu, _("_Delete..."), GTK_STOCK_DELETE, G_CALLBACK(li_pop_menu_delete_cb), lw);
	if (!path) gtk_widget_set_sensitive(item, FALSE);
	
	item = menu_item_add(menu, _("_Copy path"), G_CALLBACK(li_pop_menu_copy_path_cb), lw);
	if (!path) gtk_widget_set_sensitive(item, FALSE);

	menu_item_add_divider(menu);

	if (layout_image_slideshow_active(lw))
		{
		menu_item_add(menu, _("_Stop slideshow"), G_CALLBACK(li_pop_menu_slide_stop_cb), lw);
		if (layout_image_slideshow_paused(lw))
			{
			item = menu_item_add(menu, _("Continue slides_how"),
					     G_CALLBACK(li_pop_menu_slide_pause_cb), lw);
			}
		else
			{
			item = menu_item_add(menu, _("Pause slides_how"),
					     G_CALLBACK(li_pop_menu_slide_pause_cb), lw);
			}
		}
	else
		{
		menu_item_add(menu, _("_Start slideshow"), G_CALLBACK(li_pop_menu_slide_start_cb), lw);
		item = menu_item_add(menu, _("Pause slides_how"), G_CALLBACK(li_pop_menu_slide_pause_cb), lw);
		gtk_widget_set_sensitive(item, FALSE);
		}

	if (!fullscreen)
		{
		menu_item_add(menu, _("_Full screen"), G_CALLBACK(li_pop_menu_full_screen_cb), lw);
		}
	else
		{
		menu_item_add(menu, _("Exit _full screen"), G_CALLBACK(li_pop_menu_full_screen_cb), lw);
		}

	menu_item_add_divider(menu);

	item = menu_item_add_check(menu, _("Hide file _list"), lw->options.tools_hidden,
				   G_CALLBACK(li_pop_menu_hide_cb), lw);
	if (fullscreen) gtk_widget_set_sensitive(item, FALSE);

	return menu;
}

static void layout_image_menu_pos_cb(GtkMenu *menu, gint *x, gint *y, gboolean *push_in, gpointer data)
{
	LayoutWindow *lw = data;

	gdk_window_get_origin(lw->image->pr->window, x, y);
	popup_menu_position_clamp(menu, x, y, 0);
}

void layout_image_menu_popup(LayoutWindow *lw)
{
	GtkWidget *menu;

	menu = layout_image_pop_menu(lw);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, layout_image_menu_pos_cb, lw, 0, GDK_CURRENT_TIME);
}

/*
 *----------------------------------------------------------------------------
 * dnd
 *----------------------------------------------------------------------------
 */

static void layout_image_dnd_receive(GtkWidget *widget, GdkDragContext *context,
				     gint x, gint y,
				     GtkSelectionData *selection_data, guint info,
				     guint time, gpointer data)
{
	LayoutWindow *lw = data;
	gint i;


	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i]->pr == widget)
			break;
		}
	if (i < MAX_SPLIT_IMAGES)
		{
		DEBUG_1("dnd image activate %d", i);
		layout_image_activate(lw, i, FALSE);
		}


	if (info == TARGET_URI_LIST || info == TARGET_APP_COLLECTION_MEMBER)
		{
		CollectionData *source;
		GList *list;
		GList *info_list;

		if (info == TARGET_URI_LIST)
			{
			list = uri_filelist_from_text((gchar *)selection_data->data, TRUE);
			source = NULL;
			info_list = NULL;
			}
		else
			{
			source = collection_from_dnd_data((gchar *)selection_data->data, &list, &info_list);
			}

		if (list)
			{
			FileData *fd = list->data;

			if (isfile(fd->path))
				{
				gchar *base;
				gint row;
				FileData *dir_fd;

				base = remove_level_from_path(fd->path);
				dir_fd = file_data_new_dir(base);
				if (dir_fd != lw->dir_fd)
					{
					layout_set_fd(lw, dir_fd);
					}
				file_data_unref(dir_fd);
				g_free(base);

				row = layout_list_get_index(lw, fd);
				if (source && info_list)
					{
					layout_image_set_collection(lw, source, info_list->data);
					}
				else if (row == -1)
					{
					layout_image_set_fd(lw, fd);
					}
				else
					{
					layout_image_set_index(lw, row);
					}
				}
			else if (isdir(fd->path))
				{
				layout_set_fd(lw, fd);
				layout_image_set_fd(lw, NULL);
				}
			}

		filelist_free(list);
		g_list_free(info_list);
		}
}

static void layout_image_dnd_get(GtkWidget *widget, GdkDragContext *context,
				 GtkSelectionData *selection_data, guint info,
				 guint time, gpointer data)
{
	LayoutWindow *lw = data;
	FileData *fd;
	gint i;


	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i]->pr == widget)
			break;
		}
	if (i < MAX_SPLIT_IMAGES)
		{
		DEBUG_1("dnd get from %d", i);
		fd = image_get_fd(lw->split_images[i]);
		}
	else
		fd = layout_image_get_fd(lw);

	if (fd)
		{
		gchar *text = NULL;
		gint len;
		gboolean plain_text;
		GList *list;

		switch (info)
			{
			case TARGET_URI_LIST:
				plain_text = FALSE;
				break;
			case TARGET_TEXT_PLAIN:
			default:
				plain_text = TRUE;
				break;
			}
		list = g_list_append(NULL, fd);
		text = uri_text_from_filelist(list, &len, plain_text);
		g_list_free(list);
		if (text)
			{
			gtk_selection_data_set(selection_data, selection_data->target,
					       8, (guchar *)text, len);
			g_free(text);
			}
		}
	else
		{
		gtk_selection_data_set(selection_data, selection_data->target,
				       8, NULL, 0);
		}
}

static void layout_image_dnd_end(GtkWidget *widget, GdkDragContext *context, gpointer data)
{
	LayoutWindow *lw = data;
	if (context->action == GDK_ACTION_MOVE)
		{
		FileData *fd;
		gint row;

		fd = layout_image_get_fd(lw);
		row = layout_list_get_index(lw, fd);
		if (row < 0) return;

		if (!isfile(fd->path))
			{
			if ((guint) row < layout_list_count(lw, NULL) - 1)
				{
				layout_image_next(lw);
				}
			else
				{
				layout_image_prev(lw);
				}
			}
		layout_refresh(lw);
		}
}

static void layout_image_dnd_init(LayoutWindow *lw, gint i)
{
	ImageWindow *imd = lw->split_images[i];

	gtk_drag_source_set(imd->pr, GDK_BUTTON2_MASK,
			    dnd_file_drag_types, dnd_file_drag_types_count,
			    GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
	g_signal_connect(G_OBJECT(imd->pr), "drag_data_get",
			 G_CALLBACK(layout_image_dnd_get), lw);
	g_signal_connect(G_OBJECT(imd->pr), "drag_end",
			 G_CALLBACK(layout_image_dnd_end), lw);

	gtk_drag_dest_set(imd->pr,
			  GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_DROP,
			  dnd_file_drop_types, dnd_file_drop_types_count,
			  GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
	g_signal_connect(G_OBJECT(imd->pr), "drag_data_received",
			 G_CALLBACK(layout_image_dnd_receive), lw);
}


/*
 *----------------------------------------------------------------------------
 * misc
 *----------------------------------------------------------------------------
 */

void layout_image_to_root(LayoutWindow *lw)
{
	image_to_root_window(lw->image, (image_zoom_get(lw->image) == 0));
}

/*
 *----------------------------------------------------------------------------
 * manipulation + accessors
 *----------------------------------------------------------------------------
 */

void layout_image_scroll(LayoutWindow *lw, gint x, gint y, gboolean connect_scroll)
{
	gdouble dx, dy;
	gint width, height, i;
	if (!layout_valid(&lw)) return;

	image_scroll(lw->image, x, y);

	if (!connect_scroll) return;

	image_get_image_size(lw->image, &width, &height);
	dx = (gdouble) x / width;
	dy = (gdouble) y / height;
	
	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i] != lw->image)
			{
			gdouble sx, sy;
			image_get_scroll_center(lw->split_images[i], &sx, &sy);
			sx += dx;
			sy += dy;
			image_set_scroll_center(lw->split_images[i], sx, sy);
			}
		}

}

void layout_image_zoom_adjust(LayoutWindow *lw, gdouble increment, gboolean connect_zoom)
{
	gint i;
	if (!layout_valid(&lw)) return;

	image_zoom_adjust(lw->image, increment);

	if (!connect_zoom) return;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i] != lw->image)
			image_zoom_adjust(lw->split_images[i], increment); ;
		}
}

void layout_image_zoom_adjust_at_point(LayoutWindow *lw, gdouble increment, gint x, gint y, gboolean connect_zoom)
{
	gint i;
	if (!layout_valid(&lw)) return;

	image_zoom_adjust_at_point(lw->image, increment, x, y);

	if (!connect_zoom) return;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i] != lw->image)
			image_zoom_adjust_at_point(lw->split_images[i], increment, x, y);
		}
}

void layout_image_zoom_set(LayoutWindow *lw, gdouble zoom, gboolean connect_zoom)
{
	gint i;
	if (!layout_valid(&lw)) return;

	image_zoom_set(lw->image, zoom);

	if (!connect_zoom) return;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i] != lw->image)
			image_zoom_set(lw->split_images[i], zoom);
		}
}

void layout_image_zoom_set_fill_geometry(LayoutWindow *lw, gboolean vertical, gboolean connect_zoom)
{
	gint i;
	if (!layout_valid(&lw)) return;

	image_zoom_set_fill_geometry(lw->image, vertical);

	if (!connect_zoom) return;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i] != lw->image)
			image_zoom_set_fill_geometry(lw->split_images[i], vertical);
		}
}

void layout_image_alter_orientation(LayoutWindow *lw, AlterType type)
{
	if (!layout_valid(&lw)) return;

	image_alter_orientation(lw->image, type);
}

void layout_image_set_desaturate(LayoutWindow *lw, gboolean desaturate)
{
	if (!layout_valid(&lw)) return;

	image_set_desaturate(lw->image, desaturate);
}

gboolean layout_image_get_desaturate(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return image_get_desaturate(lw->image);
}

/* stereo */
/*
gint layout_image_stereo_get(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return 0;

	return image_stereo_get(lw->image);
}

void layout_image_stereo_set(LayoutWindow *lw, gint stereo_mode)
{
	if (!layout_valid(&lw)) return;

	image_stereo_set(lw->image, stereo_mode);
}
void layout_image_stereo_swap(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	image_stereo_swap(lw->image);
}
*/

gint layout_image_stereo_pixbuf_get(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return 0;

	return image_stereo_pixbuf_get(lw->image);
}

void layout_image_stereo_pixbuf_set(LayoutWindow *lw, gint stereo_mode)
{
	if (!layout_valid(&lw)) return;

	image_stereo_pixbuf_set(lw->image, stereo_mode);
}

const gchar *layout_image_get_path(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;

	return image_get_path(lw->image);
}

const gchar *layout_image_get_name(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;

	return image_get_name(lw->image);
}

FileData *layout_image_get_fd(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;

	return image_get_fd(lw->image);
}

CollectionData *layout_image_get_collection(LayoutWindow *lw, CollectInfo **info)
{
	if (!layout_valid(&lw)) return NULL;

	return image_get_collection(lw->image, info);
}

gint layout_image_get_index(LayoutWindow *lw)
{
	return layout_list_get_index(lw, image_get_fd(lw->image));
}

/*
 *----------------------------------------------------------------------------
 * image changers
 *----------------------------------------------------------------------------
 */

void layout_image_set_fd(LayoutWindow *lw, FileData *fd)
{
	if (!layout_valid(&lw)) return;

	image_change_fd(lw->image, fd, image_zoom_get_default(lw->image));

	layout_list_sync_fd(lw, fd);
	layout_image_slideshow_continue_check(lw);
	layout_bars_new_image(lw);
}

void layout_image_set_with_ahead(LayoutWindow *lw, FileData *fd, FileData *read_ahead_fd)
{
	if (!layout_valid(&lw)) return;

/*
This should be handled at the caller: in vflist_select_image
	if (path)
		{
		const gchar *old_path;

		old_path = layout_image_get_path(lw);
		if (old_path && strcmp(path, old_path) == 0) return;
		}
*/
	layout_image_set_fd(lw, fd);
	if (options->image.enable_read_ahead) image_prebuffer_set(lw->image, read_ahead_fd);
}

void layout_image_set_index(LayoutWindow *lw, gint index)
{
	FileData *fd;
	FileData *read_ahead_fd;
	gint old;

	if (!layout_valid(&lw)) return;

	old = layout_list_get_index(lw, layout_image_get_fd(lw));
	fd = layout_list_get_fd(lw, index);

	if (old > index)
		{
		read_ahead_fd = layout_list_get_fd(lw, index - 1);
		}
	else
		{
		read_ahead_fd = layout_list_get_fd(lw, index + 1);
		}

	if (layout_selection_count(lw, 0) > 1)
		{
		GList *x = layout_selection_list_by_index(lw);
		GList *y;
		GList *last;

		for (last = y = x; y; y = y->next)
			last = y;
		for (y = x; y && (GPOINTER_TO_INT(y->data)) != index; y = y->next)
			;

		if (y)
			{
			gint newindex;

			if ((index > old && (index != GPOINTER_TO_INT(last->data) || old != GPOINTER_TO_INT(x->data)))
			    || (old == GPOINTER_TO_INT(last->data) && index == GPOINTER_TO_INT(x->data)))
				{
				if (y->next)
					newindex = GPOINTER_TO_INT(y->next->data);
				else
					newindex = GPOINTER_TO_INT(x->data);
				}
			else
				{
				if (y->prev)
					newindex = GPOINTER_TO_INT(y->prev->data);
				else
					newindex = GPOINTER_TO_INT(last->data);
				}

			read_ahead_fd = layout_list_get_fd(lw, newindex);
			}

		while (x)
			x = g_list_remove(x, x->data);
		}

	layout_image_set_with_ahead(lw, fd, read_ahead_fd);
}

static void layout_image_set_collection_real(LayoutWindow *lw, CollectionData *cd, CollectInfo *info, gboolean forward)
{
	if (!layout_valid(&lw)) return;

	image_change_from_collection(lw->image, cd, info, image_zoom_get_default(lw->image));
	if (options->image.enable_read_ahead)
		{
		CollectInfo *r_info;
		if (forward)
			{
			r_info = collection_next_by_info(cd, info);
			if (!r_info) r_info = collection_prev_by_info(cd, info);
			}
		else
			{
			r_info = collection_prev_by_info(cd, info);
			if (!r_info) r_info = collection_next_by_info(cd, info);
			}
		if (r_info) image_prebuffer_set(lw->image, r_info->fd);
		}

	layout_image_slideshow_continue_check(lw);
	layout_bars_new_image(lw);
}

void layout_image_set_collection(LayoutWindow *lw, CollectionData *cd, CollectInfo *info)
{
	layout_image_set_collection_real(lw, cd, info, TRUE);
	layout_list_sync_fd(lw, layout_image_get_fd(lw));
}

void layout_image_refresh(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	image_reload(lw->image);
}

void layout_image_color_profile_set(LayoutWindow *lw,
				    gint input_type,
				    gboolean use_image)
{
	if (!layout_valid(&lw)) return;

	image_color_profile_set(lw->image, input_type, use_image);
}

gboolean layout_image_color_profile_get(LayoutWindow *lw,
				    	gint *input_type,
				    	gboolean *use_image)
{
	if (!layout_valid(&lw)) return FALSE;

	return image_color_profile_get(lw->image, input_type, use_image);
}

void layout_image_color_profile_set_use(LayoutWindow *lw, gboolean enable)
{
	if (!layout_valid(&lw)) return;

	image_color_profile_set_use(lw->image, enable);
}

gboolean layout_image_color_profile_get_use(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return image_color_profile_get_use(lw->image);
}

gboolean layout_image_color_profile_get_status(LayoutWindow *lw, gchar **image_profile, gchar **screen_profile)
{
	if (!layout_valid(&lw)) return FALSE;

	return image_color_profile_get_status(lw->image, image_profile, screen_profile);
}

/*
 *----------------------------------------------------------------------------
 * list walkers
 *----------------------------------------------------------------------------
 */

void layout_image_next(LayoutWindow *lw)
{
	gint current;
	CollectionData *cd;
	CollectInfo *info;

	if (!layout_valid(&lw)) return;

	if (layout_image_slideshow_active(lw))
		{
		layout_image_slideshow_next(lw);
		return;
		}

	if (layout_selection_count(lw, 0) > 1)
		{
		GList *x = layout_selection_list_by_index(lw);
		gint old = layout_list_get_index(lw, layout_image_get_fd(lw));
		GList *y;

		for (y = x; y && (GPOINTER_TO_INT(y->data)) != old; y = y->next)
			;
		if (y)
			{
			if (y->next)
				layout_image_set_index(lw, GPOINTER_TO_INT(y->next->data));
			else
				layout_image_set_index(lw, GPOINTER_TO_INT(x->data));
			}
		while (x)
			x = g_list_remove(x, x->data);
		if (y) /* not dereferenced */
			return;
		}

	cd = image_get_collection(lw->image, &info);

	if (cd && info)
		{
		info = collection_next_by_info(cd, info);
		if (info)
			{
			layout_image_set_collection_real(lw, cd, info, TRUE);
			}
		else
			{
			image_osd_icon(lw->image, IMAGE_OSD_LAST, -1);
			}
		return;
		}

	current = layout_image_get_index(lw);

	if (current >= 0)
		{
		if ((guint) current < layout_list_count(lw, NULL) - 1)
			{
			layout_image_set_index(lw, current + 1);
			}
		else
			{
			image_osd_icon(lw->image, IMAGE_OSD_LAST, -1);
			}
		}
	else
		{
		layout_image_set_index(lw, 0);
		}
}

void layout_image_prev(LayoutWindow *lw)
{
	gint current;
	CollectionData *cd;
	CollectInfo *info;

	if (!layout_valid(&lw)) return;

	if (layout_image_slideshow_active(lw))
		{
		layout_image_slideshow_prev(lw);
		return;
		}

	if (layout_selection_count(lw, 0) > 1)
		{
		GList *x = layout_selection_list_by_index(lw);
		gint old = layout_list_get_index(lw, layout_image_get_fd(lw));
		GList *y;
		GList *last;

		for (last = y = x; y; y = y->next)
			last = y;
		for (y = x; y && (GPOINTER_TO_INT(y->data)) != old; y = y->next)
			;
		if (y)
			{
			if (y->prev)
				layout_image_set_index(lw, GPOINTER_TO_INT(y->prev->data));
			else
				layout_image_set_index(lw, GPOINTER_TO_INT(last->data));
			}
		while (x)
			x = g_list_remove(x, x->data);
		if (y) /* not dereferenced */
			return;
		}

	cd = image_get_collection(lw->image, &info);

	if (cd && info)
		{
		info = collection_prev_by_info(cd, info);
		if (info)
			{
			layout_image_set_collection_real(lw, cd, info, FALSE);
			}
		else
			{
			image_osd_icon(lw->image, IMAGE_OSD_FIRST, -1);
			}
		return;
		}

	current = layout_image_get_index(lw);

	if (current >= 0)
		{
		if (current > 0)
			{
			layout_image_set_index(lw, current - 1);
			}
		else
			{
			image_osd_icon(lw->image, IMAGE_OSD_FIRST, -1);
			}
		}
	else
		{
		layout_image_set_index(lw, layout_list_count(lw, NULL) - 1);
		}
}

void layout_image_first(LayoutWindow *lw)
{
	gint current;
	CollectionData *cd;
	CollectInfo *info;

	if (!layout_valid(&lw)) return;

	cd = image_get_collection(lw->image, &info);

	if (cd && info)
		{
		CollectInfo *new;
		new = collection_get_first(cd);
		if (new != info) layout_image_set_collection_real(lw, cd, new, TRUE);
		return;
		}

	current = layout_image_get_index(lw);
	if (current != 0 && layout_list_count(lw, NULL) > 0)
		{
		layout_image_set_index(lw, 0);
		}
}

void layout_image_last(LayoutWindow *lw)
{
	gint current;
	gint count;
	CollectionData *cd;
	CollectInfo *info;

	if (!layout_valid(&lw)) return;

	cd = image_get_collection(lw->image, &info);

	if (cd && info)
		{
		CollectInfo *new;
		new = collection_get_last(cd);
		if (new != info) layout_image_set_collection_real(lw, cd, new, FALSE);
		return;
		}

	current = layout_image_get_index(lw);
	count = layout_list_count(lw, NULL);
	if (current != count - 1 && count > 0)
		{
		layout_image_set_index(lw, count - 1);
		}
}

/*
 *----------------------------------------------------------------------------
 * mouse callbacks
 *----------------------------------------------------------------------------
 */

static gint image_idx(LayoutWindow *lw, ImageWindow *imd)
{
	gint i;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] == imd)
			break;
		}
	if (i < MAX_SPLIT_IMAGES)
		{
		return i;
		}
	return -1;
}

static void layout_image_focus_in_cb(ImageWindow *imd, gpointer data)
{
	LayoutWindow *lw = data;

	gint i = image_idx(lw, imd);

	if (i != -1)
		{
		DEBUG_1("image activate focus_in %d", i);
		layout_image_activate(lw, i, FALSE);
		}
}


static void layout_image_button_cb(ImageWindow *imd, GdkEventButton *event, gpointer data)
{
	LayoutWindow *lw = data;
	GtkWidget *menu;

	switch (event->button)
		{
		case MOUSE_BUTTON_LEFT:
			if (lw->split_mode == SPLIT_NONE)
				layout_image_next(lw);
			break;
		case MOUSE_BUTTON_MIDDLE:
			if (lw->split_mode == SPLIT_NONE)
				layout_image_prev(lw);
			break;
		case MOUSE_BUTTON_RIGHT:
			menu = layout_image_pop_menu(lw);
			if (imd == lw->image)
				{
				g_object_set_data(G_OBJECT(menu), "click_parent", imd->widget);
				}
			gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, 3, event->time);
			break;
		default:
			break;
		}
}

static void layout_image_scroll_cb(ImageWindow *imd, GdkEventScroll *event, gpointer data)
{
	LayoutWindow *lw = data;

	gint i = image_idx(lw, imd);

	if (i != -1)
		{
		DEBUG_1("image activate scroll %d", i);
		layout_image_activate(lw, i, FALSE);
		}


	if (event->state & GDK_CONTROL_MASK)
		{
		switch (event->direction)
			{
			case GDK_SCROLL_UP:
				layout_image_zoom_adjust_at_point(lw, get_zoom_increment(), event->x, event->y, event->state & GDK_SHIFT_MASK);
				break;
			case GDK_SCROLL_DOWN:
				layout_image_zoom_adjust_at_point(lw, -get_zoom_increment(), event->x, event->y, event->state & GDK_SHIFT_MASK);
				break;
			default:
				break;
			}
		}
	else if (options->mousewheel_scrolls)
		{
		switch (event->direction)
			{
			case GDK_SCROLL_UP:
				image_scroll(imd, 0, -MOUSEWHEEL_SCROLL_SIZE);
				break;
			case GDK_SCROLL_DOWN:
				image_scroll(imd, 0, MOUSEWHEEL_SCROLL_SIZE);
				break;
			case GDK_SCROLL_LEFT:
				image_scroll(imd, -MOUSEWHEEL_SCROLL_SIZE, 0);
				break;
			case GDK_SCROLL_RIGHT:
				image_scroll(imd, MOUSEWHEEL_SCROLL_SIZE, 0);
				break;
			default:
				break;
			}
		}
	else
		{
		switch (event->direction)
			{
			case GDK_SCROLL_UP:
				layout_image_prev(lw);
				break;
			case GDK_SCROLL_DOWN:
				layout_image_next(lw);
				break;
			default:
				break;
			}
		}
}

static void layout_image_drag_cb(ImageWindow *imd, GdkEventButton *event, gdouble dx, gdouble dy, gpointer data)
{
	gint i;
	LayoutWindow *lw = data;

	if (!(event->state & GDK_SHIFT_MASK)) return;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i] && lw->split_images[i] != imd)
			{
			gdouble sx, sy;

			if (event->state & GDK_CONTROL_MASK)
				{
				image_get_scroll_center(imd, &sx, &sy);
				}
			else
				{
				image_get_scroll_center(lw->split_images[i], &sx, &sy);
				sx += dx;
				sy += dy;
				}
			image_set_scroll_center(lw->split_images[i], sx, sy);
			}
		}
}

static void layout_image_button_inactive_cb(ImageWindow *imd, GdkEventButton *event, gpointer data)
{
	LayoutWindow *lw = data;
	GtkWidget *menu;
	gint i = image_idx(lw, imd);

	if (i != -1)
		{
		layout_image_activate(lw, i, FALSE);
		}

	switch (event->button)
		{
		case MOUSE_BUTTON_RIGHT:
			menu = layout_image_pop_menu(lw);
			if (imd == lw->image)
				{
				g_object_set_data(G_OBJECT(menu), "click_parent", imd->widget);
				}
			gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, 3, event->time);
			break;
		default:
			break;
		}

}

static void layout_image_drag_inactive_cb(ImageWindow *imd, GdkEventButton *event, gdouble dx, gdouble dy, gpointer data)
{
	LayoutWindow *lw = data;
	gint i = image_idx(lw, imd);

	if (i != -1)
		{
		layout_image_activate(lw, i, FALSE);
		}

	/* continue as with active image */
	layout_image_drag_cb(imd, event, dx, dy, data);
}


static void layout_image_set_buttons(LayoutWindow *lw)
{
	image_set_button_func(lw->image, layout_image_button_cb, lw);
	image_set_scroll_func(lw->image, layout_image_scroll_cb, lw);
}

static void layout_image_set_buttons_inactive(LayoutWindow *lw, gint i)
{
	image_set_button_func(lw->split_images[i], layout_image_button_inactive_cb, lw);
	image_set_scroll_func(lw->split_images[i], layout_image_scroll_cb, lw);
}

/* Returns the length of an integer */
static gint num_length(gint num)
{
	gint len = 0;
	if (num < 0) num = -num;
	while (num)
		{
		num /= 10;
		len++;
		}
	return len;
}

void layout_status_update_pixel_cb(PixbufRenderer *pr, gpointer data)
{
	LayoutWindow *lw = data;
	gint x_pixel, y_pixel;
	gint width, height;
	gchar *text;
	PangoAttrList *attrs;

	if (!data || !layout_valid(&lw) || !lw->image
	    || !lw->options.show_info_pixel || lw->image->unknown) return;

	pixbuf_renderer_get_image_size(pr, &width, &height);
	if (width < 1 || height < 1) return;

	pixbuf_renderer_get_mouse_position(pr, &x_pixel, &y_pixel);
	
	if(x_pixel >= 0 && y_pixel >= 0)
		{
		gint r_mouse, g_mouse, b_mouse;
			
		pixbuf_renderer_get_pixel_colors(pr, x_pixel, y_pixel,
						 &r_mouse, &g_mouse, &b_mouse);			
		
		text = g_strdup_printf(_("[%*d,%*d]: RGB(%3d,%3d,%3d)"),
					 num_length(width - 1), x_pixel,
					 num_length(height - 1), y_pixel,
					 r_mouse, g_mouse, b_mouse);
		
		}
	else
		{
		text = g_strdup_printf(_("[%*s,%*s]: RGB(---,---,---)"),
					 num_length(width - 1), " ",
					 num_length(height - 1), " ");
		}

	attrs = pango_attr_list_new();
	pango_attr_list_insert(attrs, pango_attr_family_new("Monospace"));
	gtk_label_set_text(GTK_LABEL(lw->info_pixel), text);
	gtk_label_set_attributes(GTK_LABEL(lw->info_pixel), attrs);
	pango_attr_list_unref(attrs);
	g_free(text);
}


/*
 *----------------------------------------------------------------------------
 * setup
 *----------------------------------------------------------------------------
 */

static void layout_image_update_cb(ImageWindow *imd, gpointer data)
{
	LayoutWindow *lw = data;
	layout_status_update_image(lw);
}

GtkWidget *layout_image_new(LayoutWindow *lw, gint i)
{
	if (!lw->split_image_sizegroup) lw->split_image_sizegroup = gtk_size_group_new(GTK_SIZE_GROUP_BOTH);

	if (!lw->split_images[i])
		{
		lw->split_images[i] = image_new(TRUE);

#if GTK_CHECK_VERSION(2,12,0)
		g_object_ref(lw->split_images[i]->widget);
#else
		gtk_widget_ref(lw->split_images[i]->widget);
#endif

		g_signal_connect(G_OBJECT(lw->split_images[i]->pr), "update-pixel",
				 G_CALLBACK(layout_status_update_pixel_cb), lw);

		image_background_set_color_from_options(lw->split_images[i], FALSE);

		image_auto_refresh_enable(lw->split_images[i], TRUE);

		layout_image_dnd_init(lw, i);
		image_color_profile_set(lw->split_images[i],
					options->color_profile.input_type,
					options->color_profile.use_image);
		image_color_profile_set_use(lw->split_images[i], options->color_profile.enabled);

		gtk_size_group_add_widget(lw->split_image_sizegroup, lw->split_images[i]->widget);
		gtk_widget_set_size_request(lw->split_images[i]->widget, IMAGE_MIN_WIDTH, -1);

		image_set_focus_in_func(lw->split_images[i], layout_image_focus_in_cb, lw);

		}

	return lw->split_images[i]->widget;
}

void layout_image_deactivate(LayoutWindow *lw, gint i)
{
	if (!lw->split_images[i]) return;
	image_set_update_func(lw->split_images[i], NULL, NULL);
	layout_image_set_buttons_inactive(lw, i);
	image_set_drag_func(lw->split_images[i], layout_image_drag_inactive_cb, lw);

	image_attach_window(lw->split_images[i], NULL, NULL, NULL, FALSE);
	image_select(lw->split_images[i], FALSE);
}

/* force should be set after change of lw->split_mode */
void layout_image_activate(LayoutWindow *lw, gint i, gboolean force)
{
	FileData *fd;

	if (!lw->split_images[i]) return;
	if (!force && lw->active_split_image == i) return;

	/* deactivate currently active */
	if (lw->active_split_image != i)
		layout_image_deactivate(lw, lw->active_split_image);

	lw->image = lw->split_images[i];
	lw->active_split_image = i;

	image_set_update_func(lw->image, layout_image_update_cb, lw);
	layout_image_set_buttons(lw);
	image_set_drag_func(lw->image, layout_image_drag_cb, lw);

	image_attach_window(lw->image, lw->window, NULL, GQ_APPNAME, FALSE);

	/* do not hilight selected image in SPLIT_NONE */
	/* maybe the image should be selected always and hilight should be controled by
	   another image option */
	if (lw->split_mode != SPLIT_NONE)
		image_select(lw->split_images[i], TRUE);
	else
		image_select(lw->split_images[i], FALSE);

	fd = image_get_fd(lw->image);

	if (fd)
		{
//		layout_list_sync_path(lw, path);
		layout_set_fd(lw, fd);
		}
	layout_status_update_image(lw);
}


static void layout_image_setup_split_common(LayoutWindow *lw, gint n)
{
	gboolean frame = (n > 1) || (!lw->options.tools_float && !lw->options.tools_hidden);
	gint i;

	for (i = 0; i < n; i++)
		if (!lw->split_images[i])
			{
			FileData *img_fd = NULL;
			double zoom = 0.0;

			layout_image_new(lw, i);
			image_set_frame(lw->split_images[i], frame);
			image_set_selectable(lw->split_images[i], (n > 1));
			
			if (lw->image)
				{
				image_osd_copy_status(lw->image, lw->split_images[i]);
				}

			if (layout_selection_count(lw, 0) > 1)
				{
				GList *work = g_list_last(layout_selection_list(lw));
				gint j = 0;
				
				if (work) work = work->prev;

				while (work && j < i)
					{
					FileData *fd = work->data;
					work = work->prev;
					
					j++;
					if (!fd || !*fd->path) continue;
					img_fd = fd;
					}
				}

			if (!img_fd && lw->image)
				{
				img_fd = image_get_fd(lw->image);
				zoom = image_zoom_get(lw->image);
				}

			if (img_fd)
				{
				gdouble sx, sy;
				image_change_fd(lw->split_images[i], img_fd, zoom);
				image_get_scroll_center(lw->image, &sx, &sy);
				image_set_scroll_center(lw->split_images[i], sx, sy);
				}
			layout_image_deactivate(lw, i);
			}
		else
			{
			image_set_frame(lw->split_images[i], frame);
			image_set_selectable(lw->split_images[i], (n > 1));
			}

	for (i = n; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i])
			{
#if GTK_CHECK_VERSION(2,12,0)
			g_object_unref(lw->split_images[i]->widget);
#else
			gtk_widget_unref(lw->split_images[i]->widget);
#endif
			lw->split_images[i] = NULL;
			}
		}
	
	if (!lw->image || lw->active_split_image < 0 || lw->active_split_image >= n)
		{
		layout_image_activate(lw, 0, TRUE);
		}
	else
		{
		/* this will draw the frame around selected image (image_select)
		   on switch from single to split images */
		layout_image_activate(lw, lw->active_split_image, TRUE);
		}
}

GtkWidget *layout_image_setup_split_none(LayoutWindow *lw)
{
	lw->split_mode = SPLIT_NONE;
	
	layout_image_setup_split_common(lw, 1);

	lw->split_image_widget = lw->split_images[0]->widget;

	return lw->split_image_widget;
}


GtkWidget *layout_image_setup_split_hv(LayoutWindow *lw, gboolean horizontal)
{
	GtkWidget *paned;
	
	lw->split_mode = horizontal ? SPLIT_HOR : SPLIT_VERT;

	layout_image_setup_split_common(lw, 2);

	/* horizontal split means vpaned and vice versa */
	if (horizontal)
		paned = gtk_vpaned_new();
	else
		paned = gtk_hpaned_new();

	gtk_paned_pack1(GTK_PANED(paned), lw->split_images[0]->widget, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(paned), lw->split_images[1]->widget, TRUE, TRUE);

	gtk_widget_show(lw->split_images[0]->widget);
	gtk_widget_show(lw->split_images[1]->widget);

	lw->split_image_widget = paned;

	return lw->split_image_widget;

}

GtkWidget *layout_image_setup_split_quad(LayoutWindow *lw)
{
	GtkWidget *hpaned;
	GtkWidget *vpaned1;
	GtkWidget *vpaned2;
	gint i;

	lw->split_mode = SPLIT_QUAD;

	layout_image_setup_split_common(lw, 4);

	hpaned = gtk_hpaned_new();
	vpaned1 = gtk_vpaned_new();
	vpaned2 = gtk_vpaned_new();

	gtk_paned_pack1(GTK_PANED(vpaned1), lw->split_images[0]->widget, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(vpaned1), lw->split_images[2]->widget, TRUE, TRUE);

	gtk_paned_pack1(GTK_PANED(vpaned2), lw->split_images[1]->widget, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(vpaned2), lw->split_images[3]->widget, TRUE, TRUE);

	gtk_paned_pack1(GTK_PANED(hpaned), vpaned1, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(hpaned), vpaned2, TRUE, TRUE);

	for (i = 0; i < 4; i++)
		gtk_widget_show(lw->split_images[i]->widget);

	gtk_widget_show(vpaned1);
	gtk_widget_show(vpaned2);

	lw->split_image_widget = hpaned;

	return lw->split_image_widget;

}

GtkWidget *layout_image_setup_split(LayoutWindow *lw, ImageSplitMode mode)
{
	switch (mode)
		{
		case SPLIT_HOR:
			return layout_image_setup_split_hv(lw, TRUE);
		case SPLIT_VERT:
			return layout_image_setup_split_hv(lw, FALSE);
		case SPLIT_QUAD:
			return layout_image_setup_split_quad(lw);
		case SPLIT_NONE:
		default:
			return layout_image_setup_split_none(lw);
		}
}


/*
 *-----------------------------------------------------------------------------
 * maintenance (for rename, move, remove)
 *-----------------------------------------------------------------------------
 */

static void layout_image_maint_renamed(LayoutWindow *lw, FileData *fd)
{
	if (fd == layout_image_get_fd(lw))
		{
		image_set_fd(lw->image, fd);
		}
}

static void layout_image_maint_removed(LayoutWindow *lw, FileData *fd)
{
	if (fd == layout_image_get_fd(lw))
		{
		CollectionData *cd;
		CollectInfo *info;

		cd = image_get_collection(lw->image, &info);
		if (cd && info)
			{
			CollectInfo *new;

			new = collection_next_by_info(cd, info);
			if (!new) new = collection_prev_by_info(cd, info);

			if (new)
				{
				layout_image_set_collection(lw, cd, new);
				return;
				}
			layout_image_set_fd(lw, NULL);
			}
			
		/* the image will be set to the next image from the list soon,  
		   setting it to NULL here is not necessary*/
		}
}


void layout_image_notify_cb(FileData *fd, NotifyType type, gpointer data)
{
	LayoutWindow *lw = data;

	if (!(type & NOTIFY_CHANGE) || !fd->change) return;

	DEBUG_1("Notify layout_image: %s %04x", fd->path, type);
	
	switch (fd->change->type)
		{
		case FILEDATA_CHANGE_MOVE:
		case FILEDATA_CHANGE_RENAME:
			layout_image_maint_renamed(lw, fd);
			break;
		case FILEDATA_CHANGE_DELETE:
			layout_image_maint_removed(lw, fd);
			break;
		case FILEDATA_CHANGE_COPY:
		case FILEDATA_CHANGE_UNSPECIFIED:
		case FILEDATA_CHANGE_WRITE_METADATA:
			break;
		}

}
/* vim: set shiftwidth=8 softtabstop=0 cindent cinoptions={1s: */
