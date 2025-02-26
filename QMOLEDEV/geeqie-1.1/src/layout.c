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
#include "layout.h"

#include "color-man.h"
#include "filedata.h"
#include "histogram.h"
#include "image.h"
#include "image-overlay.h"
#include "layout_config.h"
#include "layout_image.h"
#include "layout_util.h"
#include "menu.h"
#include "pixbuf-renderer.h"
#include "pixbuf_util.h"
#include "utilops.h"
#include "view_dir.h"
#include "view_file.h"
#include "ui_fileops.h"
#include "ui_menu.h"
#include "ui_misc.h"
#include "ui_tabcomp.h"
#include "window.h"
#include "metadata.h"
#include "rcfile.h"
#include "bar.h"
#include "bar_sort.h"
#include "preferences.h"

#ifdef HAVE_LIRC
#include "lirc.h"
#endif

#define MAINWINDOW_DEF_WIDTH 700
#define MAINWINDOW_DEF_HEIGHT 500

#define MAIN_WINDOW_DIV_HPOS (MAINWINDOW_DEF_WIDTH / 2)
#define MAIN_WINDOW_DIV_VPOS (MAINWINDOW_DEF_HEIGHT / 2)

#define TOOLWINDOW_DEF_WIDTH 260
#define TOOLWINDOW_DEF_HEIGHT 450

#define PROGRESS_WIDTH 150
#define ZOOM_LABEL_WIDTH 64

#define PANE_DIVIDER_SIZE 10


GList *layout_window_list = NULL;
LayoutWindow *current_lw = NULL;

static void layout_list_scroll_to_subpart(LayoutWindow *lw, const gchar *needle);


/*
 *-----------------------------------------------------------------------------
 * misc
 *-----------------------------------------------------------------------------
 */

gboolean layout_valid(LayoutWindow **lw)
{
	if (*lw == NULL)
		{
		if (current_lw) *lw = current_lw;
		else if (layout_window_list) *lw = layout_window_list->data;
		return (*lw != NULL);
		}
	return (g_list_find(layout_window_list, *lw) != NULL);
}

LayoutWindow *layout_find_by_image(ImageWindow *imd)
{
	GList *work;

	work = layout_window_list;
	while (work)
		{
		LayoutWindow *lw = work->data;
		work = work->next;

		if (lw->image == imd) return lw;
		}

	return NULL;
}

LayoutWindow *layout_find_by_image_fd(ImageWindow *imd)
{
	GList *work;

	work = layout_window_list;
	while (work)
		{
		LayoutWindow *lw = work->data;
		work = work->next;

		if (lw->image->image_fd == imd->image_fd)
			return lw;
		}

	return NULL;
}

LayoutWindow *layout_find_by_layout_id(const gchar *id)
{
	GList *work;

	if (!id || !id[0]) return NULL;
	
	if (strcmp(id, LAYOUT_ID_CURRENT) == 0)
		{
		if (current_lw) return current_lw;
		if (layout_window_list) return layout_window_list->data;
		return NULL;
		}

	work = layout_window_list;
	while (work)
		{
		LayoutWindow *lw = work->data;
		work = work->next;

		if (lw->options.id && strcmp(id, lw->options.id) == 0)
			return lw;
		}

	return NULL;
}

static void layout_set_unique_id(LayoutWindow *lw)
{
	char id[10];
	gint i;
	if (lw->options.id && lw->options.id[0]) return; /* id is already set */
	
	g_free(lw->options.id);
	lw->options.id = NULL;
	
	if (!layout_find_by_layout_id("main"))
		{
		lw->options.id = g_strdup("main");
		return;
		}
	
	i = 1;
	while (TRUE)
		{
		g_snprintf(id, sizeof(id), "lw%d", i);
		if (!layout_find_by_layout_id(id))
			{
			lw->options.id = g_strdup(id);
			return;
			}
		i++;
		}
}

static gboolean layout_set_current_cb(GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	LayoutWindow *lw = data;
	current_lw = lw;
	return FALSE;
}

/*
 *-----------------------------------------------------------------------------
 * menu, toolbar, and dir view
 *-----------------------------------------------------------------------------
 */

static void layout_path_entry_changed_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;
	gchar *buf;

	if (gtk_combo_box_get_active(GTK_COMBO_BOX(widget)) < 0) return;

	buf = g_strdup(gtk_entry_get_text(GTK_ENTRY(lw->path_entry)));
	if (!lw->dir_fd || strcmp(buf, lw->dir_fd->path) != 0)
		{
		layout_set_path(lw, buf);
		}

	g_free(buf);
}

static void layout_path_entry_tab_cb(const gchar *path, gpointer data)
{
	LayoutWindow *lw = data;
	gchar *buf;

	buf = g_strdup(path);
	parse_out_relatives(buf);
	
	if (isdir(buf))
		{
		if ((!lw->dir_fd || strcmp(lw->dir_fd->path, buf) != 0) && layout_set_path(lw, buf))
			{
			gint pos = -1;
			/* put the G_DIR_SEPARATOR back, if we are in tab completion for a dir and result was path change */
			gtk_editable_insert_text(GTK_EDITABLE(lw->path_entry), G_DIR_SEPARATOR_S, -1, &pos);
			gtk_editable_set_position(GTK_EDITABLE(lw->path_entry),
						  strlen(gtk_entry_get_text(GTK_ENTRY(lw->path_entry))));
			}
		}
	else if (lw->dir_fd)
		{
		gchar *base = remove_level_from_path(buf);

		if (strcmp(lw->dir_fd->path, base) == 0)
			{
			layout_list_scroll_to_subpart(lw, filename_from_path(buf));
			}
		g_free(base);
		}
		
	g_free(buf);
}

static void layout_path_entry_cb(const gchar *path, gpointer data)
{
	LayoutWindow *lw = data;
	gchar *buf;

	buf = g_strdup(path);
	parse_out_relatives(buf);

	layout_set_path(lw, buf);

	g_free(buf);
}

static void layout_vd_select_cb(ViewDir *vd, FileData *fd, gpointer data)
{
	LayoutWindow *lw = data;

	layout_set_fd(lw, fd);
}

static void layout_path_entry_tab_append_cb(const gchar *path, gpointer data, gint n)
{
	LayoutWindow *lw = data;

	if (!lw || !lw->back_button) return;
	if (!layout_valid(&lw)) return;

	/* Enable back button if it makes sense */
	gtk_widget_set_sensitive(lw->back_button, (n > 1));
}

static GtkWidget *layout_tool_setup(LayoutWindow *lw)
{
	GtkWidget *box;
	GtkWidget *menu_bar;
	GtkWidget *tabcomp;
	GtkWidget *toolbar;

	box = gtk_vbox_new(FALSE, 0);

	menu_bar = layout_actions_menu_bar(lw);
	gtk_box_pack_start(GTK_BOX(box), menu_bar, FALSE, FALSE, 0);
	gtk_widget_show(menu_bar);

	toolbar = layout_actions_toolbar(lw, TOOLBAR_MAIN);
	gtk_box_pack_start(GTK_BOX(box), toolbar, FALSE, FALSE, 0);
	if (!lw->options.toolbar_hidden) gtk_widget_show(toolbar);

	tabcomp = tab_completion_new_with_history(&lw->path_entry, NULL, "path_list", -1,
						  layout_path_entry_cb, lw);
	tab_completion_add_tab_func(lw->path_entry, layout_path_entry_tab_cb, lw);
	tab_completion_add_append_func(lw->path_entry, layout_path_entry_tab_append_cb, lw);
	gtk_box_pack_start(GTK_BOX(box), tabcomp, FALSE, FALSE, 0);
	gtk_widget_show(tabcomp);

	g_signal_connect(G_OBJECT(lw->path_entry->parent), "changed",
			 G_CALLBACK(layout_path_entry_changed_cb), lw);

	lw->vd = vd_new(lw->options.dir_view_type, lw->dir_fd);
	vd_set_layout(lw->vd, lw);
	vd_set_select_func(lw->vd, layout_vd_select_cb, lw);

	lw->dir_view = lw->vd->widget;

	gtk_box_pack_start(GTK_BOX(box), lw->dir_view, TRUE, TRUE, 0);
	gtk_widget_show(lw->dir_view);

	gtk_widget_show(box);

	return box;
}

/*
 *-----------------------------------------------------------------------------
 * sort button (and menu)
 *-----------------------------------------------------------------------------
 */

static void layout_sort_menu_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw;
	SortType type;

	if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) return;

	lw = submenu_item_get_data(widget);
	if (!lw) return;

	type = (SortType)GPOINTER_TO_INT(data);

	layout_sort_set(lw, type, lw->sort_ascend);
}

static void layout_sort_menu_ascend_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;

	layout_sort_set(lw, lw->sort_method, !lw->sort_ascend);
}

static void layout_sort_menu_hide_cb(GtkWidget *widget, gpointer data)
{
	/* destroy the menu */
#if GTK_CHECK_VERSION(2,12,0)
	g_object_unref(widget);
#else
	gtk_widget_unref(GTK_WIDGET(widget));
#endif
}

static void layout_sort_button_press_cb(GtkWidget *widget, gpointer data)
{
	LayoutWindow *lw = data;
	GtkWidget *menu;
	GdkEvent *event;
	guint32 etime;

	menu = submenu_add_sort(NULL, G_CALLBACK(layout_sort_menu_cb), lw, FALSE, FALSE, TRUE, lw->sort_method);

	/* take ownership of menu */
#ifdef GTK_OBJECT_FLOATING
	/* GTK+ < 2.10 */
	g_object_ref(G_OBJECT(menu));
	gtk_object_sink(GTK_OBJECT(menu));
#else
	/* GTK+ >= 2.10 */
	g_object_ref_sink(G_OBJECT(menu));
#endif

	/* ascending option */
	menu_item_add_divider(menu);
	menu_item_add_check(menu, _("Ascending"), lw->sort_ascend, G_CALLBACK(layout_sort_menu_ascend_cb), lw);

	g_signal_connect(G_OBJECT(menu), "selection_done",
			 G_CALLBACK(layout_sort_menu_hide_cb), NULL);

	event = gtk_get_current_event();
	if (event)
		{
		etime = gdk_event_get_time(event);
		gdk_event_free(event);
		}
	else
		{
		etime = 0;
		}

	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, 0, etime);
}

static GtkWidget *layout_sort_button(LayoutWindow *lw)
{
	GtkWidget *button;

	button = gtk_button_new_with_label(sort_type_get_text(lw->sort_method));
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(layout_sort_button_press_cb), lw);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);

	return button;
}

/*
 *-----------------------------------------------------------------------------
 * status bar
 *-----------------------------------------------------------------------------
 */


void layout_status_update_progress(LayoutWindow *lw, gdouble val, const gchar *text)
{
	if (!layout_valid(&lw)) return;
	if (!lw->info_progress_bar) return;

	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(lw->info_progress_bar), val);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(lw->info_progress_bar), (text) ? text : " ");
}

void layout_status_update_info(LayoutWindow *lw, const gchar *text)
{
	gchar *buf = NULL;

	if (!layout_valid(&lw)) return;

	if (!text)
		{
		guint n;
		gint64 n_bytes = 0;
	
		n = layout_list_count(lw, &n_bytes);
		
		if (n)
			{
			guint s;
			gint64 s_bytes = 0;
			const gchar *ss;

			if (layout_image_slideshow_active(lw))
				{
				if (!layout_image_slideshow_paused(lw))
					{
					ss = _(" Slideshow");
					}
				else
					{
					ss = _(" Paused");
					}
				}
			else
				{
				ss = "";
				}
	
			s = layout_selection_count(lw, &s_bytes);
	
			layout_bars_new_selection(lw, s);
	
			if (s > 0)
				{
				gchar *b = text_from_size_abrev(n_bytes);
				gchar *sb = text_from_size_abrev(s_bytes);
				buf = g_strdup_printf(_("%s, %d files (%s, %d)%s"), b, n, sb, s, ss);
				g_free(b);
				g_free(sb);
				}
			else if (n > 0)
				{
				gchar *b = text_from_size_abrev(n_bytes);
				buf = g_strdup_printf(_("%s, %d files%s"), b, n, ss);
				g_free(b);
				}
			else
				{
				buf = g_strdup_printf(_("%d files%s"), n, ss);
				}
	
			text = buf;
	
			image_osd_update(lw->image);
			}
		else
			{
			text = "";
			}
	}
	
	if (lw->info_status) gtk_label_set_text(GTK_LABEL(lw->info_status), text);
	g_free(buf);
}

void layout_status_update_image(LayoutWindow *lw)
{
	guint64 n;
	
	if (!layout_valid(&lw) || !lw->image) return;
	if (!lw->info_zoom || !lw->info_details) return; /*called from layout_style_set */

	n = layout_list_count(lw, NULL);
	
	if (!n)
		{
		gtk_label_set_text(GTK_LABEL(lw->info_zoom), "");
		gtk_label_set_text(GTK_LABEL(lw->info_details), "");
		}
	else
		{
		gchar *text;
		gchar *b;

		text = image_zoom_get_as_text(lw->image);
		gtk_label_set_text(GTK_LABEL(lw->info_zoom), text);
		g_free(text);

		b = image_get_fd(lw->image) ? text_from_size(image_get_fd(lw->image)->size) : g_strdup("0");

		if (lw->image->unknown)
			{
			if (image_get_path(lw->image) && !access_file(image_get_path(lw->image), R_OK))
				{
				text = g_strdup_printf(_("(no read permission) %s bytes"), b);
				}
			else
				{
				text = g_strdup_printf(_("( ? x ? ) %s bytes"), b);
				}
			}
		else
			{
			gint width, height;
	
			image_get_image_size(lw->image, &width, &height);
			text = g_strdup_printf(_("( %d x %d ) %s bytes"),
					       width, height, b);
			}

		g_signal_emit_by_name (lw->image->pr, "update-pixel");
		
		g_free(b);
		
		gtk_label_set_text(GTK_LABEL(lw->info_details), text);
		g_free(text);
		}
	layout_util_sync_color(lw); /* update color button */
}

void layout_status_update_all(LayoutWindow *lw)
{
	layout_status_update_progress(lw, 0.0, NULL);
	layout_status_update_info(lw, NULL);
	layout_status_update_image(lw);
	layout_util_status_update_write(lw);
}

static GtkWidget *layout_status_label(gchar *text, GtkWidget *box, gboolean start, gint size, gboolean expand)
{
	GtkWidget *label;
	GtkWidget *frame;

	frame = gtk_frame_new(NULL);
	if (size) gtk_widget_set_size_request(frame, size, -1);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
	if (start)
		{
		gtk_box_pack_start(GTK_BOX(box), frame, expand, expand, 0);
		}
	else
		{
		gtk_box_pack_end(GTK_BOX(box), frame, expand, expand, 0);
		}
	gtk_widget_show(frame);

	label = gtk_label_new(text ? text : "");
	gtk_container_add(GTK_CONTAINER(frame), label);
	gtk_widget_show(label);

	return label;
}

static void layout_status_setup(LayoutWindow *lw, GtkWidget *box, gboolean small_format)
{
	GtkWidget *hbox;
	GtkWidget *toolbar;
	GtkWidget *toolbar_frame;

	if (lw->info_box) return;

	if (small_format)
		{
		lw->info_box = gtk_vbox_new(FALSE, 0);
		}
	else
		{
		lw->info_box = gtk_hbox_new(FALSE, 0);
		}
	gtk_box_pack_end(GTK_BOX(box), lw->info_box, FALSE, FALSE, 0);
	gtk_widget_show(lw->info_box);

	if (small_format)
		{
		hbox = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(lw->info_box), hbox, FALSE, FALSE, 0);
		gtk_widget_show(hbox);
		}
	else
		{
		hbox = lw->info_box;
		}
	lw->info_progress_bar = gtk_progress_bar_new();
	gtk_widget_set_size_request(lw->info_progress_bar, PROGRESS_WIDTH, -1);
	gtk_box_pack_start(GTK_BOX(hbox), lw->info_progress_bar, FALSE, FALSE, 0);
	gtk_widget_show(lw->info_progress_bar);

	lw->info_sort = layout_sort_button(lw);
	gtk_box_pack_start(GTK_BOX(hbox), lw->info_sort, FALSE, FALSE, 0);
	gtk_widget_show(lw->info_sort);

	lw->info_status = layout_status_label(NULL, lw->info_box, TRUE, 0, (!small_format));

	if (small_format)
		{
		hbox = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(lw->info_box), hbox, FALSE, FALSE, 0);
		gtk_widget_show(hbox);
		}
	lw->info_details = layout_status_label(NULL, hbox, TRUE, 0, TRUE);
	toolbar = layout_actions_toolbar(lw, TOOLBAR_STATUS);
	
	toolbar_frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(toolbar_frame), GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(toolbar_frame), toolbar);
	gtk_widget_show(toolbar_frame);
	gtk_widget_show(toolbar);
	gtk_box_pack_end(GTK_BOX(hbox), toolbar_frame, FALSE, FALSE, 0);
	lw->info_zoom = layout_status_label(NULL, hbox, FALSE, ZOOM_LABEL_WIDTH, FALSE);
	if (small_format)
		{
		hbox = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(lw->info_box), hbox, FALSE, FALSE, 0);
		gtk_widget_show(hbox);
		}
	lw->info_pixel = layout_status_label(NULL, hbox, FALSE, 0, small_format); /* expand only in small format */
	if (!lw->options.show_info_pixel) gtk_widget_hide(gtk_widget_get_parent(lw->info_pixel));
}

/*
 *-----------------------------------------------------------------------------
 * views
 *-----------------------------------------------------------------------------
 */

static GtkWidget *layout_tools_new(LayoutWindow *lw)
{
	lw->dir_view = layout_tool_setup(lw);
	return lw->dir_view;
}

static void layout_list_status_cb(ViewFile *vf, gpointer data)
{
	LayoutWindow *lw = data;

	layout_status_update_info(lw, NULL);
}

static void layout_list_thumb_cb(ViewFile *vf, gdouble val, const gchar *text, gpointer data)
{
	LayoutWindow *lw = data;

	layout_status_update_progress(lw, val, text);
}

static void layout_list_sync_thumb(LayoutWindow *lw)
{
	if (lw->vf) vf_thumb_set(lw->vf, lw->options.show_thumbnails);
}

static GtkWidget *layout_list_new(LayoutWindow *lw)
{
	lw->vf = vf_new(lw->options.file_view_type, NULL);
	vf_set_layout(lw->vf, lw);

	vf_set_status_func(lw->vf, layout_list_status_cb, lw);
	vf_set_thumb_status_func(lw->vf, layout_list_thumb_cb, lw);

	vf_marks_set(lw->vf, lw->options.show_marks);
	
	layout_list_sync_thumb(lw);

	return lw->vf->widget;
}

static void layout_list_sync_marks(LayoutWindow *lw)
{
	if (lw->vf) vf_marks_set(lw->vf, lw->options.show_marks);
}

static void layout_list_scroll_to_subpart(LayoutWindow *lw, const gchar *needle)
{
	if (!lw) return;
#if 0
	if (lw->vf) vf_scroll_to_subpart(lw->vf, needle);
#endif
}

GList *layout_list(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;

	if (lw->vf) return vf_get_list(lw->vf);

	return NULL;
}

guint layout_list_count(LayoutWindow *lw, gint64 *bytes)
{
	if (!layout_valid(&lw)) return 0;

	if (lw->vf) return vf_count(lw->vf, bytes);

	return 0;
}

FileData *layout_list_get_fd(LayoutWindow *lw, gint index)
{
	if (!layout_valid(&lw)) return NULL;

	if (lw->vf) return vf_index_get_data(lw->vf, index);

	return NULL;
}

gint layout_list_get_index(LayoutWindow *lw, FileData *fd)
{
	if (!layout_valid(&lw) || !fd) return -1;

	if (lw->vf) return vf_index_by_fd(lw->vf, fd);

	return -1;
}

void layout_list_sync_fd(LayoutWindow *lw, FileData *fd)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_select_by_fd(lw->vf, fd);
}

static void layout_list_sync_sort(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_sort_set(lw->vf, lw->sort_method, lw->sort_ascend);
}

GList *layout_selection_list(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;

	if (layout_image_get_collection(lw, NULL))
		{
		FileData *fd;

		fd = layout_image_get_fd(lw);
		if (fd) return g_list_append(NULL, file_data_ref(fd));
		return NULL;
		}

	if (lw->vf) return vf_selection_get_list(lw->vf);

	return NULL;
}

GList *layout_selection_list_by_index(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;

	if (lw->vf) return vf_selection_get_list_by_index(lw->vf);

	return NULL;
}

guint layout_selection_count(LayoutWindow *lw, gint64 *bytes)
{
	if (!layout_valid(&lw)) return 0;

	if (lw->vf) return vf_selection_count(lw->vf, bytes);

	return 0;
}

void layout_select_all(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_select_all(lw->vf);
}

void layout_select_none(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_select_none(lw->vf);
}

void layout_select_invert(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_select_invert(lw->vf);
}

void layout_mark_to_selection(LayoutWindow *lw, gint mark, MarkToSelectionMode mode)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_mark_to_selection(lw->vf, mark, mode);
}

void layout_selection_to_mark(LayoutWindow *lw, gint mark, SelectionToMarkMode mode)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_selection_to_mark(lw->vf, mark, mode);

	layout_status_update_info(lw, NULL); /* osd in fullscreen mode */
}

void layout_mark_filter_toggle(LayoutWindow *lw, gint mark)
{
	if (!layout_valid(&lw)) return;

	if (lw->vf) vf_mark_filter_toggle(lw->vf, mark);
}


/*
 *-----------------------------------------------------------------------------
 * access
 *-----------------------------------------------------------------------------
 */

const gchar *layout_get_path(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return NULL;
	return lw->dir_fd ? lw->dir_fd->path : NULL;
}

static void layout_sync_path(LayoutWindow *lw)
{
	if (!lw->dir_fd) return;

	if (lw->path_entry) gtk_entry_set_text(GTK_ENTRY(lw->path_entry), lw->dir_fd->path);

	if (lw->vd) vd_set_fd(lw->vd, lw->dir_fd);
	if (lw->vf) vf_set_fd(lw->vf, lw->dir_fd);
}

gboolean layout_set_path(LayoutWindow *lw, const gchar *path)
{
	FileData *fd;
	gboolean ret;

	if (!path) return FALSE;
	
	fd = file_data_new_group(path);
	ret = layout_set_fd(lw, fd);
	file_data_unref(fd);
	return ret;
}


gboolean layout_set_fd(LayoutWindow *lw, FileData *fd)
{
	gboolean have_file = FALSE;
	gboolean dir_changed = TRUE;

	if (!layout_valid(&lw)) return FALSE;

	if (!fd || !isname(fd->path)) return FALSE;
	if (lw->dir_fd && fd == lw->dir_fd)
		{
		return TRUE;
		}

	if (isdir(fd->path))
		{
		if (lw->dir_fd)
			{
			file_data_unregister_real_time_monitor(lw->dir_fd);
			file_data_unref(lw->dir_fd);
			}
		lw->dir_fd = file_data_ref(fd);
		file_data_register_real_time_monitor(fd);
		}
	else
		{
		gchar *base;

		base = remove_level_from_path(fd->path);
		if (lw->dir_fd && strcmp(lw->dir_fd->path, base) == 0)
			{
			g_free(base);
			dir_changed = FALSE;
			}
		else if (isdir(base))
			{
			if (lw->dir_fd)
				{
				file_data_unregister_real_time_monitor(lw->dir_fd);
				file_data_unref(lw->dir_fd);
				}
			lw->dir_fd = file_data_new_dir(base);
			file_data_register_real_time_monitor(lw->dir_fd);
			g_free(base);
			}
		else
			{
			g_free(base);
			return FALSE;
			}
		if (isfile(fd->path)) have_file = TRUE;
		}

	if (lw->path_entry) tab_completion_append_to_history(lw->path_entry, lw->dir_fd->path);
	layout_sync_path(lw);
	layout_list_sync_sort(lw);
	
	if (have_file)
		{
		gint row;

		row = layout_list_get_index(lw, fd);
		if (row >= 0)
			{
			layout_image_set_index(lw, row);
			}
		else
			{
			layout_image_set_fd(lw, fd);
			}
		}
	else if (!options->lazy_image_sync)
		{
		layout_image_set_index(lw, 0);
		}

	if (options->metadata.confirm_on_dir_change && dir_changed)
		metadata_write_queue_confirm(FALSE, NULL, NULL);

	return TRUE;
}

static void layout_refresh_lists(LayoutWindow *lw)
{
	if (lw->vd) vd_refresh(lw->vd);

	if (lw->vf)
		{
		vf_refresh(lw->vf);
		vf_thumb_update(lw->vf);
		}
}

void layout_refresh(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;

	DEBUG_1("layout refresh");

	layout_refresh_lists(lw);

	if (lw->image) layout_image_refresh(lw);
}

void layout_thumb_set(LayoutWindow *lw, gboolean enable)
{
	if (!layout_valid(&lw)) return;

	if (lw->options.show_thumbnails == enable) return;

	lw->options.show_thumbnails = enable;

	layout_util_sync_thumb(lw);
	layout_list_sync_thumb(lw);
}

void layout_marks_set(LayoutWindow *lw, gboolean enable)
{
	if (!layout_valid(&lw)) return;

	if (lw->options.show_marks == enable) return;

	lw->options.show_marks = enable;

//	layout_util_sync_marks(lw);
	layout_list_sync_marks(lw);
}

gboolean layout_thumb_get(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return lw->options.show_thumbnails;
}

gboolean layout_marks_get(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return FALSE;

	return lw->options.show_marks;
}

void layout_sort_set(LayoutWindow *lw, SortType type, gboolean ascend)
{
	if (!layout_valid(&lw)) return;
	if (lw->sort_method == type && lw->sort_ascend == ascend) return;

	lw->sort_method = type;
	lw->sort_ascend = ascend;

	if (lw->info_sort) gtk_label_set_text(GTK_LABEL(GTK_BIN(lw->info_sort)->child),
					      sort_type_get_text(type));
	layout_list_sync_sort(lw);
}

gboolean layout_sort_get(LayoutWindow *lw, SortType *type, gboolean *ascend)
{
	if (!layout_valid(&lw)) return FALSE;

	if (type) *type = lw->sort_method;
	if (ascend) *ascend = lw->sort_ascend;

	return TRUE;
}

gboolean layout_geometry_get(LayoutWindow *lw, gint *x, gint *y, gint *w, gint *h)
{
	if (!layout_valid(&lw)) return FALSE;

	gdk_window_get_root_origin(lw->window->window, x, y);
	gdk_drawable_get_size(lw->window->window, w, h);

	return TRUE;
}

gboolean layout_geometry_get_dividers(LayoutWindow *lw, gint *h, gint *v)
{
	if (!layout_valid(&lw)) return FALSE;

	if (lw->h_pane && GTK_PANED(lw->h_pane)->child1->allocation.x >= 0)
		{
		*h = GTK_PANED(lw->h_pane)->child1->allocation.width;
		}
	else if (h != &lw->options.main_window.hdivider_pos)
		{
		*h = lw->options.main_window.hdivider_pos;
		}

	if (lw->v_pane && GTK_PANED(lw->v_pane)->child1->allocation.x >= 0)
		{
		*v = GTK_PANED(lw->v_pane)->child1->allocation.height;
		}
	else if (v != &lw->options.main_window.vdivider_pos)
		{
		*v = lw->options.main_window.vdivider_pos;
		}

	return TRUE;
}

void layout_views_set(LayoutWindow *lw, DirViewType dir_view_type, FileViewType file_view_type)
{
	if (!layout_valid(&lw)) return;

	if (lw->options.dir_view_type == dir_view_type && lw->options.file_view_type == file_view_type) return;

	lw->options.dir_view_type = dir_view_type;
	lw->options.file_view_type = file_view_type;

	layout_style_set(lw, -1, NULL);
}

gboolean layout_views_get(LayoutWindow *lw, DirViewType *dir_view_type, FileViewType *file_view_type)
{
	if (!layout_valid(&lw)) return FALSE;

	*dir_view_type = lw->options.dir_view_type;
	*file_view_type = lw->options.file_view_type;

	return TRUE;
}

/*
 *-----------------------------------------------------------------------------
 * location utils
 *-----------------------------------------------------------------------------
 */

static gboolean layout_location_single(LayoutLocation l)
{
	return (l == LAYOUT_LEFT ||
		l == LAYOUT_RIGHT ||
		l == LAYOUT_TOP ||
		l == LAYOUT_BOTTOM);
}

static gboolean layout_location_vertical(LayoutLocation l)
{
	return (l & LAYOUT_TOP ||
		l & LAYOUT_BOTTOM);
}

static gboolean layout_location_first(LayoutLocation l)
{
	return (l & LAYOUT_TOP ||
		l & LAYOUT_LEFT);
}

static LayoutLocation layout_grid_compass(LayoutWindow *lw)
{
	if (layout_location_single(lw->dir_location)) return lw->dir_location;
	if (layout_location_single(lw->file_location)) return lw->file_location;
	return lw->image_location;
}

static void layout_location_compute(LayoutLocation l1, LayoutLocation l2,
				    GtkWidget *s1, GtkWidget *s2,
				    GtkWidget **d1, GtkWidget **d2)
{
	LayoutLocation l;

	l = l1 & l2;	/* get common compass direction */
	l = l1 - l;	/* remove it */

	if (layout_location_first(l))
		{
		*d1 = s1;
		*d2 = s2;
		}
	else
		{
		*d1 = s2;
		*d2 = s1;
		}
}

/*
 *-----------------------------------------------------------------------------
 * tools window (for floating/hidden)
 *-----------------------------------------------------------------------------
 */

gboolean layout_geometry_get_tools(LayoutWindow *lw, gint *x, gint *y, gint *w, gint *h, gint *divider_pos)
{
	if (!layout_valid(&lw)) return FALSE;

#if GTK_CHECK_VERSION(2,20,0)
	if (!lw->tools || !gtk_widget_get_visible(lw->tools))
#else
	if (!lw->tools || !GTK_WIDGET_VISIBLE(lw->tools))
#endif
		{
		/* use the stored values (sort of breaks success return value) */

		*divider_pos = lw->options.float_window.vdivider_pos;

		return FALSE;
		}

	gdk_window_get_root_origin(lw->tools->window, x, y);
	gdk_drawable_get_size(lw->tools->window, w, h);

	if (GTK_IS_VPANED(lw->tools_pane))
		{
		*divider_pos = GTK_PANED(lw->tools_pane)->child1->allocation.height;
		}
	else
		{
		*divider_pos = GTK_PANED(lw->tools_pane)->child1->allocation.width;
		}

	return TRUE;
}

static void layout_tools_geometry_sync(LayoutWindow *lw)
{
	layout_geometry_get_tools(lw, &lw->options.float_window.x, &lw->options.float_window.x,
				  &lw->options.float_window.w, &lw->options.float_window.h, &lw->options.float_window.vdivider_pos);
}

static void layout_tools_hide(LayoutWindow *lw, gboolean hide)
{
	if (!lw->tools) return;

	if (hide)
		{
#if GTK_CHECK_VERSION(2,20,0)
		if (gtk_widget_get_visible(lw->tools))
#else
		if (GTK_WIDGET_VISIBLE(lw->tools))
#endif
			{
			layout_tools_geometry_sync(lw);
			gtk_widget_hide(lw->tools);
			}
		}
	else
		{
#if GTK_CHECK_VERSION(2,20,0)
		if (!gtk_widget_get_visible(lw->tools))
#else
		if (!GTK_WIDGET_VISIBLE(lw->tools))
#endif
			{
			gtk_widget_show(lw->tools);
			if (lw->vf) vf_refresh(lw->vf);
			}
		}

	lw->options.tools_hidden = hide;
}

static gboolean layout_tools_delete_cb(GtkWidget *widget, GdkEventAny *event, gpointer data)
{
	LayoutWindow *lw = data;

	layout_tools_float_toggle(lw);

	return TRUE;
}

static void layout_tools_setup(LayoutWindow *lw, GtkWidget *tools, GtkWidget *files)
{
	GtkWidget *vbox;
	GtkWidget *w1, *w2;
	gboolean vertical;
	gboolean new_window = FALSE;

	vertical = (layout_location_single(lw->image_location) && !layout_location_vertical(lw->image_location)) ||
		   (!layout_location_single(lw->image_location) && layout_location_vertical(layout_grid_compass(lw)));
#if 0
	layout_location_compute(lw->dir_location, lw->file_location,
				tools, files, &w1, &w2);
#endif
	/* for now, tools/dir are always first in order */
	w1 = tools;
	w2 = files;

	if (!lw->tools)
		{
		GdkGeometry geometry;
		GdkWindowHints hints;

		lw->tools = window_new(GTK_WINDOW_TOPLEVEL, "tools", PIXBUF_INLINE_ICON_TOOLS, NULL, _("Tools"));
		g_signal_connect(G_OBJECT(lw->tools), "delete_event",
				 G_CALLBACK(layout_tools_delete_cb), lw);
		layout_keyboard_init(lw, lw->tools);

		if (options->save_window_positions)
			{
			hints = GDK_HINT_USER_POS;
			}
		else
			{
			hints = 0;
			}

		geometry.min_width = DEFAULT_MINIMAL_WINDOW_SIZE;
		geometry.min_height = DEFAULT_MINIMAL_WINDOW_SIZE;
		geometry.base_width = TOOLWINDOW_DEF_WIDTH;
		geometry.base_height = TOOLWINDOW_DEF_HEIGHT;
		gtk_window_set_geometry_hints(GTK_WINDOW(lw->tools), NULL, &geometry,
					      GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE | hints);


		gtk_window_set_resizable(GTK_WINDOW(lw->tools), TRUE);
		gtk_container_set_border_width(GTK_CONTAINER(lw->tools), 0);

		new_window = TRUE;
		}
	else
		{
		layout_tools_geometry_sync(lw);
		/* dump the contents */
		gtk_widget_destroy(GTK_BIN(lw->tools)->child);
		}

	layout_actions_add_window(lw, lw->tools);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(lw->tools), vbox);
	gtk_widget_show(vbox);

	layout_status_setup(lw, vbox, TRUE);

	if (vertical)
		{
		lw->tools_pane = gtk_vpaned_new();
		}
	else
		{
		lw->tools_pane = gtk_hpaned_new();
		}
	gtk_box_pack_start(GTK_BOX(vbox), lw->tools_pane, TRUE, TRUE, 0);
	gtk_widget_show(lw->tools_pane);

	gtk_paned_pack1(GTK_PANED(lw->tools_pane), w1, FALSE, TRUE);
	gtk_paned_pack2(GTK_PANED(lw->tools_pane), w2, TRUE, TRUE);

	gtk_widget_show(tools);
	gtk_widget_show(files);

	if (new_window)
		{
		if (options->save_window_positions)
			{
			gtk_window_set_default_size(GTK_WINDOW(lw->tools), lw->options.float_window.w, lw->options.float_window.h);
			gtk_window_move(GTK_WINDOW(lw->tools), lw->options.float_window.x, lw->options.float_window.y);
			}
		else
			{
			if (vertical)
				{
				gtk_window_set_default_size(GTK_WINDOW(lw->tools),
							    TOOLWINDOW_DEF_WIDTH, TOOLWINDOW_DEF_HEIGHT);
				}
			else
				{
				gtk_window_set_default_size(GTK_WINDOW(lw->tools),
							    TOOLWINDOW_DEF_HEIGHT, TOOLWINDOW_DEF_WIDTH);
				}
			}
		}

	if (!options->save_window_positions)
		{
		if (vertical)
			{
			lw->options.float_window.vdivider_pos = MAIN_WINDOW_DIV_VPOS;
			}
		else
			{
			lw->options.float_window.vdivider_pos = MAIN_WINDOW_DIV_HPOS;
			}
		}

	gtk_paned_set_position(GTK_PANED(lw->tools_pane), lw->options.float_window.vdivider_pos);
}

/*
 *-----------------------------------------------------------------------------
 * glue (layout arrangement)
 *-----------------------------------------------------------------------------
 */

static void layout_grid_compute(LayoutWindow *lw,
				GtkWidget *image, GtkWidget *tools, GtkWidget *files,
				GtkWidget **w1, GtkWidget **w2, GtkWidget **w3)
{
	/* heh, this was fun */

	if (layout_location_single(lw->dir_location))
		{
		if (layout_location_first(lw->dir_location))
			{
			*w1 = tools;
			layout_location_compute(lw->file_location, lw->image_location, files, image, w2, w3);
			}
		else
			{
			*w3 = tools;
			layout_location_compute(lw->file_location, lw->image_location, files, image, w1, w2);
			}
		}
	else if (layout_location_single(lw->file_location))
		{
		if (layout_location_first(lw->file_location))
			{
			*w1 = files;
			layout_location_compute(lw->dir_location, lw->image_location, tools, image, w2, w3);
			}
		else
			{
			*w3 = files;
			layout_location_compute(lw->dir_location, lw->image_location, tools, image, w1, w2);
			}
		}
	else
		{
		/* image */
		if (layout_location_first(lw->image_location))
			{
			*w1 = image;
			layout_location_compute(lw->file_location, lw->dir_location, files, tools, w2, w3);
			}
		else
			{
			*w3 = image;
			layout_location_compute(lw->file_location, lw->dir_location, files, tools, w1, w2);
			}
		}
}

void layout_split_change(LayoutWindow *lw, ImageSplitMode mode)
{
	GtkWidget *image;
	gint i;

	for (i = 0; i < MAX_SPLIT_IMAGES; i++)
		{
		if (lw->split_images[i])
			{
			gtk_widget_hide(lw->split_images[i]->widget);
			if (lw->split_images[i]->widget->parent != lw->utility_paned)
				gtk_container_remove(GTK_CONTAINER(lw->split_images[i]->widget->parent), lw->split_images[i]->widget);
			}
		}
	gtk_container_remove(GTK_CONTAINER(lw->utility_paned), lw->split_image_widget);

	image = layout_image_setup_split(lw, mode);

	gtk_paned_pack1(GTK_PANED(lw->utility_paned), image, TRUE, FALSE);
	gtk_widget_show(image);
	layout_util_sync(lw);
}

static void layout_grid_setup(LayoutWindow *lw)
{
	gint priority_location;
	GtkWidget *h;
	GtkWidget *v;
	GtkWidget *w1, *w2, *w3;

	GtkWidget *image_sb; /* image together with sidebars in utility box */
	GtkWidget *tools;
	GtkWidget *files;

	layout_actions_setup(lw);

	lw->group_box = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(lw->main_box), lw->group_box, TRUE, TRUE, 0);
	gtk_widget_show(lw->group_box);

	priority_location = layout_grid_compass(lw);

	if (lw->utility_box)
		{
		layout_split_change(lw, lw->split_mode); /* this re-creates image frame for the new configuration */
		image_sb = lw->utility_box;
		}
	else
		{
		GtkWidget *image; /* image or split images together */
		image = layout_image_setup_split(lw, lw->split_mode);
		image_sb = layout_bars_prepare(lw, image);
		}
	
	tools = layout_tools_new(lw);
	files = layout_list_new(lw);


	if (lw->options.tools_float || lw->options.tools_hidden)
		{
		gtk_box_pack_start(GTK_BOX(lw->group_box), image_sb, TRUE, TRUE, 0);
		gtk_widget_show(image_sb);

		layout_tools_setup(lw, tools, files);

		image_grab_focus(lw->image);

		return;
		}
	else if (lw->tools)
		{
		layout_tools_geometry_sync(lw);
		gtk_widget_destroy(lw->tools);
		lw->tools = NULL;
		lw->tools_pane = NULL;
		}

	layout_status_setup(lw, lw->group_box, FALSE);

	layout_grid_compute(lw, image_sb, tools, files, &w1, &w2, &w3);

	v = lw->v_pane = gtk_vpaned_new();

	h = lw->h_pane = gtk_hpaned_new();

	if (!layout_location_vertical(priority_location))
		{
		GtkWidget *tmp;

		tmp = v;
		v = h;
		h = tmp;
		}

	gtk_box_pack_start(GTK_BOX(lw->group_box), v, TRUE, TRUE, 0);

	if (!layout_location_first(priority_location))
		{
		gtk_paned_pack1(GTK_PANED(v), h, FALSE, TRUE);
		gtk_paned_pack2(GTK_PANED(v), w3, TRUE, TRUE);

		gtk_paned_pack1(GTK_PANED(h), w1, FALSE, TRUE);
		gtk_paned_pack2(GTK_PANED(h), w2, TRUE, TRUE);
		}
	else
		{
		gtk_paned_pack1(GTK_PANED(v), w1, FALSE, TRUE);
		gtk_paned_pack2(GTK_PANED(v), h, TRUE, TRUE);

		gtk_paned_pack1(GTK_PANED(h), w2, FALSE, TRUE);
		gtk_paned_pack2(GTK_PANED(h), w3, TRUE, TRUE);
		}

	gtk_widget_show(image_sb);
	gtk_widget_show(tools);
	gtk_widget_show(files);

	gtk_widget_show(v);
	gtk_widget_show(h);

	/* fix to have image pane visible when it is left and priority widget */
	if (lw->options.main_window.hdivider_pos == -1 &&
	    w1 == image_sb &&
	    !layout_location_vertical(priority_location) &&
	    layout_location_first(priority_location))
		{
		gtk_widget_set_size_request(image_sb, 200, -1);
		}

	gtk_paned_set_position(GTK_PANED(lw->h_pane), lw->options.main_window.hdivider_pos);
	gtk_paned_set_position(GTK_PANED(lw->v_pane), lw->options.main_window.vdivider_pos);

	image_grab_focus(lw->image);
}

void layout_style_set(LayoutWindow *lw, gint style, const gchar *order)
{
	FileData *dir_fd;
	gint i;

	if (!layout_valid(&lw)) return;

	if (style != -1)
		{
		LayoutLocation d, f, i;

		layout_config_parse(style, order, &d,  &f, &i);

		if (lw->dir_location == d &&
		    lw->file_location == f &&
		    lw->image_location == i) return;

		lw->dir_location = d;
		lw->file_location = f;
		lw->image_location = i;
		}

	/* remember state */

	/* layout_image_slideshow_stop(lw); slideshow should survive */ 
	layout_image_full_screen_stop(lw);

	dir_fd = lw->dir_fd;
	if (dir_fd) file_data_unregister_real_time_monitor(dir_fd);
	lw->dir_fd = NULL;

	layout_geometry_get_dividers(lw, &lw->options.main_window.hdivider_pos, &lw->options.main_window.vdivider_pos);

	/* preserve utility_box (image + sidebars), menu_bar and toolbars to be reused later in layout_grid_setup */
	/* lw->image is preserved together with lw->utility_box */
	if (lw->utility_box) gtk_container_remove(GTK_CONTAINER(lw->utility_box->parent), lw->utility_box);
	if (lw->menu_bar) gtk_container_remove(GTK_CONTAINER(lw->menu_bar->parent), lw->menu_bar);
	for (i = 0; i < TOOLBAR_COUNT; i++)
		if (lw->toolbar[i]) gtk_container_remove(GTK_CONTAINER(lw->toolbar[i]->parent), lw->toolbar[i]);

	/* clear it all */

	lw->h_pane = NULL;
	lw->v_pane = NULL;

	lw->path_entry = NULL;
	lw->dir_view = NULL;
	lw->vd = NULL;

	lw->file_view = NULL;
	lw->vf = NULL;

	lw->info_box = NULL;
	lw->info_progress_bar = NULL;
	lw->info_sort = NULL;
	lw->info_status = NULL;
	lw->info_details = NULL;
	lw->info_pixel = NULL;
	lw->info_zoom = NULL;

/*
	if (lw->ui_manager) g_object_unref(lw->ui_manager);
	lw->ui_manager = NULL;
	lw->action_group = NULL;
	lw->action_group_editors = NULL;
*/

	gtk_container_remove(GTK_CONTAINER(lw->main_box), lw->group_box);
	lw->group_box = NULL;

	/* re-fill */

	layout_grid_setup(lw);
	layout_tools_hide(lw, lw->options.tools_hidden);

	layout_util_sync(lw);
	layout_status_update_all(lw);


	// printf("%d %d %d \n", G_OBJECT(lw->utility_box)->ref_count, G_OBJECT(lw->menu_bar)->ref_count, G_OBJECT(lw->toolbar)->ref_count);

	/* sync */

	if (image_get_fd(lw->image))
		{
		layout_set_fd(lw, image_get_fd(lw->image));
		}
	else
		{
		layout_set_fd(lw, dir_fd);
		}
	image_top_window_set_sync(lw->image, (lw->options.tools_float || lw->options.tools_hidden));

	/* clean up */

	file_data_unref(dir_fd);
}

void layout_colors_update(void)
{
	GList *work;

	work = layout_window_list;
	while (work)
		{
		gint i;
		LayoutWindow *lw = work->data;
		work = work->next;
		
		if (!lw->image) continue;

		for (i = 0; i < MAX_SPLIT_IMAGES; i++)
			{
			if (!lw->split_images[i]) continue;
			image_background_set_color_from_options(lw->split_images[i], !!lw->full_screen);
			}
		
		image_background_set_color_from_options(lw->image, !!lw->full_screen);
		}
}

void layout_tools_float_toggle(LayoutWindow *lw)
{
	gboolean popped;

	if (!lw) return;

	if (!lw->options.tools_hidden)
		{
		popped = !lw->options.tools_float;
		}
	else
		{
		popped = TRUE;
		}

	if (lw->options.tools_float == popped)
		{
		if (popped && lw->options.tools_hidden)
			{
			layout_tools_float_set(lw, popped, FALSE);
			}
		}
	else
		{
		if (lw->options.tools_float)
			{
			layout_tools_float_set(lw, FALSE, FALSE);
			}
		else
			{
			layout_tools_float_set(lw, TRUE, FALSE);
			}
		}
}

void layout_tools_hide_toggle(LayoutWindow *lw)
{
	if (!lw) return;

	layout_tools_float_set(lw, lw->options.tools_float, !lw->options.tools_hidden);
}

void layout_tools_float_set(LayoutWindow *lw, gboolean popped, gboolean hidden)
{
	if (!layout_valid(&lw)) return;

	if (lw->options.tools_float == popped && lw->options.tools_hidden == hidden) return;

	if (lw->options.tools_float == popped && lw->options.tools_float && lw->tools)
		{
		layout_tools_hide(lw, hidden);
		return;
		}

	lw->options.tools_float = popped;
	lw->options.tools_hidden = hidden;

	layout_style_set(lw, -1, NULL);
}

gboolean layout_tools_float_get(LayoutWindow *lw, gboolean *popped, gboolean *hidden)
{
	if (!layout_valid(&lw)) return FALSE;

	*popped = lw->options.tools_float;
	*hidden = lw->options.tools_hidden;

	return TRUE;
}

void layout_toolbar_toggle(LayoutWindow *lw)
{
	if (!layout_valid(&lw)) return;
	if (!lw->toolbar) return;

	lw->options.toolbar_hidden = !lw->options.toolbar_hidden;

	if (lw->options.toolbar_hidden)
		{
#if GTK_CHECK_VERSION(2,20,0)
		if (gtk_widget_get_visible(lw->toolbar[TOOLBAR_MAIN])) gtk_widget_hide(lw->toolbar[TOOLBAR_MAIN]);
#else
		if (GTK_WIDGET_VISIBLE(lw->toolbar[TOOLBAR_MAIN])) gtk_widget_hide(lw->toolbar[TOOLBAR_MAIN]);
#endif
		}
	else
		{
#if GTK_CHECK_VERSION(2,20,0)
		if (!gtk_widget_get_visible(lw->toolbar[TOOLBAR_MAIN])) gtk_widget_show(lw->toolbar[TOOLBAR_MAIN]);
#else
		if (!GTK_WIDGET_VISIBLE(lw->toolbar[TOOLBAR_MAIN])) gtk_widget_show(lw->toolbar[TOOLBAR_MAIN]);
#endif
		}
}

void layout_info_pixel_set(LayoutWindow *lw, gboolean show)
{
	GtkWidget *frame;
	
	if (!layout_valid(&lw)) return;
	if (!lw->info_pixel) return;

	lw->options.show_info_pixel = show;

	frame = gtk_widget_get_parent(lw->info_pixel);
	if (!lw->options.show_info_pixel)
		{
		gtk_widget_hide(frame);
		}
	else
		{
		gtk_widget_show(frame);
		}
	
	g_signal_emit_by_name (lw->image->pr, "update-pixel");
}

/*
 *-----------------------------------------------------------------------------
 * configuration
 *-----------------------------------------------------------------------------
 */

#define CONFIG_WINDOW_DEF_WIDTH		600
#define CONFIG_WINDOW_DEF_HEIGHT	400

typedef struct _LayoutConfig LayoutConfig;
struct _LayoutConfig
{
	LayoutWindow *lw;

	GtkWidget *configwindow;
	GtkWidget *home_path_entry;
	GtkWidget *layout_widget;
	
	LayoutOptions options;
};

static gint layout_config_delete_cb(GtkWidget *w, GdkEventAny *event, gpointer data);

static void layout_config_close_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	
	gtk_widget_destroy(lc->configwindow);
	free_layout_options_content(&lc->options);
	g_free(lc);
}

static gint layout_config_delete_cb(GtkWidget *w, GdkEventAny *event, gpointer data)
{
	layout_config_close_cb(w, data);
	return TRUE;
}

static void layout_config_apply_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	
	g_free(lc->options.order);
	lc->options.order = layout_config_get(lc->layout_widget, &lc->options.style);

	config_entry_to_option(lc->home_path_entry, &lc->options.home_path, remove_trailing_slash);

	layout_apply_options(lc->lw, &lc->options);
}

static void layout_config_ok_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	layout_config_apply_cb(widget, lc);
	layout_config_close_cb(widget, lc);
}

static void home_path_set_current_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	gtk_entry_set_text(GTK_ENTRY(lc->home_path_entry), layout_get_path(lc->lw));
}

static void startup_path_set_current_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	lc->options.startup_path = STARTUP_PATH_CURRENT;
}

static void startup_path_set_last_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	lc->options.startup_path = STARTUP_PATH_LAST;
}

static void startup_path_set_home_cb(GtkWidget *widget, gpointer data)
{
	LayoutConfig *lc = data;
	lc->options.startup_path = STARTUP_PATH_HOME;
}


/*
static void layout_config_save_cb(GtkWidget *widget, gpointer data)
{
	layout_config_apply();
	save_options(options);
}
*/

void layout_show_config_window(LayoutWindow *lw)
{
	LayoutConfig *lc;
	GtkWidget *win_vbox;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *button;
	GtkWidget *ct_button;
	GtkWidget *group;
	GtkWidget *frame;
	GtkWidget *tabcomp;

	lc = g_new0(LayoutConfig, 1);
	lc->lw = lw;
	layout_sync_options_with_current_state(lw);
	copy_layout_options(&lc->options, &lw->options);

	lc->configwindow = window_new(GTK_WINDOW_TOPLEVEL, "Layout", PIXBUF_INLINE_ICON_CONFIG, NULL, _("Window options and layout"));
	gtk_window_set_type_hint(GTK_WINDOW(lc->configwindow), GDK_WINDOW_TYPE_HINT_DIALOG);

	g_signal_connect(G_OBJECT(lc->configwindow), "delete_event",
			 G_CALLBACK(layout_config_delete_cb), lc);

	gtk_window_set_default_size(GTK_WINDOW(lc->configwindow), CONFIG_WINDOW_DEF_WIDTH, CONFIG_WINDOW_DEF_HEIGHT);
	gtk_window_set_resizable(GTK_WINDOW(lc->configwindow), TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(lc->configwindow), PREF_PAD_BORDER);

	win_vbox = gtk_vbox_new(FALSE, PREF_PAD_SPACE);
	gtk_container_add(GTK_CONTAINER(lc->configwindow), win_vbox);
	gtk_widget_show(win_vbox);

	hbox = gtk_hbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox), GTK_BUTTONBOX_END);
	gtk_box_set_spacing(GTK_BOX(hbox), PREF_PAD_BUTTON_GAP);
	gtk_box_pack_end(GTK_BOX(win_vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	button = pref_button_new(NULL, GTK_STOCK_OK, NULL, FALSE,
				 G_CALLBACK(layout_config_ok_cb), lc);
	gtk_container_add(GTK_CONTAINER(hbox), button);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(button);
	gtk_widget_show(button);

	ct_button = button;
/*
	button = pref_button_new(NULL, GTK_STOCK_SAVE, NULL, FALSE,
				 G_CALLBACK(layout_config_save_cb), NULL);
	gtk_container_add(GTK_CONTAINER(hbox), button);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_show(button);
*/	
	button = pref_button_new(NULL, GTK_STOCK_APPLY, NULL, FALSE,
				 G_CALLBACK(layout_config_apply_cb), lc);
	gtk_container_add(GTK_CONTAINER(hbox), button);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_show(button);

	button = pref_button_new(NULL, GTK_STOCK_CANCEL, NULL, FALSE,
				 G_CALLBACK(layout_config_close_cb), lc);
	gtk_container_add(GTK_CONTAINER(hbox), button);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_widget_show(button);

	if (!generic_dialog_get_alternative_button_order(lc->configwindow))
		{
		gtk_box_reorder_child(GTK_BOX(hbox), ct_button, -1);
		}

	frame = pref_frame_new(win_vbox, TRUE, NULL, GTK_ORIENTATION_VERTICAL, PREF_PAD_GAP);
	
	vbox = gtk_vbox_new(FALSE, PREF_PAD_SPACE);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);


	group = pref_group_new(vbox, FALSE, _("General options"), GTK_ORIENTATION_VERTICAL);

	pref_label_new(group, _("Home path (empty to use your home directory)"));
	hbox = pref_box_new(group, FALSE, GTK_ORIENTATION_HORIZONTAL, PREF_PAD_SPACE);

	tabcomp = tab_completion_new(&lc->home_path_entry, lc->options.home_path, NULL, NULL);
	tab_completion_add_select_button(lc->home_path_entry, NULL, TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), tabcomp, TRUE, TRUE, 0);
	gtk_widget_show(tabcomp);

	button = pref_button_new(hbox, NULL, _("Use current"), FALSE,
				 G_CALLBACK(home_path_set_current_cb), lc);

	pref_checkbox_new_int(group, _("Show date in directories list view"),
			      lc->options.show_directory_date, &lc->options.show_directory_date);

	pref_checkbox_new_int(group, _("Exit program when this window is closed"),
			      lc->options.exit_on_close, &lc->options.exit_on_close);

	group = pref_group_new(vbox, FALSE, _("Start-up directory:"), GTK_ORIENTATION_VERTICAL);

	button = pref_radiobutton_new(group, NULL, _("No change"),
				      (lc->options.startup_path == STARTUP_PATH_CURRENT),
				      G_CALLBACK(startup_path_set_current_cb), lc);
	button = pref_radiobutton_new(group, button, _("Restore last path"),
				      (lc->options.startup_path == STARTUP_PATH_LAST),
				      G_CALLBACK(startup_path_set_last_cb), lc);
	button = pref_radiobutton_new(group, button, _("Home path"),
				      (lc->options.startup_path == STARTUP_PATH_HOME),
				      G_CALLBACK(startup_path_set_home_cb), lc);

	group = pref_group_new(vbox, FALSE, _("Layout"), GTK_ORIENTATION_VERTICAL);

	lc->layout_widget = layout_config_new();
	layout_config_set(lc->layout_widget, lw->options.style, lw->options.order);
	gtk_box_pack_start(GTK_BOX(group), lc->layout_widget, TRUE, TRUE, 0);

	gtk_widget_show(lc->layout_widget);
	gtk_widget_show(lc->configwindow);
}

/*
 *-----------------------------------------------------------------------------
 * base
 *-----------------------------------------------------------------------------
 */

void layout_sync_options_with_current_state(LayoutWindow *lw)
{
	Histogram *histogram;
	if (!layout_valid(&lw)) return;

	lw->options.main_window.maximized =  window_maximized(lw->window);
	if (!lw->options.main_window.maximized)
		{
		layout_geometry_get(lw, &lw->options.main_window.x, &lw->options.main_window.y,
				    &lw->options.main_window.w, &lw->options.main_window.h);
		}

	layout_geometry_get_dividers(lw, &lw->options.main_window.hdivider_pos, &lw->options.main_window.vdivider_pos);

//	layout_sort_get(NULL, &options->file_sort.method, &options->file_sort.ascending);

	layout_geometry_get_tools(lw, &lw->options.float_window.x, &lw->options.float_window.y,
				  &lw->options.float_window.w, &lw->options.float_window.h, &lw->options.float_window.vdivider_pos);

	lw->options.image_overlay.state = image_osd_get(lw->image);
	histogram = image_osd_get_histogram(lw->image);
	
	lw->options.image_overlay.histogram_channel = histogram->histogram_channel;
	lw->options.image_overlay.histogram_mode = histogram->histogram_mode;

	g_free(lw->options.last_path);
	lw->options.last_path = g_strdup(layout_get_path(lw));
}

void layout_apply_options(LayoutWindow *lw, LayoutOptions *lop)
{
	gboolean refresh_style;
	gboolean refresh_lists;

	if (!layout_valid(&lw)) return;
/* FIXME: add other options too */

	refresh_style = (lop->style != lw->options.style || strcmp(lop->order, lw->options.order) != 0);
	refresh_lists = (lop->show_directory_date != lw->options.show_directory_date);

	copy_layout_options(&lw->options, lop);

	if (refresh_style) layout_style_set(lw, lw->options.style, lw->options.order);
	if (refresh_lists) layout_refresh(lw);
}


void layout_close(LayoutWindow *lw)
{
	if (!lw->options.exit_on_close && layout_window_list && layout_window_list->next)
		{
		layout_free(lw);
		}
	else
		{
		exit_program();
		}
}

void layout_free(LayoutWindow *lw)
{
	gint i;
	if (!lw) return;

	layout_window_list = g_list_remove(layout_window_list, lw);
	if (current_lw == lw) current_lw = NULL;

	if (lw->exif_window) g_signal_handlers_disconnect_matched(G_OBJECT(lw->exif_window), G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, lw);
		
	layout_bars_close(lw);

	g_object_unref(lw->menu_bar);
	g_object_unref(lw->utility_box);

	for (i = 0; i < TOOLBAR_COUNT; i++)
		{
		if (lw->toolbar[i]) g_object_unref(lw->toolbar[i]);
		string_list_free(lw->toolbar_actions[i]);
		}
	
	gtk_widget_destroy(lw->window);
	
	if (lw->split_image_sizegroup) g_object_unref(lw->split_image_sizegroup);

	file_data_unregister_notify_func(layout_image_notify_cb, lw);

	if (lw->dir_fd)
		{
		file_data_unregister_real_time_monitor(lw->dir_fd);
		file_data_unref(lw->dir_fd);
		}

	free_layout_options_content(&lw->options);
	g_free(lw);
}

static gboolean layout_delete_cb(GtkWidget *widget, GdkEventAny *event, gpointer data)
{
	LayoutWindow *lw = data;

	layout_close(lw);
	return TRUE;
}

LayoutWindow *layout_new(FileData *dir_fd, LayoutOptions *lop)
{
	return layout_new_with_geometry(dir_fd, lop, NULL);
}

LayoutWindow *layout_new_with_geometry(FileData *dir_fd, LayoutOptions *lop,
				       const gchar *geometry)
{
	LayoutWindow *lw;
	GdkGeometry hint;
	GdkWindowHints hint_mask;
	Histogram *histogram;

	DEBUG_1("%s layout_new: start", get_exec_time());
	lw = g_new0(LayoutWindow, 1);

	if (lop)
		copy_layout_options(&lw->options, lop);
	else
		init_layout_options(&lw->options);

	lw->sort_method = SORT_NAME;
	lw->sort_ascend = TRUE;

	layout_set_unique_id(lw);
//	lw->options.tools_float = popped;
//	lw->options.tools_hidden = hidden;
//	lw->bar_sort_enabled = options->panels.sort.enabled;
//	lw->bar_enabled = options->panels.info.enabled;

	/* default layout */

	layout_config_parse(lw->options.style, lw->options.order,
			    &lw->dir_location,  &lw->file_location, &lw->image_location);
	if (lw->options.dir_view_type >= VIEW_DIR_TYPES_COUNT) lw->options.dir_view_type = 0;
	if (lw->options.file_view_type >= VIEW_FILE_TYPES_COUNT) lw->options.file_view_type = 0;

	/* divider positions */

	if (!options->save_window_positions)
		{
		lw->options.main_window.hdivider_pos = MAIN_WINDOW_DIV_HPOS;
		lw->options.main_window.vdivider_pos = MAIN_WINDOW_DIV_VPOS;
		lw->options.float_window.vdivider_pos = MAIN_WINDOW_DIV_VPOS;
		}

	/* window */

	lw->window = window_new(GTK_WINDOW_TOPLEVEL, GQ_APPNAME_LC, NULL, NULL, NULL);
	gtk_window_set_resizable(GTK_WINDOW(lw->window), TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(lw->window), 0);

	if (options->save_window_positions)
		{
		hint_mask = GDK_HINT_USER_POS;
		}
	else
		{
		hint_mask = 0;
		}

	hint.min_width = 32;
	hint.min_height = 32;
	hint.base_width = 0;
	hint.base_height = 0;
	gtk_window_set_geometry_hints(GTK_WINDOW(lw->window), NULL, &hint,
				      GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE | hint_mask);

	if (options->save_window_positions)
		{
		gtk_window_set_default_size(GTK_WINDOW(lw->window), lw->options.main_window.w, lw->options.main_window.h);
//		if (!layout_window_list)
//			{
		gtk_window_move(GTK_WINDOW(lw->window), lw->options.main_window.x, lw->options.main_window.y);
		if (lw->options.main_window.maximized) gtk_window_maximize(GTK_WINDOW(lw->window));
//			}
		}
	else
		{
		gtk_window_set_default_size(GTK_WINDOW(lw->window), MAINWINDOW_DEF_WIDTH, MAINWINDOW_DEF_HEIGHT);
		}

	g_signal_connect(G_OBJECT(lw->window), "delete_event",
			 G_CALLBACK(layout_delete_cb), lw);

	g_signal_connect(G_OBJECT(lw->window), "focus-in-event",
			 G_CALLBACK(layout_set_current_cb), lw);

	layout_keyboard_init(lw, lw->window);

#ifdef HAVE_LIRC
	layout_image_lirc_init(lw);
#endif

	lw->main_box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(lw->window), lw->main_box);
	gtk_widget_show(lw->main_box);

	layout_grid_setup(lw);
	image_top_window_set_sync(lw->image, (lw->options.tools_float || lw->options.tools_hidden));

	layout_util_sync(lw);
	layout_status_update_all(lw);

	if (dir_fd)
		{
		layout_set_fd(lw, dir_fd);
		}
	else
		{
		GdkPixbuf *pixbuf;

		pixbuf = pixbuf_inline(PIXBUF_INLINE_LOGO);
		
		/* FIXME: the zoom value set here is the value, which is then copied again and again
		   in "Leave Zoom at previous setting" mode. This is not ideal.  */
		image_change_pixbuf(lw->image, pixbuf, 0.0, FALSE);
		g_object_unref(pixbuf);
		}

	if (geometry)
		{
		if (!gtk_window_parse_geometry(GTK_WINDOW(lw->window), geometry))
			{
			log_printf("%s", _("Invalid geometry\n"));
			}
		}

	gtk_widget_show(lw->window);
	layout_tools_hide(lw, lw->options.tools_hidden);

	image_osd_set(lw->image, lw->options.image_overlay.state);
	histogram = image_osd_get_histogram(lw->image);
	
	histogram->histogram_channel = lw->options.image_overlay.histogram_channel;
	histogram->histogram_mode = lw->options.image_overlay.histogram_mode;

	layout_window_list = g_list_append(layout_window_list, lw);

	file_data_register_notify_func(layout_image_notify_cb, lw, NOTIFY_PRIORITY_LOW);

	DEBUG_1("%s layout_new: end", get_exec_time());

	return lw;
}

void layout_write_attributes(LayoutOptions *layout, GString *outstr, gint indent)
{
	WRITE_NL(); WRITE_CHAR(*layout, id);

	WRITE_NL(); WRITE_INT(*layout, style);
	WRITE_NL(); WRITE_CHAR(*layout, order);
	WRITE_NL(); WRITE_UINT(*layout, dir_view_type);
	WRITE_NL(); WRITE_UINT(*layout, file_view_type);
	WRITE_NL(); WRITE_BOOL(*layout, show_marks);
	WRITE_NL(); WRITE_BOOL(*layout, show_thumbnails);
	WRITE_NL(); WRITE_BOOL(*layout, show_directory_date);
	WRITE_NL(); WRITE_CHAR(*layout, home_path);
	WRITE_NL(); WRITE_CHAR(*layout, last_path);
	WRITE_NL(); WRITE_UINT(*layout, startup_path);
	WRITE_NL(); WRITE_BOOL(*layout, exit_on_close);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_INT(*layout, main_window.x);
	WRITE_NL(); WRITE_INT(*layout, main_window.y);
	WRITE_NL(); WRITE_INT(*layout, main_window.w);
	WRITE_NL(); WRITE_INT(*layout, main_window.h);
	WRITE_NL(); WRITE_BOOL(*layout, main_window.maximized);
	WRITE_NL(); WRITE_INT(*layout, main_window.hdivider_pos);
	WRITE_NL(); WRITE_INT(*layout, main_window.vdivider_pos);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_INT(*layout, float_window.x);
	WRITE_NL(); WRITE_INT(*layout, float_window.y);
	WRITE_NL(); WRITE_INT(*layout, float_window.w);
	WRITE_NL(); WRITE_INT(*layout, float_window.h);
	WRITE_NL(); WRITE_INT(*layout, float_window.vdivider_pos);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_INT(*layout, properties_window.w);
	WRITE_NL(); WRITE_INT(*layout, properties_window.h);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_BOOL(*layout, tools_float);
	WRITE_NL(); WRITE_BOOL(*layout, tools_hidden);
	WRITE_SEPARATOR();

	WRITE_NL(); WRITE_BOOL(*layout, toolbar_hidden);
	WRITE_NL(); WRITE_BOOL(*layout, show_info_pixel);

	WRITE_NL(); WRITE_UINT(*layout, image_overlay.state);
	WRITE_NL(); WRITE_INT(*layout, image_overlay.histogram_channel);
	WRITE_NL(); WRITE_INT(*layout, image_overlay.histogram_mode);
}


void layout_write_config(LayoutWindow *lw, GString *outstr, gint indent)
{
	layout_sync_options_with_current_state(lw);
	WRITE_NL(); WRITE_STRING("<layout");
	layout_write_attributes(&lw->options, outstr, indent + 1);
	WRITE_STRING(">");

	bar_sort_write_config(lw->bar_sort, outstr, indent + 1);
	bar_write_config(lw->bar, outstr, indent + 1);
	
	layout_toolbar_write_config(lw, TOOLBAR_MAIN, outstr, indent + 1);
	layout_toolbar_write_config(lw, TOOLBAR_STATUS, outstr, indent + 1);

	WRITE_NL(); WRITE_STRING("</layout>");
}

void layout_load_attributes(LayoutOptions *layout, const gchar **attribute_names, const gchar **attribute_values)
{
	gchar *id = NULL;
	
	while (*attribute_names)
		{
		const gchar *option = *attribute_names++;
		const gchar *value = *attribute_values++;

		/* layout options */
		if (READ_CHAR_FULL("id", id)) continue;

		if (READ_INT(*layout, style)) continue;
		if (READ_CHAR(*layout, order)) continue;
		
		if (READ_UINT(*layout, dir_view_type)) continue;
		if (READ_UINT(*layout, file_view_type)) continue;
		if (READ_BOOL(*layout, show_marks)) continue;
		if (READ_BOOL(*layout, show_thumbnails)) continue;
		if (READ_BOOL(*layout, show_directory_date)) continue;
		if (READ_CHAR(*layout, home_path)) continue;
		if (READ_CHAR(*layout, last_path)) continue;
		if (READ_UINT_CLAMP(*layout, startup_path, 0, STARTUP_PATH_HOME)) continue;
		if (READ_BOOL(*layout, exit_on_close)) continue;

		/* window positions */

		if (READ_INT(*layout, main_window.x)) continue;
		if (READ_INT(*layout, main_window.y)) continue;
		if (READ_INT(*layout, main_window.w)) continue;
		if (READ_INT(*layout, main_window.h)) continue;
		if (READ_BOOL(*layout, main_window.maximized)) continue;
		if (READ_INT(*layout, main_window.hdivider_pos)) continue;
		if (READ_INT(*layout, main_window.vdivider_pos)) continue;

		if (READ_INT(*layout, float_window.x)) continue;
		if (READ_INT(*layout, float_window.y)) continue;
		if (READ_INT(*layout, float_window.w)) continue;
		if (READ_INT(*layout, float_window.h)) continue;
		if (READ_INT(*layout, float_window.vdivider_pos)) continue;
	
		if (READ_INT(*layout, properties_window.w)) continue;
		if (READ_INT(*layout, properties_window.h)) continue;

		if (READ_BOOL(*layout, tools_float)) continue;
		if (READ_BOOL(*layout, tools_hidden)) continue;
		if (READ_BOOL(*layout, toolbar_hidden)) continue;
		if (READ_BOOL(*layout, show_info_pixel)) continue;

		if (READ_UINT(*layout, image_overlay.state)) continue;
		if (READ_INT(*layout, image_overlay.histogram_channel)) continue;
		if (READ_INT(*layout, image_overlay.histogram_mode)) continue;

		log_printf("unknown attribute %s = %s\n", option, value);
		}
	if (id && strcmp(id, LAYOUT_ID_CURRENT) != 0)
		{
		g_free(layout->id);
		layout->id = id;
		}
	else
		{
		g_free(id);
		}
}

static void layout_config_startup_path(LayoutOptions *lop, gchar **path)
{
	switch (lop->startup_path)
		{
		case STARTUP_PATH_LAST:
			*path = (lop->last_path && isdir(lop->last_path)) ? g_strdup(lop->last_path) : get_current_dir();
			break;
		case STARTUP_PATH_HOME:
			*path = (lop->home_path && isdir(lop->home_path)) ? g_strdup(lop->home_path) : g_strdup(homedir());
			break;
		default:
			*path = get_current_dir();
			break;
		}
}	


static void layout_config_commandline(LayoutOptions *lop, gchar **path)
{
	if (command_line->startup_blank)
		{
		*path = NULL;
		}
	else if (command_line->file)
		{
		*path = g_strdup(command_line->file);
		}
	else if (command_line->path)
		{
		*path = g_strdup(command_line->path);
		}
	else layout_config_startup_path(lop, path);
	
	if (command_line->tools_show)
		{
		lop->tools_float = FALSE;
		lop->tools_hidden = FALSE;
		}
	else if (command_line->tools_hide)
		{
		lop->tools_hidden = TRUE;
		}
}

LayoutWindow *layout_new_from_config(const gchar **attribute_names, const gchar **attribute_values, gboolean use_commandline)
{
	LayoutOptions lop;
	LayoutWindow *lw;
	gchar *path = NULL;
	
	init_layout_options(&lop);

	if (attribute_names) layout_load_attributes(&lop, attribute_names, attribute_values);
	
	if (use_commandline)
		{
		layout_config_commandline(&lop, &path);
		}
	else 
		{
		layout_config_startup_path(&lop, &path);
		}

	lw = layout_new_with_geometry(NULL, &lop, use_commandline ? command_line->geometry : NULL);
	layout_sort_set(lw, options->file_sort.method, options->file_sort.ascending);
	layout_set_path(lw, path);

	if (use_commandline && command_line->startup_full_screen) layout_image_full_screen_start(lw);
	if (use_commandline && command_line->startup_in_slideshow) layout_image_slideshow_start(lw);


	g_free(path);
	free_layout_options_content(&lop);
	return lw;
}

void layout_update_from_config(LayoutWindow *lw, const gchar **attribute_names, const gchar **attribute_values)
{
	LayoutOptions lop;
	
	init_layout_options(&lop);

	if (attribute_names) layout_load_attributes(&lop, attribute_names, attribute_values);

	layout_apply_options(lw, &lop);
		
	free_layout_options_content(&lop);
}


/* vim: set shiftwidth=8 softtabstop=0 cindent cinoptions={1s: */
