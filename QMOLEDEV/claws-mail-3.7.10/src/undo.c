/*
 * Sylpheed -- a GTK+ based, lightweight, and fast e-mail client
 * Copyright (C) 1999-2011 Hiroyuki Yamamoto and the Claws Mail team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/* code ported from gedit */
/* This is for my patient girlfirend Regina */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <glib.h>

#include <string.h> /* for strlen */
#include <stdlib.h> /* for mbstowcs */

#include "undo.h"
#include "utils.h"
#include "prefs_common.h"

typedef struct _UndoInfo UndoInfo;

struct _UndoInfo 
{
	UndoAction action;
	gchar *text;
	gint start_pos;
	gint end_pos;
	gfloat window_position;
	gint mergeable;
};

static void undo_free_list	(GList	       **list_pointer);
static void undo_check_size	(UndoMain	*undostruct);
static gint undo_merge		(GList		*list,
				 guint		 start_pos,
				 guint		 end_pos,
				 gint		 action,
				 const guchar	*text);
static void undo_add		(const gchar	*text,
				 gint		 start_pos,
				 gint		 end_pos,
				 UndoAction	 action,
				 UndoMain	*undostruct);
static gint undo_get_selection	(GtkTextView	*textview,
				 guint		*start,
				 guint		*end);
static void undo_insert_text_cb (GtkTextBuffer	*textbuf,
				 GtkTextIter	*iter,
				 gchar		*new_text,
				 gint		new_text_length,
				 UndoMain	*undostruct);
static void undo_delete_text_cb (GtkTextBuffer	*textbuf,
				 GtkTextIter	*start,
				 GtkTextIter	*end,
				 UndoMain	*undostruct);

static void undo_paste_clipboard_cb	(GtkTextView	*textview,
					 UndoMain	*undostruct);

void undo_undo			(UndoMain	*undostruct);
void undo_redo			(UndoMain	*undostruct);


UndoMain *undo_init(GtkWidget *text) 
{
	UndoMain *undostruct;
	GtkTextView *textview = GTK_TEXT_VIEW(text); 
	GtkTextBuffer *textbuf = gtk_text_view_get_buffer(textview);

	cm_return_val_if_fail(text != NULL, NULL);

	undostruct = g_new0(UndoMain, 1);
	undostruct->textview = textview;
	undostruct->undo = NULL;
	undostruct->redo = NULL;
	undostruct->paste = 0;
	undostruct->undo_state = FALSE;
	undostruct->redo_state = FALSE;

	g_signal_connect(G_OBJECT(textbuf), "insert-text",
			 G_CALLBACK(undo_insert_text_cb), undostruct);
	g_signal_connect(G_OBJECT(textbuf), "delete-range",
			 G_CALLBACK(undo_delete_text_cb), undostruct);
	g_signal_connect(G_OBJECT(textview), "paste-clipboard",
			 G_CALLBACK(undo_paste_clipboard_cb), undostruct);

	return undostruct;
}

void undo_destroy (UndoMain *undostruct) 
{
	undo_free_list(&undostruct->undo);
	undo_free_list(&undostruct->redo);
	g_free(undostruct);
}

static UndoInfo *undo_object_new(gchar *text, gint start_pos, gint end_pos, 
				 UndoAction action, gfloat window_position) 
{
	UndoInfo *undoinfo;
	undoinfo = g_new (UndoInfo, 1);
	undoinfo->text      = text;
	undoinfo->start_pos = start_pos;
	undoinfo->end_pos   = end_pos;
	undoinfo->action    = action;
	undoinfo->window_position = window_position;
	return undoinfo;
}

static void undo_object_free(UndoInfo *undo) 
{
	g_free (undo->text);
	g_free (undo);
}

/**
 * undo_free_list:
 * @list_pointer: list to be freed
 *
 * frees and undo structure list
 **/
static void undo_free_list(GList **list_pointer) 
{
	UndoInfo *undo;
	GList *cur, *list = *list_pointer;

	if (list == NULL) return;

	for (cur = list; cur != NULL; cur = cur->next) {
		undo = (UndoInfo *)cur->data;
		undo_object_free(undo);
	}

	g_list_free(list);
	*list_pointer = NULL;
}

void undo_set_change_state_func(UndoMain *undostruct, UndoChangeStateFunc func,
				gpointer data)
{
	cm_return_if_fail(undostruct != NULL);

	undostruct->change_state_func = func;
	undostruct->change_state_data = data;
}

/**
 * undo_check_size:
 * @compose: document to check
 *
 * Checks that the size of compose->undo does not excede settings->undo_levels and
 * frees any undo level above sett->undo_level.
 *
 **/
static void undo_check_size(UndoMain *undostruct) 
{
	UndoInfo *last_undo;
	guint length;

	if (prefs_common.undolevels < 1) return;

	/* No need to check for the redo list size since the undo
	   list gets freed on any call to compose_undo_add */
	length = g_list_length(undostruct->undo);
	if (length >= prefs_common.undolevels && prefs_common.undolevels > 0) {
		last_undo = (UndoInfo *)g_list_last(undostruct->undo)->data;
		undostruct->undo = g_list_remove(undostruct->undo, last_undo);
		undo_object_free(last_undo);
	}
}

/**
 * undo_merge:
 * @last_undo:
 * @start_pos:
 * @end_pos:
 * @action:
 *
 * This function tries to merge the undo object at the top of
 * the stack with a new set of data. So when we undo for example
 * typing, we can undo the whole word and not each letter by itself
 *
 * Return Value: TRUE is merge was sucessful, FALSE otherwise
 **/
static gint undo_merge(GList *list, guint start_pos, guint end_pos,
		       gint action, const guchar *text) 
{
	guchar *temp_string;
	UndoInfo *last_undo;

	/* This are the cases in which we will NOT merge :
	   1. if (last_undo->mergeable == FALSE)
	   [mergeable = FALSE when the size of the undo data was not 1.
	   or if the data was size = 1 but = '\n' or if the undo object
	   has been "undone" already ]
	   2. The size of text is not 1
	   3. If the new merging data is a '\n'
	   4. If the last char of the undo_last data is a space/tab
	   and the new char is not a space/tab ( so that we undo
	   words and not chars )
	   5. If the type (action) of undo is different from the last one
	   Chema */

	if (list == NULL) return FALSE;

	last_undo = list->data;

	if (!last_undo->mergeable) return FALSE;

	if (end_pos - start_pos != 1 ||
	    text[0] == '\n' ||
	    action != last_undo->action ||
	    action == UNDO_ACTION_REPLACE_INSERT ||
	    action == UNDO_ACTION_REPLACE_DELETE) {
		last_undo->mergeable = FALSE;
		return FALSE;
	}

	if (action == UNDO_ACTION_DELETE) {
		if (last_undo->start_pos != end_pos &&
		    last_undo->start_pos != start_pos) {
			last_undo->mergeable = FALSE;
			return FALSE;
		} else if (last_undo->start_pos == start_pos) {
			/* Deleted with the delete key */
			temp_string = g_strdup_printf("%s%s", last_undo->text, text);
			last_undo->end_pos++;
			g_free(last_undo->text);
			last_undo->text = temp_string;
		} else {
			/* Deleted with the backspace key */
			temp_string = g_strdup_printf("%s%s", text, last_undo->text);
			last_undo->start_pos = start_pos;
			g_free(last_undo->text);
			last_undo->text = temp_string;
		}
	} else if (action == UNDO_ACTION_INSERT) {
		if (last_undo->end_pos != start_pos) {
			last_undo->mergeable = FALSE;
			return FALSE;
		} else {
			temp_string = g_strdup_printf("%s%s", last_undo->text, text);
			g_free(last_undo->text);
			last_undo->end_pos = end_pos;
			last_undo->text = temp_string;
		}
	} else
		debug_print("Unknown action [%i] inside undo merge encountered", action);

	return TRUE;
}

/**
 * compose_undo_add:
 * @text:
 * @start_pos:
 * @end_pos:
 * @action: either UNDO_ACTION_INSERT or UNDO_ACTION_DELETE
 * @compose:
 * @view: The view so that we save the scroll bar position.
 *
 * Adds text to the undo stack. It also performs test to limit the number
 * of undo levels and deltes the redo list
 **/

static void undo_add(const gchar *text, 
		     gint start_pos, gint end_pos,
		     UndoAction action, UndoMain *undostruct) 
{
	UndoInfo *undoinfo;
	GtkAdjustment *vadj;

	cm_return_if_fail(text != NULL);
	cm_return_if_fail(end_pos >= start_pos);

	undo_free_list(&undostruct->redo);

	/* Set the redo sensitivity */
	undostruct->change_state_func(undostruct,
				      UNDO_STATE_UNCHANGED, UNDO_STATE_FALSE,
				      undostruct->change_state_data);

	if (undostruct->paste != 0) {
		if (action == UNDO_ACTION_INSERT) 
			action = UNDO_ACTION_REPLACE_INSERT;
		else 
			action = UNDO_ACTION_REPLACE_DELETE;
		undostruct->paste = undostruct->paste + 1;
		if (undostruct->paste == 3) 
			undostruct->paste = 0;
	}

	if (undo_merge(undostruct->undo, start_pos, end_pos, action, text))
		return;

	undo_check_size(undostruct);

	vadj = GTK_ADJUSTMENT(GTK_TEXT_VIEW(undostruct->textview)->vadjustment);
	undoinfo = undo_object_new(g_strdup(text), start_pos, end_pos, action,
				   vadj->value);

	if (end_pos - start_pos != 1 || text[0] == '\n')
		undoinfo->mergeable = FALSE;
	else
		undoinfo->mergeable = TRUE;

	undostruct->undo = g_list_prepend(undostruct->undo, undoinfo);

	undostruct->change_state_func(undostruct,
				      UNDO_STATE_TRUE, UNDO_STATE_UNCHANGED,
				      undostruct->change_state_data);
}

/**
 * undo_undo:
 * @w: not used
 * @data: not used
 *
 * Executes an undo request on the current document
 **/
void undo_undo(UndoMain *undostruct) 
{
	UndoInfo *undoinfo;
	GtkTextView *textview;
	GtkTextBuffer *buffer;
	GtkTextIter iter, start_iter, end_iter;
	GtkTextMark *mark;

	cm_return_if_fail(undostruct != NULL);

	if (undostruct->undo == NULL) return;

	/* The undo data we need is always at the top op the
	   stack. So, therefore, the first one */
	undoinfo = (UndoInfo *)undostruct->undo->data;
	cm_return_if_fail(undoinfo != NULL);
	undoinfo->mergeable = FALSE;
	undostruct->redo = g_list_prepend(undostruct->redo, undoinfo);
	undostruct->undo = g_list_remove(undostruct->undo, undoinfo);

	textview = undostruct->textview;
	buffer = gtk_text_view_get_buffer(textview);

	undo_block(undostruct);

	/* Check if there is a selection active */
	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	gtk_text_buffer_place_cursor(buffer, &iter);

	/* Move the view (scrollbars) to the correct position */
	gtk_adjustment_set_value
		(GTK_ADJUSTMENT(textview->vadjustment),
		 undoinfo->window_position);
	
	switch (undoinfo->action) {
	case UNDO_ACTION_DELETE:
		gtk_text_buffer_get_iter_at_offset(buffer, &iter, undoinfo->start_pos);
		gtk_text_buffer_insert(buffer, &iter, undoinfo->text, -1);
		break;
	case UNDO_ACTION_INSERT:
		gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, undoinfo->start_pos);
		gtk_text_buffer_get_iter_at_offset(buffer, &end_iter, undoinfo->end_pos);
		gtk_text_buffer_delete(buffer, &start_iter, &end_iter);
		break;
	case UNDO_ACTION_REPLACE_INSERT:
		gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, undoinfo->start_pos);
		gtk_text_buffer_get_iter_at_offset(buffer, &end_iter, undoinfo->end_pos);
		gtk_text_buffer_delete(buffer, &start_iter, &end_iter);
		/* "pull" another data structure from the list */
		if (undostruct->undo){
			undoinfo = (UndoInfo *)undostruct->undo->data;
			undostruct->redo = g_list_prepend(undostruct->redo, undoinfo);
			undostruct->undo = g_list_remove(undostruct->undo, undoinfo);
			cm_return_if_fail(undoinfo != NULL);
			cm_return_if_fail(undoinfo->action == UNDO_ACTION_REPLACE_DELETE);
			gtk_text_buffer_insert(buffer, &start_iter, undoinfo->text, -1);
		}
		break;
	case UNDO_ACTION_REPLACE_DELETE:
		g_warning("This should not happen. UNDO_REPLACE_DELETE");
		break;
	default:
		g_assert_not_reached();
		break;
	}
	
	undostruct->change_state_func(undostruct,
				      UNDO_STATE_UNCHANGED, UNDO_STATE_TRUE,
				      undostruct->change_state_data);

	if (undostruct->undo == NULL)
		undostruct->change_state_func(undostruct,
					      UNDO_STATE_FALSE,
					      UNDO_STATE_UNCHANGED,
					      undostruct->change_state_data);

	undo_unblock(undostruct);
}

/**
 * undo_redo:
 * @w: not used
 * @data: not used
 *
 * executes a redo request on the current document
 **/
void undo_redo(UndoMain *undostruct) 
{
	UndoInfo *redoinfo;
	GtkTextView *textview;
	GtkTextBuffer *buffer;
	GtkTextIter iter, start_iter, end_iter;
	GtkTextMark *mark;

	cm_return_if_fail(undostruct != NULL);

	if (undostruct->redo == NULL) return;

	redoinfo = (UndoInfo *)undostruct->redo->data;
	cm_return_if_fail (redoinfo != NULL);
	undostruct->undo = g_list_prepend(undostruct->undo, redoinfo);
	undostruct->redo = g_list_remove(undostruct->redo, redoinfo);

	textview = undostruct->textview;
	buffer = gtk_text_view_get_buffer(textview);

	undo_block(undostruct);

	/* Check if there is a selection active */
	mark = gtk_text_buffer_get_insert(buffer);
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
	gtk_text_buffer_place_cursor(buffer, &iter);

	/* Move the view to the right position. */
	gtk_adjustment_set_value(textview->vadjustment, 
				 redoinfo->window_position);

	switch (redoinfo->action) {
	case UNDO_ACTION_INSERT:
		gtk_text_buffer_get_iter_at_offset(buffer, &iter, redoinfo->start_pos);
		gtk_text_buffer_insert(buffer, &iter, redoinfo->text, -1);
		break;
	case UNDO_ACTION_DELETE:
		gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, redoinfo->start_pos);
		gtk_text_buffer_get_iter_at_offset(buffer, &end_iter, redoinfo->end_pos);
		gtk_text_buffer_delete(buffer, &start_iter, &end_iter);
		break;
	case UNDO_ACTION_REPLACE_DELETE:
		gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, redoinfo->start_pos);
		gtk_text_buffer_get_iter_at_offset(buffer, &end_iter, redoinfo->end_pos);
		gtk_text_buffer_delete(buffer, &start_iter, &end_iter);
		debug_print("UNDO_ACTION_REPLACE %s\n", redoinfo->text);
		/* "pull" another data structure from the list */
		redoinfo = (UndoInfo *)undostruct->redo->data;
		cm_return_if_fail(redoinfo != NULL);
		undostruct->undo = g_list_prepend(undostruct->undo, redoinfo);
		undostruct->redo = g_list_remove(undostruct->redo, redoinfo);
		cm_return_if_fail(redoinfo->action == UNDO_ACTION_REPLACE_INSERT);
		gtk_text_buffer_insert(buffer, &start_iter, redoinfo->text, -1);
		break;
	case UNDO_ACTION_REPLACE_INSERT:
		/* This is needed only if we redo from a middle-click button */
		gtk_text_buffer_get_iter_at_offset(buffer, &iter, redoinfo->start_pos);
		gtk_text_buffer_insert(buffer, &iter, redoinfo->text, -1);
		break;
	default:
		g_assert_not_reached();
		break;
	}

	undostruct->change_state_func(undostruct,
				      UNDO_STATE_TRUE, UNDO_STATE_UNCHANGED, 
				      undostruct->change_state_data);

	if (undostruct->redo == NULL)
		undostruct->change_state_func(undostruct,
					      UNDO_STATE_UNCHANGED,
					      UNDO_STATE_FALSE,
					      undostruct->change_state_data);

	undo_unblock(undostruct);
}

void undo_block(UndoMain *undostruct)
{
	GtkTextBuffer *buffer;

	cm_return_if_fail(GTK_IS_TEXT_VIEW(undostruct->textview));

	buffer = gtk_text_view_get_buffer(undostruct->textview);
	g_signal_handlers_block_by_func(buffer, undo_insert_text_cb, undostruct);
	g_signal_handlers_block_by_func(buffer, undo_delete_text_cb, undostruct);
	g_signal_handlers_block_by_func(buffer, undo_paste_clipboard_cb,
					  undostruct);
}

void undo_unblock(UndoMain *undostruct)
{
	GtkTextBuffer *buffer;

	cm_return_if_fail(GTK_IS_TEXT_VIEW(undostruct->textview));

	buffer = gtk_text_view_get_buffer(undostruct->textview);
	g_signal_handlers_unblock_by_func(buffer, undo_insert_text_cb, undostruct);
	g_signal_handlers_unblock_by_func(buffer, undo_delete_text_cb, undostruct);
	g_signal_handlers_unblock_by_func(buffer, undo_paste_clipboard_cb,
					  undostruct);
}

void undo_wrapping(UndoMain *undostruct, gboolean wrap)
{
//	debug_print("undo wrapping now %d\n", wrap);
	undostruct->wrap = wrap;
}

void undo_insert_text_cb(GtkTextBuffer *textbuf, GtkTextIter *iter,
			 gchar *new_text, gint new_text_length,
			 UndoMain *undostruct) 
{
	gchar *text_to_insert;
	gint pos;
	if (prefs_common.undolevels <= 0) return;

	pos = gtk_text_iter_get_offset(iter);
	if (undostruct->wrap && undostruct->undo) {
		UndoInfo *last_undo = undostruct->undo->data;
		if (last_undo && (last_undo->action == UNDO_ACTION_INSERT
				  || last_undo->action == UNDO_ACTION_REPLACE_INSERT)
		&&  last_undo->start_pos < pos && last_undo->end_pos > pos) {
			GtkTextIter start,end;
			last_undo->end_pos += g_utf8_strlen(new_text, -1);
			gtk_text_buffer_get_iter_at_offset(textbuf, &start, last_undo->start_pos);
			gtk_text_buffer_get_iter_at_offset(textbuf, &end, last_undo->end_pos);
			g_free(last_undo->text);
			last_undo->text = gtk_text_buffer_get_text(textbuf, &start, &end, FALSE);
			debug_print("add:undo upd %d-%d\n", last_undo->start_pos, last_undo->end_pos);
			return;
		} else if (last_undo)
			debug_print("add:last: %d, %d-%d (%d)\n", last_undo->action,
				last_undo->start_pos, last_undo->end_pos, pos);
	} 
	Xstrndup_a(text_to_insert, new_text, new_text_length, return);
	debug_print("add:undo add %d-%ld\n", pos, pos + g_utf8_strlen(text_to_insert, -1));
	undo_add(text_to_insert, pos, pos + g_utf8_strlen(text_to_insert, -1),
		 UNDO_ACTION_INSERT, undostruct);
}

void undo_delete_text_cb(GtkTextBuffer *textbuf, GtkTextIter *start,
			 GtkTextIter *end, UndoMain *undostruct) 
{
	gchar *text_to_delete;
	gint start_pos, end_pos;

	if (prefs_common.undolevels <= 0) return;

	text_to_delete = gtk_text_buffer_get_text(textbuf, start, end, FALSE);
	if (!text_to_delete || !*text_to_delete) return;

	start_pos = gtk_text_iter_get_offset(start);
	end_pos   = gtk_text_iter_get_offset(end);

	if (undostruct->wrap && undostruct->undo) {
		UndoInfo *last_undo = undostruct->undo->data;
		if (last_undo && (last_undo->action == UNDO_ACTION_INSERT
				  || last_undo->action == UNDO_ACTION_REPLACE_INSERT)
		&&  last_undo->start_pos < start_pos && last_undo->end_pos > end_pos) {
			GtkTextIter start,end;
			last_undo->end_pos -= g_utf8_strlen(text_to_delete, -1);
			gtk_text_buffer_get_iter_at_offset(textbuf, &start, last_undo->start_pos);
			gtk_text_buffer_get_iter_at_offset(textbuf, &end, last_undo->end_pos);
			g_free(last_undo->text);
			last_undo->text = gtk_text_buffer_get_text(textbuf, &start, &end, FALSE);
			debug_print("del:undo upd %d-%d\n", last_undo->start_pos, last_undo->end_pos);
			return;
		} else if (last_undo)
			debug_print("del:last: %d, %d-%d (%d)\n", last_undo->action,
				last_undo->start_pos, last_undo->end_pos, start_pos);
		
	} 
	debug_print("del:undo add %d-%d\n", start_pos, end_pos);
	undo_add(text_to_delete, start_pos, end_pos, UNDO_ACTION_DELETE,
		 undostruct);
	g_free(text_to_delete);
}

void undo_paste_clipboard(GtkTextView *textview, UndoMain *undostruct)
{
	undo_paste_clipboard_cb(textview, undostruct);
}

static void undo_paste_clipboard_cb(GtkTextView *textview, UndoMain *undostruct)
{
	if (prefs_common.undolevels > 0)
		if (undo_get_selection(textview, NULL, NULL))
			undostruct->paste = TRUE;
}

/**
 * undo_get_selection:
 * @text: Text to get the selection from
 * @start: return here the start position of the selection
 * @end: return here the end position of the selection
 *
 * Gets the current selection for View
 *
 * Return Value: TRUE if there is a selection active, FALSE if not
 **/
static gint undo_get_selection(GtkTextView *textview, guint *start, guint *end) 
{
	GtkTextBuffer *buffer;
	GtkTextIter start_iter, end_iter;
	guint start_pos, end_pos;

	buffer = gtk_text_view_get_buffer(textview);
	gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);

	start_pos = gtk_text_iter_get_offset(&start_iter);
	end_pos   = gtk_text_iter_get_offset(&end_iter);

	/* The user can select from end to start too. If so, swap it*/
	if (end_pos < start_pos) {
		guint swap_pos;
		swap_pos  = end_pos;
		end_pos   = start_pos;
		start_pos = swap_pos;
	}

	if (start != NULL)
		*start = start_pos;
		
	if (end != NULL)
		*end = end_pos;

	if ((start_pos > 0 || end_pos > 0) && (start_pos != end_pos))
		return TRUE;
	else
		return FALSE;
}
