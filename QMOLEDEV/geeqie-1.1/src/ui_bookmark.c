/*
 * (SLIK) SimpLIstic sKin functions
 * (C) 2004 John Ellis
 * Copyright (C) 2008 - 2012 The Geeqie Team
 *
 * Author: John Ellis
 *
 * This software is released under the GNU General Public License (GNU GPL).
 * Please read the included file COPYING for more information.
 * This software comes with no warranty of any kind, use at your own risk!
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "intl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include <gdk/gdkkeysyms.h> /* for key values */

#include "main.h"

#include "filedata.h"
#include "history_list.h"

#include "ui_bookmark.h"
#include "ui_fileops.h"
#include "ui_menu.h"
#include "ui_misc.h"
#include "ui_utildlg.h"
#include "ui_tabcomp.h"
#include "uri_utils.h"



/*
 *-----------------------------------------------------------------------------
 * bookmarks
 *-----------------------------------------------------------------------------
 */

#define BOOKMARK_DATA_KEY "bookmarkdata"
#define MARKER_PATH "[path]"
#define MARKER_ICON "[icon]"

typedef struct _BookMarkData BookMarkData;
typedef struct _BookButtonData BookButtonData;
typedef struct _BookPropData BookPropData;

struct _BookMarkData
{
	GtkWidget *widget;
	GtkWidget *box;
	gchar *key;

	void (*select_func)(const gchar *path, gpointer data);
	gpointer select_data;

	gboolean no_defaults;
	gboolean editable;
	gboolean only_directories;

	BookButtonData *active_button;
};

struct _BookButtonData
{
	GtkWidget *button;
	GtkWidget *image;
	GtkWidget *label;

	gchar *key;
	gchar *name;
	gchar *path;
	gchar *icon;
	gchar *parent;
};

struct _BookPropData
{
	GtkWidget *name_entry;
	GtkWidget *path_entry;
	GtkWidget *icon_entry;

	BookButtonData *bb;
};

enum {
	TARGET_URI_LIST,
	TARGET_X_URL,
	TARGET_TEXT_PLAIN
};

static GtkTargetEntry bookmark_drop_types[] = {
	{ "text/uri-list", 0, TARGET_URI_LIST },
	{ "x-url/http",    0, TARGET_X_URL },
	{ "_NETSCAPE_URL", 0, TARGET_X_URL }
};
#define bookmark_drop_types_n 3

static GtkTargetEntry bookmark_drag_types[] = {
	{ "text/uri-list", 0, TARGET_URI_LIST },
	{ "text/plain",    0, TARGET_TEXT_PLAIN }
};
#define bookmark_drag_types_n 2


static GList *bookmark_widget_list = NULL;
static GList *bookmark_default_list = NULL;


static void bookmark_populate_all(const gchar *key);


static BookButtonData *bookmark_from_string(const gchar *text)
{
	BookButtonData *b;
	const gchar *path_ptr;
	const gchar *icon_ptr;

	b = g_new0(BookButtonData, 1);

	if (!text)
		{
		b->name = g_strdup(_("New Bookmark"));
		b->path = g_strdup(homedir());
		b->key = NULL;
		return b;
		}

	b->key = g_strdup(text);

	path_ptr = strstr(text, MARKER_PATH);
	icon_ptr = strstr(text, MARKER_ICON);

	if (path_ptr && icon_ptr && icon_ptr < path_ptr)
		{
		log_printf("warning, bookmark icon must be after path\n");
		return NULL;
		}

	if (path_ptr)
		{
		gint l;

		l = path_ptr - text;
		b->name = g_strndup(text, l);
		path_ptr += strlen(MARKER_PATH);
		if (icon_ptr)
			{
			l = icon_ptr - path_ptr;
			b->path = g_strndup(path_ptr, l);
			}
		else
			{
			b->path = g_strdup(path_ptr);
			}
		}
	else
		{
		b->name = g_strdup(text);
		b->path = g_strdup("");
		}

	if (icon_ptr)
		{
		icon_ptr += strlen(MARKER_ICON);
		b->icon = g_strdup(icon_ptr);
		}

	return b;
}

static void bookmark_free(BookButtonData *b)
{
	if (!b) return;

	g_free(b->name);
	g_free(b->path);
	g_free(b->icon);
	g_free(b->key);
	g_free(b->parent);
	g_free(b);
}

static gchar *bookmark_string(const gchar *name, const gchar *path, const gchar *icon)
{
	if (!name) name = _("New Bookmark");
	if (icon && strncmp(icon, G_DIR_SEPARATOR_S, 1) != 0) icon = NULL;

	if (icon)
		{
		return g_strdup_printf("%s"MARKER_PATH"%s"MARKER_ICON"%s", name, path, icon);
		}

	return g_strdup_printf("%s"MARKER_PATH"%s", name, path);
}

static void bookmark_select_cb(GtkWidget *button, gpointer data)
{
	BookMarkData *bm = data;
	BookButtonData *b;

	b = g_object_get_data(G_OBJECT(button), "bookbuttondata");
	if (!b) return;

	if (bm->select_func) bm->select_func(b->path, bm->select_data);
}

static void bookmark_edit_destroy_cb(GtkWidget *widget, gpointer data)
{
	BookPropData *p = data;

	bookmark_free(p->bb);
	g_free(p);
}

static void bookmark_edit_cancel_cb(GenericDialog *gd, gpointer data)
{
}

static void bookmark_edit_ok_cb(GenericDialog *gd, gpointer data)
{
	BookPropData *p = data;
	const gchar *name;
	gchar *path;
	const gchar *icon;
	gchar *new;

	name = gtk_entry_get_text(GTK_ENTRY(p->name_entry));
	path = remove_trailing_slash(gtk_entry_get_text(GTK_ENTRY(p->path_entry)));
	icon = gtk_entry_get_text(GTK_ENTRY(p->icon_entry));

	new = bookmark_string(name, path, icon);

	if (p->bb->key)
		{
		history_list_item_change(p->bb->parent, p->bb->key, new);
		}
	else
		{
		history_list_add_to_key(p->bb->parent, new, 0);
		}

	if (path && strlen(path) > 0) tab_completion_append_to_history(p->path_entry, path);
	if (icon && strlen(icon) > 0) tab_completion_append_to_history(p->icon_entry, icon);

	g_free(path);
	g_free(new);

	bookmark_populate_all(p->bb->parent);
}

/* simply pass NULL for text to turn this into a 'new bookmark' dialog */

static void bookmark_edit(const gchar *key, const gchar *text, GtkWidget *parent)
{
	BookPropData *p;
	GenericDialog *gd;
	GtkWidget *table;
	GtkWidget *label;
	const gchar *icon;

	if (!key) key = "bookmarks";

	p = g_new0(BookPropData, 1);

	p->bb = bookmark_from_string(text);
	p->bb->parent = g_strdup(key);

	gd = generic_dialog_new(_("Edit Bookmark"), "bookmark_edit",
				parent, TRUE,
				bookmark_edit_cancel_cb, p);
	g_signal_connect(G_OBJECT(gd->dialog), "destroy",
			 G_CALLBACK(bookmark_edit_destroy_cb), p);

	generic_dialog_add_message(gd, NULL, _("Edit Bookmark"), NULL);

	generic_dialog_add_button(gd, GTK_STOCK_OK, NULL,
				  bookmark_edit_ok_cb, TRUE);

	table = pref_table_new(gd->vbox, 3, 2, FALSE, TRUE);
	pref_table_label(table, 0, 0, _("Name:"), 1.0);

	p->name_entry = gtk_entry_new();
	gtk_widget_set_size_request(p->name_entry, 300, -1);
	if (p->bb->name) gtk_entry_set_text(GTK_ENTRY(p->name_entry), p->bb->name);
	gtk_table_attach_defaults(GTK_TABLE(table), p->name_entry, 1, 2, 0, 1);
	generic_dialog_attach_default(gd, p->name_entry);
	gtk_widget_show(p->name_entry);

	pref_table_label(table, 0, 1, _("Path:"), 1.0);

	label = tab_completion_new_with_history(&p->path_entry, p->bb->path,
						"bookmark_path", -1, NULL, NULL);
	tab_completion_add_select_button(p->path_entry, NULL, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 1, 2);
	generic_dialog_attach_default(gd, p->path_entry);
	gtk_widget_show(label);

	pref_table_label(table, 0, 2, _("Icon:"), 1.0);

	icon = p->bb->icon;
	if (!icon) icon = "";
	label = tab_completion_new_with_history(&p->icon_entry, icon,
						"bookmark_icons", -1, NULL, NULL);
	tab_completion_add_select_button(p->icon_entry, _("Select icon"), FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, 2, 3);
	generic_dialog_attach_default(gd, p->icon_entry);
	gtk_widget_show(label);

	gtk_widget_show(gd->dialog);
}

static void bookmark_move(BookMarkData *bm, GtkWidget *button, gint direction)
{
	BookButtonData *b;
	gint p;
	GList *list;
	gchar *key_holder;

	if (!bm->editable) return;

	b = g_object_get_data(G_OBJECT(button), "bookbuttondata");
	if (!b) return;

	list = gtk_container_get_children(GTK_CONTAINER(bm->box));
	p = g_list_index(list, button);
	g_list_free(list);

	if (p < 0 || p + direction < 0) return;

	key_holder = bm->key;
	bm->key = "_TEMPHOLDER";
	history_list_item_move(key_holder, b->key, -direction);
	bookmark_populate_all(key_holder);
	bm->key = key_holder;

	gtk_box_reorder_child(GTK_BOX(bm->box), button, p + direction);
}

static void bookmark_menu_prop_cb(GtkWidget *widget, gpointer data)
{
	BookMarkData *bm = data;

	if (!bm->active_button) return;

	bookmark_edit(bm->key, bm->active_button->key, widget);
}

static void bookmark_menu_move(BookMarkData *bm, gint direction)
{
	if (!bm->active_button) return;

	bookmark_move(bm, bm->active_button->button, direction);
}

static void bookmark_menu_up_cb(GtkWidget *widget, gpointer data)
{
	bookmark_menu_move(data, -1);
}

static void bookmark_menu_down_cb(GtkWidget *widget, gpointer data)
{
	bookmark_menu_move(data, 1);
}

static void bookmark_menu_remove_cb(GtkWidget *widget, gpointer data)
{
	BookMarkData *bm = data;

	if (!bm->active_button) return;

	history_list_item_remove(bm->key, bm->active_button->key);
	bookmark_populate_all(bm->key);
}

static void bookmark_menu_position_cb(GtkMenu *menu, gint *x, gint *y, gint *pushed_in, gpointer data)
{
	GtkWidget *button = data;

	gdk_window_get_origin(button->window, x, y);
	*y += button->allocation.y + button->allocation.height;
}

static void bookmark_menu_popup(BookMarkData *bm, GtkWidget *button,
				gint button_n, guint32 time, gboolean local)
{
	GtkWidget *menu;
	BookButtonData *b;

	b = g_object_get_data(G_OBJECT(button), "bookbuttondata");
	if (!b) return;

	bm->active_button = b;

	menu = popup_menu_short_lived();
	menu_item_add_stock_sensitive(menu, _("_Properties..."), GTK_STOCK_PROPERTIES, bm->editable,
		      G_CALLBACK(bookmark_menu_prop_cb), bm);
	menu_item_add_stock_sensitive(menu, _("Move _up"), GTK_STOCK_GO_UP, bm->editable,
		      G_CALLBACK(bookmark_menu_up_cb), bm);
	menu_item_add_stock_sensitive(menu, _("Move _down"), GTK_STOCK_GO_DOWN, bm->editable,
		      G_CALLBACK(bookmark_menu_down_cb), bm);
	menu_item_add_stock_sensitive(menu, _("_Remove"), GTK_STOCK_REMOVE, bm->editable,
		      G_CALLBACK(bookmark_menu_remove_cb), bm);

	if (local)
		{
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL,
			       bookmark_menu_position_cb, button, button_n, time);
		}
	else
		{
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, button_n, time);
		}
}

static gboolean bookmark_press_cb(GtkWidget *button, GdkEventButton *event, gpointer data)
{
	BookMarkData *bm = data;

	if (event->button != MOUSE_BUTTON_RIGHT) return FALSE;

	bookmark_menu_popup(bm, button, event->button, event->time, FALSE);

	return TRUE;
}

static gboolean bookmark_keypress_cb(GtkWidget *button, GdkEventKey *event, gpointer data)
{
	BookMarkData *bm = data;

	switch (event->keyval)
		{
		case GDK_F10:
			if (!(event->state & GDK_CONTROL_MASK)) return FALSE;
		case GDK_Menu:
			bookmark_menu_popup(bm, button, 0, event->time, TRUE);
			return TRUE;
			break;
		case GDK_Up:
			if (event->state & GDK_SHIFT_MASK)
				{
				bookmark_move(bm, button, -1);
				return TRUE;
				}
			break;
		case GDK_Down:
			if (event->state & GDK_SHIFT_MASK)
				{
				bookmark_move(bm, button, 1);
				return TRUE;
				}
			break;
		}

	return FALSE;
}

static void bookmark_drag_set_data(GtkWidget *button,
				   GdkDragContext *context, GtkSelectionData *selection_data,
				   guint info, guint time, gpointer data)
{
	BookMarkData *bm = data;
	BookButtonData *b;
	gchar *uri_text = NULL;
	gint length = 0;
	GList *list = NULL;

	if (context->dest_window == bm->widget->window) return;

	b = g_object_get_data(G_OBJECT(button), "bookbuttondata");
	if (!b) return;

	list = g_list_append(list, b->path);

	switch (info)
		{
		case TARGET_URI_LIST:
			uri_text = uri_text_from_list(list, &length, FALSE);
			break;
		case TARGET_TEXT_PLAIN:
			uri_text = uri_text_from_list(list, &length, TRUE);
			break;
		}

	g_list_free(list);

	if (!uri_text) return;

	gtk_selection_data_set(selection_data, selection_data->target,
			       8, (guchar *)uri_text, length);
	g_free(uri_text);
}

static void bookmark_drag_begin(GtkWidget *button, GdkDragContext *context, gpointer data)
{
	GdkPixbuf *pixbuf;
	GdkModifierType mask;
	gint x, y;

	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,
				button->allocation.width, button->allocation.height);
	gdk_pixbuf_get_from_drawable(pixbuf, button->window, NULL,
				     button->allocation.x, button->allocation.y,
				     0, 0, button->allocation.width, button->allocation.height);

	gdk_window_get_pointer(button->window, &x, &y, &mask);

	gtk_drag_set_icon_pixbuf(context, pixbuf,
				 x - button->allocation.x, y - button->allocation.y);
	g_object_unref(pixbuf);
}

static void bookmark_populate(BookMarkData *bm)
{
	GtkBox *box;
	GList *work;
	GList *children;

	box = GTK_BOX(bm->box);
	children = gtk_container_get_children(GTK_CONTAINER(box));
	work = children;
	while (work)
		{
		GtkWidget *widget = GTK_WIDGET(work->data);
		work = work->next;
		gtk_widget_destroy(widget);
		}

	if (!bm->no_defaults && !history_list_get_by_key(bm->key))
		{
		gchar *buf;
		gchar *path;

		if (!bookmark_default_list)
			{
			buf = bookmark_string(_("Home"), homedir(), NULL);
			history_list_add_to_key(bm->key, buf, 0);
			g_free(buf);

			path = g_build_filename(homedir(), "Desktop", NULL);
			if (isname(path))
				{
				buf = bookmark_string(_("Desktop"), path, NULL);
				history_list_add_to_key(bm->key, buf, 0);
				g_free(buf);
				}
			g_free(path);
			}

		work = bookmark_default_list;
		while (work && work->next)
			{
			gchar *name;

			name = work->data;
			work = work->next;
			path = work->data;
			work = work->next;

			buf = bookmark_string(name, path, NULL);
			history_list_add_to_key(bm->key, buf, 0);
			g_free(buf);
			}
		}

	work = history_list_get_by_key(bm->key);
	work = g_list_last(work);
	while (work)
		{
		BookButtonData *b;

		b = bookmark_from_string(work->data);
		if (b)
			{
			GtkWidget *box;

			b->button = gtk_button_new();
			gtk_button_set_relief(GTK_BUTTON(b->button), GTK_RELIEF_NONE);
			gtk_box_pack_start(GTK_BOX(bm->box), b->button, FALSE, FALSE, 0);
			gtk_widget_show(b->button);

			g_object_set_data_full(G_OBJECT(b->button), "bookbuttondata",
					       b, (GDestroyNotify)bookmark_free);

			box = gtk_hbox_new(FALSE, PREF_PAD_BUTTON_GAP);
			gtk_container_add(GTK_CONTAINER(b->button), box);
			gtk_widget_show(box);

			if (b->icon)
				{
				GdkPixbuf *pixbuf;
				gchar *iconl;

				iconl = path_from_utf8(b->icon);
				pixbuf = gdk_pixbuf_new_from_file(iconl, NULL);
				g_free(iconl);
				if (pixbuf)
					{
					GdkPixbuf *scaled;
					gint w, h;

					w = h = 16;
					gtk_icon_size_lookup(GTK_ICON_SIZE_BUTTON, &w, &h);

					scaled = gdk_pixbuf_scale_simple(pixbuf, w, h,
									 GDK_INTERP_BILINEAR);
					b->image = gtk_image_new_from_pixbuf(scaled);
					g_object_unref(scaled);
					g_object_unref(pixbuf);
					}
				else
					{
					b->image = gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE,
									    GTK_ICON_SIZE_BUTTON);
					}
				}
			else
				{
				b->image = gtk_image_new_from_stock(GTK_STOCK_JUMP_TO, GTK_ICON_SIZE_BUTTON);
				}
			gtk_box_pack_start(GTK_BOX(box), b->image, FALSE, FALSE, 0);
			gtk_widget_show(b->image);

			b->label = gtk_label_new(b->name);
			gtk_box_pack_start(GTK_BOX(box), b->label, FALSE, FALSE, 0);
			gtk_widget_show(b->label);

			g_signal_connect(G_OBJECT(b->button), "clicked",
					 G_CALLBACK(bookmark_select_cb), bm);
			g_signal_connect(G_OBJECT(b->button), "button_press_event",
					 G_CALLBACK(bookmark_press_cb), bm);
			g_signal_connect(G_OBJECT(b->button), "key_press_event",
					 G_CALLBACK(bookmark_keypress_cb), bm);

			gtk_drag_source_set(b->button, GDK_BUTTON1_MASK,
					    bookmark_drag_types, bookmark_drag_types_n,
					    GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
			g_signal_connect(G_OBJECT(b->button), "drag_data_get",
					 G_CALLBACK(bookmark_drag_set_data), bm);
			g_signal_connect(G_OBJECT(b->button), "drag_begin",
					 G_CALLBACK(bookmark_drag_begin), bm);
			}

		work = work->prev;
		}
}

static void bookmark_populate_all(const gchar *key)
{
	GList *work;

	if (!key) return;

	work = bookmark_widget_list;
	while (work)
		{
		BookMarkData *bm;

		bm = work->data;
		work = work->next;

		if (strcmp(bm->key, key) == 0)
			{
			bookmark_populate(bm);
			}
		}
}

static void bookmark_dnd_get_data(GtkWidget *widget,
				  GdkDragContext *context, gint x, gint y,
				  GtkSelectionData *selection_data, guint info,
				  guint time, gpointer data)
{
	BookMarkData *bm = data;
	GList *list = NULL;
	GList *work;

	if (!bm->editable) return;

	switch (info)
		{
		case TARGET_URI_LIST:
		case TARGET_X_URL:
			list = uri_list_from_text((gchar *)selection_data->data, FALSE);
			break;
		}

	work = list;
	while (work)
		{
		gchar *path = work->data;
		gchar *buf;

		work = work->next;

		if (bm->only_directories && !isdir(path)) continue;
		buf = bookmark_string(filename_from_path(path), path, NULL);
		history_list_add_to_key(bm->key, buf, 0);
		g_free(buf);
		}

	string_list_free(list);

	bookmark_populate_all(bm->key);
}

static void bookmark_list_destroy(GtkWidget *widget, gpointer data)
{
	BookMarkData *bm = data;

	bookmark_widget_list = g_list_remove(bookmark_widget_list, bm);

	g_free(bm->key);
	g_free(bm);
}

GtkWidget *bookmark_list_new(const gchar *key,
			     void (*select_func)(const gchar *path, gpointer data), gpointer select_data)
{
	GtkWidget *scrolled;
	BookMarkData *bm;

	if (!key) key = "bookmarks";

	bm = g_new0(BookMarkData, 1);
	bm->key = g_strdup(key);

	bm->select_func = select_func;
	bm->select_data = select_data;

	bm->no_defaults = FALSE;
	bm->editable = TRUE;
	bm->only_directories = FALSE;

	scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
				       GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	bm->box = gtk_vbox_new(FALSE, 0);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled), bm->box);
	gtk_widget_show(bm->box);

	bookmark_populate(bm);

	g_signal_connect(G_OBJECT(bm->box), "destroy",
			 G_CALLBACK(bookmark_list_destroy), bm);
	g_object_set_data(G_OBJECT(bm->box), BOOKMARK_DATA_KEY, bm);
	g_object_set_data(G_OBJECT(scrolled), BOOKMARK_DATA_KEY, bm);
	bm->widget = scrolled;

	gtk_drag_dest_set(scrolled,
			  GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_DROP,
			  bookmark_drop_types, bookmark_drop_types_n,
			  GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK);
	g_signal_connect(G_OBJECT(scrolled), "drag_data_received",
			 G_CALLBACK(bookmark_dnd_get_data), bm);

	bookmark_widget_list = g_list_append(bookmark_widget_list, bm);

	return scrolled;
}

void bookmark_list_set_key(GtkWidget *list, const gchar *key)
{
	BookMarkData *bm;

	if (!list || !key) return;

	bm = g_object_get_data(G_OBJECT(list), BOOKMARK_DATA_KEY);
	if (!bm) return;

	if (bm->key && strcmp(bm->key, key) == 0) return;

	g_free(bm->key);
	bm->key = g_strdup(key);

	bookmark_populate(bm);
}

void bookmark_list_set_no_defaults(GtkWidget *list, gboolean no_defaults)
{
	BookMarkData *bm;

	bm = g_object_get_data(G_OBJECT(list), BOOKMARK_DATA_KEY);
	if (!bm) return;

	bm->no_defaults = no_defaults;
}

void bookmark_list_set_editable(GtkWidget *list, gboolean editable)
{
	BookMarkData *bm;

	bm = g_object_get_data(G_OBJECT(list), BOOKMARK_DATA_KEY);
	if (!bm) return;

	bm->editable = editable;
}

void bookmark_list_set_only_directories(GtkWidget *list, gboolean only_directories)
{
	BookMarkData *bm;

	bm = g_object_get_data(G_OBJECT(list), BOOKMARK_DATA_KEY);
	if (!bm) return;

	bm->only_directories = only_directories;
}

void bookmark_list_add(GtkWidget *list, const gchar *name, const gchar *path)
{
	BookMarkData *bm;
	gchar *buf;

	bm = g_object_get_data(G_OBJECT(list), BOOKMARK_DATA_KEY);
	if (!bm) return;

	buf = bookmark_string(name, path, NULL);
	history_list_add_to_key(bm->key, buf, 0);
	g_free(buf);

	bookmark_populate_all(bm->key);
}

void bookmark_add_default(const gchar *name, const gchar *path)
{
	if (!name || !path) return;
	bookmark_default_list = g_list_append(bookmark_default_list, g_strdup(name));
	bookmark_default_list = g_list_append(bookmark_default_list, g_strdup(path));
}

/*
 *-----------------------------------------------------------------------------
 * combo with history key
 *-----------------------------------------------------------------------------
 */

typedef struct _HistoryComboData HistoryComboData;
struct _HistoryComboData
{
	GtkWidget *combo;
	GtkWidget *entry;
	gchar *history_key;
	gint history_levels;
};

static void history_combo_destroy(GtkWidget *widget, gpointer data)
{
	HistoryComboData *hc = data;

	g_free(hc->history_key);
	g_free(data);
}

/* if text is NULL, entry is set to the most recent item */
GtkWidget *history_combo_new(GtkWidget **entry, const gchar *text,
			     const gchar *history_key, gint max_levels)
{
	HistoryComboData *hc;
	GList *work;
	gint n = 0;

	hc = g_new0(HistoryComboData, 1);
	hc->history_key = g_strdup(history_key);
	hc->history_levels = max_levels;

	hc->combo = gtk_combo_box_entry_new_text();
#if 0
	gtk_combo_set_case_sensitive(GTK_COMBO(hc->combo), TRUE);
	gtk_combo_set_use_arrows(GTK_COMBO(hc->combo), FALSE);
#endif

	hc->entry = GTK_BIN(hc->combo)->child;

	g_object_set_data(G_OBJECT(hc->combo), "history_combo_data", hc);
	g_object_set_data(G_OBJECT(hc->entry), "history_combo_data", hc);
	g_signal_connect(G_OBJECT(hc->combo), "destroy",
			 G_CALLBACK(history_combo_destroy), hc);

	work = history_list_get_by_key(hc->history_key);
	while (work)
		{
		gtk_combo_box_append_text(GTK_COMBO_BOX(hc->combo), (gchar *)work->data);
		work = work->next;
		n++;
		}

	if (text)
		{
		gtk_entry_set_text(GTK_ENTRY(hc->entry), text);
		}
	else if (n > 0)
		{
		gtk_combo_box_set_active(GTK_COMBO_BOX(hc->combo), 0);
		}

	if (entry) *entry = hc->entry;
	return hc->combo;
}

/* if text is NULL, current entry text is used
 * widget can be the combo or entry widget
 */
void history_combo_append_history(GtkWidget *widget, const gchar *text)
{
	HistoryComboData *hc;
	gchar *new_text;

	hc = g_object_get_data(G_OBJECT(widget), "history_combo_data");
	if (!hc)
		{
		log_printf("widget is not a history combo\n");
		return;
		}

	if (text)
		{
		new_text = g_strdup(text);
		}
	else
		{
		new_text = g_strdup(gtk_entry_get_text(GTK_ENTRY(hc->entry)));
		}

	if (new_text && strlen(new_text) > 0)
		{
		GtkTreeModel *store;
		GList *work;

		history_list_add_to_key(hc->history_key, new_text, hc->history_levels);

		gtk_combo_box_set_active(GTK_COMBO_BOX(hc->combo), -1);

		store = gtk_combo_box_get_model(GTK_COMBO_BOX(hc->combo));
		gtk_list_store_clear(GTK_LIST_STORE(store));

		work = history_list_get_by_key(hc->history_key);
		while (work)
			{
			gtk_combo_box_append_text(GTK_COMBO_BOX(hc->combo), (gchar *)work->data);
			work = work->next;
			}
		}

	g_free(new_text);
}
/* vim: set shiftwidth=8 softtabstop=0 cindent cinoptions={1s: */
