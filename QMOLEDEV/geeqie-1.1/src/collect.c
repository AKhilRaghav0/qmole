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
#include "collect.h"

#include "collect-dlg.h"
#include "collect-io.h"
#include "collect-table.h"
#include "editors.h"
#include "filedata.h"
#include "img-view.h"
#include "layout.h"
#include "layout_image.h"
#include "misc.h"
#include "pixbuf_util.h"
#include "print.h"
#include "ui_fileops.h"
#include "ui_tree_edit.h"
#include "utilops.h"
#include "window.h"

#include <gdk/gdkkeysyms.h> /* for keyboard values */


#define COLLECT_DEF_WIDTH 440
#define COLLECT_DEF_HEIGHT 450

static GList *collection_list = NULL;
static GList *collection_window_list = NULL;

static void collection_window_get_geometry(CollectWindow *cw);
static void collection_window_refresh(CollectWindow *cw);
static void collection_window_update_title(CollectWindow *cw);
static void collection_window_add(CollectWindow *cw, CollectInfo *ci);
static void collection_window_insert(CollectWindow *cw, CollectInfo *ci);
static void collection_window_remove(CollectWindow *cw, CollectInfo *ci);
static void collection_window_update(CollectWindow *cw, CollectInfo *ci);

static void collection_window_close(CollectWindow *cw);

static void collection_notify_cb(FileData *fd, NotifyType type, gpointer data);

/*
 *-------------------------------------------------------------------
 * data, list handling
 *-------------------------------------------------------------------
 */

CollectInfo *collection_info_new(FileData *fd, struct stat *st, GdkPixbuf *pixbuf)
{
	CollectInfo *ci;

	if (!fd) return NULL;

	ci = g_new0(CollectInfo, 1);
	ci->fd = file_data_ref(fd);

	ci->pixbuf = pixbuf;
	if (ci->pixbuf) g_object_ref(ci->pixbuf);

	return ci;
}

void collection_info_free_thumb(CollectInfo *ci)
{
	if (ci->pixbuf) g_object_unref(ci->pixbuf);
	ci->pixbuf = NULL;
}

void collection_info_free(CollectInfo *ci)
{
	if (!ci) return;

	file_data_unref(ci->fd);
	collection_info_free_thumb(ci);
	g_free(ci);
}

void collection_info_set_thumb(CollectInfo *ci, GdkPixbuf *pixbuf)
{
	if (pixbuf) g_object_ref(pixbuf);
	collection_info_free_thumb(ci);
	ci->pixbuf = pixbuf;
}

gboolean collection_info_load_thumb(CollectInfo *ci)
{
	if (!ci) return FALSE;

	collection_info_free_thumb(ci);

	log_printf("collection_info_load_thumb not implemented!\n(because an instant thumb loader not implemented)");
	return FALSE;
#if 0
	if (create_thumbnail(ci->fd->path, &ci->pixmap, &ci->mask) < 0) return FALSE;

	if (ci->pixmap) gdk_pixmap_ref(ci->pixmap);
	if (ci->mask) gdk_bitmap_ref(ci->mask);

	return TRUE;
#endif
}

void collection_list_free(GList *list)
{
	GList *work;
	work = list;
	while (work)
		{
		collection_info_free((CollectInfo *)work->data);
		work = work->next;
		}
	g_list_free(list);
}

/* an ugly static var, well what ya gonna do ? */
static SortType collection_list_sort_method = SORT_NAME;

static gint collection_list_sort_cb(gconstpointer a, gconstpointer b)
{
	const CollectInfo *cia = a;
	const CollectInfo *cib = b;

	switch (collection_list_sort_method)
		{
		case SORT_NAME:
			break;
		case SORT_NONE:
			return 0;
			break;
		case SORT_SIZE:
			if (cia->fd->size < cib->fd->size) return -1;
			if (cia->fd->size > cib->fd->size) return 1;
			return 0;
			break;
		case SORT_TIME:
			if (cia->fd->date < cib->fd->date) return -1;
			if (cia->fd->date > cib->fd->date) return 1;
			return 0;
			break;
		case SORT_PATH:
			return utf8_compare(cia->fd->path, cib->fd->path, options->file_sort.case_sensitive);
			break;
#ifdef HAVE_STRVERSCMP
		case SORT_NUMBER:
			return strverscmp(cia->fd->name, cib->fd->name);
			break;
#endif
		default:
			break;
		}

	if (options->file_sort.case_sensitive)
		return strcmp(cia->fd->collate_key_name, cib->fd->collate_key_name);
	else
		return strcmp(cia->fd->collate_key_name_nocase, cib->fd->collate_key_name_nocase);
}

GList *collection_list_sort(GList *list, SortType method)
{
	if (method == SORT_NONE) return list;

	collection_list_sort_method = method;

	return g_list_sort(list, collection_list_sort_cb);
}

GList *collection_list_randomize(GList *list)
{
	guint random, length, i;
	gpointer tmp;
	GList *nlist, *olist;

	length = g_list_length(list);
	if (!length) return NULL;

	srand((unsigned int)time(NULL)); // Initialize random generator (hasn't to be that much strong)

	for (i = 0; i < length; i++)
		{
		random = (guint) (1.0 * length * rand()/(RAND_MAX + 1.0));
		olist = g_list_nth(list, i);
		nlist = g_list_nth(list, random);
		tmp = olist->data;
		olist->data = nlist->data;
		nlist->data = tmp;
		}

	return list;
}

GList *collection_list_add(GList *list, CollectInfo *ci, SortType method)
{
	if (method != SORT_NONE)
		{
		collection_list_sort_method = method;
		list = g_list_insert_sorted(list, ci, collection_list_sort_cb);
		}
	else
		{
		list = g_list_append(list, ci);
		}

	return list;
}

GList *collection_list_insert(GList *list, CollectInfo *ci, CollectInfo *insert_ci, SortType method)
{
	if (method != SORT_NONE)
		{
		collection_list_sort_method = method;
		list = g_list_insert_sorted(list, ci, collection_list_sort_cb);
		}
	else
		{
		GList *point;

		point = g_list_find(list, insert_ci);
		list = uig_list_insert_link(list, point, ci);
		}

	return list;
}

GList *collection_list_remove(GList *list, CollectInfo *ci)
{
	list = g_list_remove(list, ci);
	collection_info_free(ci);
	return list;
}

CollectInfo *collection_list_find_fd(GList *list, FileData *fd)
{
	GList *work = list;

	while (work)
		{
		CollectInfo *ci = work->data;
		if (ci->fd == fd) return ci;
		work = work->next;
		}

	return NULL;
}

#if 0
static GList *collection_list_find_link(GList *list, gchar *path)
{
	GList *work = list;

	while (work)
		{
		CollectInfo *ci = work->data;
		if (strcmp(ci->fd->path, path) == 0) return work;
		work = work->next;
		}

	return NULL;
}

static gint collection_list_find_index(GList *list, gchar *path)
{
	gint c = 0;
	GList *work = list;

	while (work)
		{
		CollectInfo *ci = work->data;
		if (strcmp(ci->fd->path, path) == 0) return c;
		work = work->next;
		c++;
		}

	return -1;
}
#endif

GList *collection_list_to_filelist(GList *list)
{
	GList *filelist = NULL;
	GList *work = list;

	while (work)
		{
		CollectInfo *info = work->data;
		filelist = g_list_prepend(filelist, file_data_ref(info->fd));
		work = work->next;
		}

	filelist = g_list_reverse(filelist);
	return filelist;
}

CollectWindow *collection_window_find(CollectionData *cd)
{
	GList *work;

	work = collection_window_list;
	while (work)
		{
		CollectWindow *cw = work->data;
		if (cw->cd == cd) return cw;
		work = work->next;
		}

	return NULL;
}

CollectWindow *collection_window_find_by_path(const gchar *path)
{
	GList *work;

	if (!path) return NULL;

	work = collection_window_list;
	while (work)
		{
		CollectWindow *cw = work->data;
		if (cw->cd->path && strcmp(cw->cd->path, path) == 0) return cw;
		work = work->next;
		}

	return NULL;
}

/*
 *-------------------------------------------------------------------
 * please use these to actually add/remove stuff
 *-------------------------------------------------------------------
 */

CollectionData *collection_new(const gchar *path)
{
	CollectionData *cd;
	static gint untitled_counter = 0;

	cd = g_new0(CollectionData, 1);

	cd->ref = 1;	/* starts with a ref of 1 */
	cd->sort_method = SORT_NONE;
	cd->window_w = COLLECT_DEF_WIDTH;
	cd->window_h = COLLECT_DEF_HEIGHT;
	cd->existence = g_hash_table_new(NULL, NULL);

	if (path)
		{
		cd->path = g_strdup(path);
		cd->name = g_strdup(filename_from_path(cd->path));
		/* load it */
		}
	else
		{
		if (untitled_counter == 0)
			{
			cd->name = g_strdup(_("Untitled"));
			}
		else
			{
			cd->name = g_strdup_printf(_("Untitled (%d)"), untitled_counter + 1);
			}

		untitled_counter++;
		}

	file_data_register_notify_func(collection_notify_cb, cd, NOTIFY_PRIORITY_MEDIUM);


	collection_list = g_list_append(collection_list, cd);
	
	return cd;
}

void collection_free(CollectionData *cd)
{
	if (!cd) return;

	DEBUG_1("collection \"%s\" freed", cd->name);

	collection_load_stop(cd);
	collection_list_free(cd->list);
	
	file_data_unregister_notify_func(collection_notify_cb, cd);

	collection_list = g_list_remove(collection_list, cd);

	g_hash_table_destroy(cd->existence);

	g_free(cd->path);
	g_free(cd->name);

	g_free(cd);
}

void collection_ref(CollectionData *cd)
{
	cd->ref++;

	DEBUG_1("collection \"%s\" ref count = %d", cd->name, cd->ref);
}

void collection_unref(CollectionData *cd)
{
	cd->ref--;

	DEBUG_1("collection \"%s\" ref count = %d", cd->name, cd->ref);

	if (cd->ref < 1)
		{
		collection_free(cd);
		}
}

void collection_path_changed(CollectionData *cd)
{
	collection_window_update_title(collection_window_find(cd));
}

gint collection_to_number(CollectionData *cd)
{
	return g_list_index(collection_list, cd);
}

CollectionData *collection_from_number(gint n)
{
	return g_list_nth_data(collection_list, n);
}

CollectionData *collection_from_dnd_data(const gchar *data, GList **list, GList **info_list)
{
	CollectionData *cd;
	gint collection_number;
	const gchar *ptr;

	if (list) *list = NULL;
	if (info_list) *info_list = NULL;

	if (strncmp(data, "COLLECTION:", 11) != 0) return NULL;
	
	ptr = data + 11;
		
	collection_number = atoi(ptr);
	cd = collection_from_number(collection_number);
	if (!cd) return NULL;

	if (!list && !info_list) return cd;
	
	while (*ptr != '\0' && *ptr != '\n' ) ptr++;
	if (*ptr == '\0') return cd;
	ptr++;

	while (*ptr != '\0')
		{
		guint item_number;
		CollectInfo *info;
		
		item_number = (guint) atoi(ptr);
		while (*ptr != '\n' && *ptr != '\0') ptr++;
		if (*ptr == '\0')
			break;
		else
			while (*ptr == '\n') ptr++;

		info = g_list_nth_data(cd->list, item_number);
		if (!info) continue;

		if (list) *list = g_list_append(*list, file_data_ref(info->fd));
		if (info_list) *info_list = g_list_append(*info_list, info);
		}
	
	return cd;
}

gchar *collection_info_list_to_dnd_data(CollectionData *cd, GList *list, gint *length)
{
	GList *work;
	GList *temp = NULL;
	gchar *ptr;
	gchar *text;
	gchar *uri_text;
	gint collection_number;

	*length = 0;
	if (!list) return NULL;

	collection_number = collection_to_number(cd);
	if (collection_number < 0) return NULL;

	text = g_strdup_printf("COLLECTION:%d\n", collection_number);
	*length += strlen(text);
	temp = g_list_prepend(temp, text);

	work = list;
	while (work)
		{
		gint item_number = g_list_index(cd->list, work->data);

		work = work->next;

		if (item_number < 0) continue;
		
		text = g_strdup_printf("%d\n", item_number);
		temp = g_list_prepend(temp, text);
		*length += strlen(text);
		}

	*length += 1; /* ending nul char */

	uri_text = g_malloc(*length);
	ptr = uri_text;

	work = g_list_last(temp);
	while (work)
		{
		gint len;
		gchar *text = work->data;

		work = work->prev;

		len = strlen(text);
		memcpy(ptr, text, len);
		ptr += len;
		}

	ptr[0] = '\0';

	string_list_free(temp);

	return uri_text;
}

gint collection_info_valid(CollectionData *cd, CollectInfo *info)
{
	if (collection_to_number(cd) < 0) return FALSE;

	return (g_list_index(cd->list, info) != 0);
}

CollectInfo *collection_next_by_info(CollectionData *cd, CollectInfo *info)
{
	GList *work;

	work = g_list_find(cd->list, info);

	if (!work) return NULL;
	work = work->next;
	if (work) return work->data;
	return NULL;
}

CollectInfo *collection_prev_by_info(CollectionData *cd, CollectInfo *info)
{
	GList *work;

	work = g_list_find(cd->list, info);

	if (!work) return NULL;
	work = work->prev;
	if (work) return work->data;
	return NULL;
}

CollectInfo *collection_get_first(CollectionData *cd)
{
	if (cd->list) return cd->list->data;

	return NULL;
}

CollectInfo *collection_get_last(CollectionData *cd)
{
	GList *list;

	list = g_list_last(cd->list);

	if (list) return list->data;

	return NULL;
}

void collection_set_sort_method(CollectionData *cd, SortType method)
{
	if (!cd) return;

	if (cd->sort_method == method) return;

	cd->sort_method = method;
	cd->list = collection_list_sort(cd->list, cd->sort_method);
	if (cd->list) cd->changed = TRUE;

	collection_window_refresh(collection_window_find(cd));
}

void collection_randomize(CollectionData *cd)
{
	if (!cd) return;

	cd->list = collection_list_randomize(cd->list);
	cd->sort_method = SORT_NONE;
	if (cd->list) cd->changed = TRUE;

	collection_window_refresh(collection_window_find(cd));
}

void collection_set_update_info_func(CollectionData *cd,
				     void (*func)(CollectionData *, CollectInfo *, gpointer), gpointer data)
{
	cd->info_updated_func = func;
	cd->info_updated_data = data;
}

static CollectInfo *collection_info_new_if_not_exists(CollectionData *cd, struct stat *st, FileData *fd)
{
	CollectInfo *ci;

	if (g_hash_table_lookup(cd->existence, fd->path)) return NULL;

	ci = collection_info_new(fd, st, NULL);
	if (ci) g_hash_table_insert(cd->existence, fd->path, "");
	return ci;
}

gboolean collection_add_check(CollectionData *cd, FileData *fd, gboolean sorted, gboolean must_exist)
{
	struct stat st;
	gboolean valid;

	if (must_exist)
		{
		valid = (stat_utf8(fd->path, &st) && !S_ISDIR(st.st_mode));
		}
	else
		{
		valid = TRUE;
		st.st_size = 0;
		st.st_mtime = 0;
		}

	if (valid)
		{
		CollectInfo *ci;

		ci = collection_info_new_if_not_exists(cd, &st, fd);
		if (!ci) return FALSE;
		DEBUG_3("add to collection: %s", fd->path);

		cd->list = collection_list_add(cd->list, ci, sorted ? cd->sort_method : SORT_NONE);
		cd->changed = TRUE;

		if (!sorted || cd->sort_method == SORT_NONE)
			{
			collection_window_add(collection_window_find(cd), ci);
			}
		else
			{
			collection_window_insert(collection_window_find(cd), ci);
			}
		}

	return valid;
}

gboolean collection_add(CollectionData *cd, FileData *fd, gboolean sorted)
{
	return collection_add_check(cd, fd, sorted, TRUE);
}

gboolean collection_insert(CollectionData *cd, FileData *fd, CollectInfo *insert_ci, gboolean sorted)
{
	struct stat st;

	if (!insert_ci) return collection_add(cd, fd, sorted);

	if (stat_utf8(fd->path, &st) >= 0 && !S_ISDIR(st.st_mode))
		{
		CollectInfo *ci;

		ci = collection_info_new_if_not_exists(cd, &st, fd);
		if (!ci) return FALSE;

		DEBUG_3("insert in collection: %s", fd->path);

		cd->list = collection_list_insert(cd->list, ci, insert_ci, sorted ? cd->sort_method : SORT_NONE);
		cd->changed = TRUE;

		collection_window_insert(collection_window_find(cd), ci);

		return TRUE;
		}

	return FALSE;
}

gboolean collection_remove(CollectionData *cd, FileData *fd)
{
	CollectInfo *ci;

	ci = collection_list_find_fd(cd->list, fd);

	if (!ci) return FALSE;

	g_hash_table_remove(cd->existence, fd->path);

	cd->list = g_list_remove(cd->list, ci);
	cd->changed = TRUE;

	collection_window_remove(collection_window_find(cd), ci);
	collection_info_free(ci);

	return TRUE;
}

static void collection_remove_by_info(CollectionData *cd, CollectInfo *info)
{
	if (!info || !g_list_find(cd->list, info)) return;

	cd->list = g_list_remove(cd->list, info);
	cd->changed = (cd->list != NULL);

	collection_window_remove(collection_window_find(cd), info);
	collection_info_free(info);
}

void collection_remove_by_info_list(CollectionData *cd, GList *list)
{
	GList *work;

	if (!list) return;

	if (!list->next)
		{
		/* more efficient (in collect-table) to remove a single item this way */
		collection_remove_by_info(cd, (CollectInfo *)list->data);
		return;
		}

	work = list;
	while (work)
		{
		cd->list = collection_list_remove(cd->list, work->data);
		work = work->next;
		}
	cd->changed = (cd->list != NULL);

	collection_window_refresh(collection_window_find(cd));
}

gboolean collection_rename(CollectionData *cd, FileData *fd)
{
	CollectInfo *ci;
	ci = collection_list_find_fd(cd->list, fd);

	if (!ci) return FALSE;

	cd->changed = TRUE;

	collection_window_update(collection_window_find(cd), ci);

	return TRUE;
}

void collection_update_geometry(CollectionData *cd)
{
	collection_window_get_geometry(collection_window_find(cd));
}

/*
 *-------------------------------------------------------------------
 * simple maintenance for renaming, deleting
 *-------------------------------------------------------------------
 */

static void collection_notify_cb(FileData *fd, NotifyType type, gpointer data)
{
	CollectionData *cd = data;

	if (!(type & NOTIFY_CHANGE) || !fd->change) return;

	DEBUG_1("Notify collection: %s %04x", fd->path, type);
	
	switch (fd->change->type)
		{
		case FILEDATA_CHANGE_MOVE:
		case FILEDATA_CHANGE_RENAME:
			collection_rename(cd, fd);
			break;
		case FILEDATA_CHANGE_COPY:
			break;
		case FILEDATA_CHANGE_DELETE:
			while (collection_remove(cd, fd));
			break;
		case FILEDATA_CHANGE_UNSPECIFIED:
		case FILEDATA_CHANGE_WRITE_METADATA:
			break;
		}

}


/*
 *-------------------------------------------------------------------
 * window key presses
 *-------------------------------------------------------------------
 */

static gboolean collection_window_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	CollectWindow *cw = data;
	gboolean stop_signal = FALSE;
	gint edit_val = -1;
	GList *list;

	if (event->state & GDK_CONTROL_MASK)
		{
		stop_signal = TRUE;
		switch (event->keyval)
			{
			case '1':
				edit_val = 0;
				break;
			case '2':
				edit_val = 1;
				break;
			case '3':
				edit_val = 2;
				break;
			case '4':
				edit_val = 3;
				break;
			case '5':
				edit_val = 4;
				break;
			case '6':
				edit_val = 5;
				break;
			case '7':
				edit_val = 6;
				break;
			case '8':
				edit_val = 7;
				break;
			case '9':
				edit_val = 8;
				break;
			case '0':
				edit_val = 9;
				break;
			case 'A': case 'a':
				if (event->state & GDK_SHIFT_MASK)
					{
					collection_table_unselect_all(cw->table);
					}
				else
					{
					collection_table_select_all(cw->table);
					}
				break;
			case 'L': case 'l':
				list = layout_list(NULL);
				if (list)
					{
					collection_table_add_filelist(cw->table, list);
					filelist_free(list);
					}
				break;
			case 'C': case 'c':
				file_util_copy(NULL, collection_table_selection_get_list(cw->table), NULL, cw->window);
				break;
			case 'M': case 'm':
				file_util_move(NULL, collection_table_selection_get_list(cw->table), NULL, cw->window);
				break;
			case 'R': case 'r':
				file_util_rename(NULL, collection_table_selection_get_list(cw->table), cw->window);
				break;
			case 'D': case 'd':
				file_util_delete(NULL, collection_table_selection_get_list(cw->table), cw->window);
				break;
			case 'S': case 's':
				collection_dialog_save_as(NULL, cw->cd);
				break;
			case 'W': case 'w':
				collection_window_close(cw);
				break;
			default:
				stop_signal = FALSE;
				break;
			}
		}
	else
		{
		stop_signal = TRUE;
		switch (event->keyval)
			{
			case GDK_Return: case GDK_KP_Enter:
				layout_image_set_collection(NULL, cw->cd,
					collection_table_get_focus_info(cw->table));
				break;
			case 'V': case 'v':
				view_window_new_from_collection(cw->cd,
					collection_table_get_focus_info(cw->table));
				break;
			case 'S': case 's':
				if (!cw->cd->path)
					{
					collection_dialog_save_as(NULL, cw->cd);
					}
				else if (!collection_save(cw->cd, cw->cd->path))
					{
					log_printf("failed saving to collection path: %s\n", cw->cd->path);
					}
				break;
			case 'A': case 'a':
				collection_dialog_append(NULL, cw->cd);
				break;
			case 'N': case 'n':
				collection_set_sort_method(cw->cd, SORT_NAME);
				break;
#ifdef HAVE_STRVERSCMP
			case 'I': case 'i':
				collection_set_sort_method(cw->cd, SORT_NUMBER);
				break;
#endif
			case 'D': case 'd':
				collection_set_sort_method(cw->cd, SORT_TIME);
				break;
			case 'B': case 'b':
				collection_set_sort_method(cw->cd, SORT_SIZE);
				break;
			case 'P': case 'p':
				if (event->state & GDK_SHIFT_MASK)
					{
					CollectInfo *info;

					info = collection_table_get_focus_info(cw->table);

					print_window_new(info->fd, collection_table_selection_get_list(cw->table),
							 collection_list_to_filelist(cw->cd->list), cw->window);
					}
				else
					{
					collection_set_sort_method(cw->cd, SORT_PATH);
					}
				break;
			case GDK_Delete: case GDK_KP_Delete:
				list = g_list_copy(cw->table->selection);
				if (list)
					{
					collection_remove_by_info_list(cw->cd, list);
					g_list_free(list);
					}
				else
					{
					collection_remove_by_info(cw->cd, collection_table_get_focus_info(cw->table));
					}
				break;
			default:
				stop_signal = FALSE;
				break;
			}
		}
	return stop_signal;
}

/*
 *-------------------------------------------------------------------
 * window
 *-------------------------------------------------------------------
 */
static void collection_window_get_geometry(CollectWindow *cw)
{
	CollectionData *cd;

	if (!cw) return;

	cd = cw->cd;
	gdk_window_get_position(cw->window->window, &cd->window_x, &cd->window_y);
	gdk_drawable_get_size(cw->window->window, &cd->window_w, &cd->window_h);
	cd->window_read = TRUE;
}

static void collection_window_refresh(CollectWindow *cw)
{
	if (!cw) return;

	collection_table_refresh(cw->table);
}

static void collection_window_update_title(CollectWindow *cw)
{
	gboolean free_name = FALSE;
	gchar *name;
	gchar *buf;

	if (!cw) return;

	if (file_extension_match(cw->cd->name, GQ_COLLECTION_EXT))
		{
		name = remove_extension_from_path(cw->cd->name);
		free_name = TRUE;
		}
	else
		{
		name = cw->cd->name;
		}

	buf = g_strdup_printf(_("%s - Collection - %s"), name, GQ_APPNAME);
	if (free_name) g_free(name);
	gtk_window_set_title(GTK_WINDOW(cw->window), buf);
	g_free(buf);
}

static void collection_window_update_info(CollectionData *cd, CollectInfo *ci, gpointer data)
{
	CollectWindow *cw = data;

	collection_table_file_update(cw->table, ci);
}

static void collection_window_add(CollectWindow *cw, CollectInfo *ci)
{
	if (!cw) return;

	if (!ci->pixbuf) collection_load_thumb_idle(cw->cd);
	collection_table_file_add(cw->table, ci);
}

static void collection_window_insert(CollectWindow *cw, CollectInfo *ci)
{
	if (!cw) return;

	if (!ci->pixbuf) collection_load_thumb_idle(cw->cd);
	collection_table_file_insert(cw->table, ci);
	if (!cw) return;
}

#if 0
static void collection_window_move(CollectWindow *cw, CollectInfo *ci)
{
	if (!cw) return;
}
#endif

static void collection_window_remove(CollectWindow *cw, CollectInfo *ci)
{
	if (!cw) return;

	collection_table_file_remove(cw->table, ci);
}

static void collection_window_update(CollectWindow *cw, CollectInfo *ci)
{
	if (!cw) return;

	collection_table_file_update(cw->table, ci);
	collection_table_file_update(cw->table, NULL);
}

static void collection_window_close_final(CollectWindow *cw)
{
	if (cw->close_dialog) return;

	collection_window_list = g_list_remove(collection_window_list, cw);
	collection_window_get_geometry(cw);

	gtk_widget_destroy(cw->window);

	collection_set_update_info_func(cw->cd, NULL, NULL);
	collection_unref(cw->cd);

	g_free(cw);
}

static void collection_close_save_cb(GenericDialog *gd, gpointer data)
{
	CollectWindow *cw = data;

	cw->close_dialog = NULL;
	generic_dialog_close(gd);

	if (!cw->cd->path)
		{
		collection_dialog_save_close(NULL, cw->cd);
		return;
		}
	else if (!collection_save(cw->cd, cw->cd->path))
		{
		gchar *buf;
		buf = g_strdup_printf(_("Failed to save the collection:\n%s"), cw->cd->path);
		warning_dialog(_("Save Failed"), buf, GTK_STOCK_DIALOG_ERROR, cw->window);
		g_free(buf);
		return;
		}

	collection_window_close_final(cw);
}

static void collection_close_close_cb(GenericDialog *gd, gpointer data)
{
	CollectWindow *cw = data;

	cw->close_dialog = NULL;
	generic_dialog_close(gd);

	collection_window_close_final(cw);
}

static void collection_close_cancel_cb(GenericDialog *gd, gpointer data)
{
	CollectWindow *cw = data;

	cw->close_dialog = NULL;
	generic_dialog_close(gd);
}

static void collection_close_dlg_show(CollectWindow *cw)
{
	GenericDialog *gd;

	if (cw->close_dialog)
		{
		gtk_window_present(GTK_WINDOW(cw->close_dialog));
		return;
		}

	gd = generic_dialog_new(_("Close collection"),
				"close_collection", cw->window, FALSE,
				collection_close_cancel_cb, cw);
	generic_dialog_add_message(gd, GTK_STOCK_DIALOG_QUESTION,
				   _("Close collection"),
				   _("Collection has been modified.\nSave first?"));

	generic_dialog_add_button(gd, GTK_STOCK_SAVE, NULL, collection_close_save_cb, TRUE);
	generic_dialog_add_button(gd, GTK_STOCK_DELETE, _("_Discard"), collection_close_close_cb, FALSE);

	cw->close_dialog = gd->dialog;

	gtk_widget_show(gd->dialog);
}

static void collection_window_close(CollectWindow *cw)
{
	if (!cw->cd->changed && !cw->close_dialog)
		{
		collection_window_close_final(cw);
		return;
		}

	collection_close_dlg_show(cw);
}

void collection_window_close_by_collection(CollectionData *cd)
{
	CollectWindow *cw;

	cw = collection_window_find(cd);
	if (cw) collection_window_close_final(cw);
}

gboolean collection_window_modified_exists(void)
{
	GList *work;

	work = collection_window_list;
	while (work)
		{
		CollectWindow *cw = work->data;
		if (cw->cd->changed) return TRUE;
		work = work->next;
		}

	return FALSE;
}

static gboolean collection_window_delete(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	CollectWindow *cw = data;
	collection_window_close(cw);

	return TRUE;
}

CollectWindow *collection_window_new(const gchar *path)
{
	CollectWindow *cw;
	GtkWidget *vbox;
	GtkWidget *frame;
	GtkWidget *status_label;
	GtkWidget *extra_label;
	GdkGeometry geometry;

	cw = g_new0(CollectWindow, 1);

	collection_window_list = g_list_append(collection_window_list, cw);

	cw->cd = collection_new(path);

	cw->window = window_new(GTK_WINDOW_TOPLEVEL, "collection", PIXBUF_INLINE_ICON_BOOK, NULL, NULL);

	geometry.min_width = DEFAULT_MINIMAL_WINDOW_SIZE;
	geometry.min_height = DEFAULT_MINIMAL_WINDOW_SIZE;
	geometry.base_width = COLLECT_DEF_WIDTH;
	geometry.base_height = COLLECT_DEF_HEIGHT;
	gtk_window_set_geometry_hints(GTK_WINDOW(cw->window), NULL, &geometry,
				      GDK_HINT_MIN_SIZE | GDK_HINT_BASE_SIZE);


	if (options->save_window_positions && path && collection_load_only_geometry(cw->cd, path))
		{
		/* FIXME: x, y is not implemented */
		gtk_window_set_default_size(GTK_WINDOW(cw->window), cw->cd->window_w, cw->cd->window_h);
		}
	else
		{
		gtk_window_set_default_size(GTK_WINDOW(cw->window), COLLECT_DEF_WIDTH, COLLECT_DEF_HEIGHT);
		}

	gtk_window_set_resizable(GTK_WINDOW(cw->window), TRUE);
	collection_window_update_title(cw);
	gtk_container_set_border_width(GTK_CONTAINER(cw->window), 0);

	g_signal_connect(G_OBJECT(cw->window), "delete_event",
			 G_CALLBACK(collection_window_delete), cw);

	g_signal_connect(G_OBJECT(cw->window), "key_press_event",
			 G_CALLBACK(collection_window_keypress), cw);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(cw->window), vbox);
	gtk_widget_show(vbox);

	cw->table = collection_table_new(cw->cd);
	gtk_box_pack_start(GTK_BOX(vbox), cw->table->scrolled, TRUE, TRUE, 0);
	gtk_widget_show(cw->table->scrolled);

	cw->status_box = gtk_hbox_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), cw->status_box, FALSE, FALSE, 0);
	gtk_widget_show(cw->status_box);

	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(cw->status_box), frame, TRUE, TRUE, 0);
	gtk_widget_show(frame);

	status_label = gtk_label_new("");
	gtk_container_add(GTK_CONTAINER(frame), status_label);
	gtk_widget_show(status_label);

	extra_label = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(extra_label), 0.0);
	gtk_box_pack_start(GTK_BOX(cw->status_box), extra_label, TRUE, TRUE, 0);
	gtk_widget_show(extra_label);

	collection_table_set_labels(cw->table, status_label, extra_label);

	gtk_widget_show(cw->window);
	gtk_widget_grab_focus(cw->table->listview);

	collection_set_update_info_func(cw->cd, collection_window_update_info, cw);

	if (path && *path == G_DIR_SEPARATOR) collection_load_begin(cw->cd, NULL, COLLECTION_LOAD_NONE);

	return cw;
}
/* vim: set shiftwidth=8 softtabstop=0 cindent cinoptions={1s: */
