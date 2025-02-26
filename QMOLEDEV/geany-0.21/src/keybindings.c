/*
 *      keybindings.c - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2006-2011 Enrico Tröger <enrico(dot)troeger(at)uvena(dot)de>
 *      Copyright 2006-2011 Nick Treleaven <nick(dot)treleaven(at)btinternet(dot)com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $Id: keybindings.c 5895 2011-08-21 17:15:19Z colombanw $
 */

/**
 * @file keybindings.h
 * Configurable keyboard shortcuts.
 * - keybindings_send_command() mimics a built-in keybinding action.
 * - @ref GeanyKeyGroupID lists groups of built-in keybindings.
 * @see plugin_set_key_group().
 **/


#include "geany.h"

#include <gdk/gdkkeysyms.h>
#include <string.h>

#include "keybindings.h"
#include "support.h"
#include "utils.h"
#include "ui_utils.h"
#include "document.h"
#include "documentprivate.h"
#include "filetypes.h"
#include "callbacks.h"
#include "prefs.h"
#include "msgwindow.h"
#include "editor.h"
#include "sciwrappers.h"
#include "build.h"
#include "tools.h"
#include "navqueue.h"
#include "symbols.h"
#include "vte.h"
#include "toolbar.h"
#include "sidebar.h"
#include "geanywraplabel.h"
#include "main.h"
#include "search.h"
#ifdef HAVE_VTE
# include "vte.h"
#endif


GPtrArray *keybinding_groups;	/* array of GeanyKeyGroup pointers */

/* keyfile group name for non-plugin KB groups */
static const gchar keybindings_keyfile_group_name[] = "Bindings";

/* core keybindings */
static GeanyKeyBinding binding_ids[GEANY_KEYS_COUNT];

static GtkAccelGroup *kb_accel_group = NULL;
static const gboolean swap_alt_tab_order = FALSE;

static const gsize MAX_MRU_DOCS = 20;
static GQueue *mru_docs = NULL;
static guint mru_pos = 0;

static gboolean switch_in_progress = FALSE;
static GtkWidget *switch_dialog = NULL;
static GtkWidget *switch_dialog_label = NULL;


/* central keypress event handler, almost all keypress events go to this function */
static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
static gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

static gboolean check_current_word(GeanyDocument *doc, gboolean sci_word);
static gboolean read_current_word(GeanyDocument *doc, gboolean sci_word);
static gchar *get_current_word_or_sel(GeanyDocument *doc, gboolean sci_word);

static gboolean cb_func_file_action(guint key_id);
static gboolean cb_func_project_action(guint key_id);
static gboolean cb_func_editor_action(guint key_id);
static gboolean cb_func_select_action(guint key_id);
static gboolean cb_func_format_action(guint key_id);
static gboolean cb_func_insert_action(guint key_id);
static gboolean cb_func_search_action(guint key_id);
static gboolean cb_func_goto_action(guint key_id);
static gboolean cb_func_switch_action(guint key_id);
static gboolean cb_func_clipboard_action(guint key_id);
static gboolean cb_func_build_action(guint key_id);
static gboolean cb_func_document_action(guint key_id);
static gboolean cb_func_view_action(guint key_id);

/* note: new keybindings should normally use per group callbacks */
static void cb_func_menu_help(guint key_id);
static void cb_func_menu_preferences(guint key_id);

static void cb_func_menu_fullscreen(guint key_id);
static void cb_func_menu_messagewindow(guint key_id);

static void cb_func_menu_opencolorchooser(guint key_id);

static void cb_func_switch_tableft(guint key_id);
static void cb_func_switch_tabright(guint key_id);
static void cb_func_switch_tablastused(guint key_id);
static void cb_func_move_tab(guint key_id);

static void add_popup_menu_accels(void);


/** Looks up a keybinding item.
 * @param group Group.
 * @param key_id Keybinding index for the group.
 * @return The keybinding.
 * @since 0.19. */
GeanyKeyBinding *keybindings_get_item(GeanyKeyGroup *group, gsize key_id)
{
	if (group->plugin)
	{
		g_assert(key_id < group->plugin_key_count);
		return &group->plugin_keys[key_id];
	}
	g_assert(key_id < GEANY_KEYS_COUNT);
	return &binding_ids[key_id];
}


/* This is used to set default keybindings on startup.
 * Menu accels are set in apply_kb_accel(). */
/** Fills a GeanyKeyBinding struct item.
 * @param group Group.
 * @param key_id Keybinding index for the group.
 * @param callback Function to call when activated, or @c NULL to use the group callback.
 * Usually it's better to use the group callback instead - see plugin_set_key_group().
 * @param key (Lower case) default key, e.g. @c GDK_j, but usually 0 for unset.
 * @param mod Default modifier, e.g. @c GDK_CONTROL_MASK, but usually 0 for unset.
 * @param kf_name Key name for the configuration file, such as @c "menu_new".
 * @param label Label used in the preferences dialog keybindings tab. May contain
 * underscores - these won't be displayed.
 * @param menu_item Optional widget to set an accelerator for, or @c NULL.
 * @return The keybinding - normally this is ignored. */
GeanyKeyBinding *keybindings_set_item(GeanyKeyGroup *group, gsize key_id,
		GeanyKeyCallback callback, guint key, GdkModifierType mod,
		const gchar *kf_name, const gchar *label, GtkWidget *menu_item)
{
	GeanyKeyBinding *kb;

	g_assert(group->name);
	kb = keybindings_get_item(group, key_id);
	g_assert(!kb->name);
	g_ptr_array_add(group->key_items, kb);

	if (group->plugin)
	{
		/* some plugins e.g. GeanyLua need these fields duplicated */
		setptr(kb->name, g_strdup(kf_name));
		setptr(kb->label, g_strdup(label));
	}
	else
	{
		/* we don't touch these strings unless group->plugin is set, const cast is safe */
		kb->name = (gchar *)kf_name;
		kb->label = (gchar *)label;
	}
	kb->key = key;
	kb->mods = mod;
	kb->callback = callback;
	kb->menu_item = menu_item;
	kb->id = key_id;
	return kb;
}


static GeanyKeyGroup *add_kb_group(GeanyKeyGroup *group,
		const gchar *name, const gchar *label, GeanyKeyGroupCallback callback, gboolean plugin)
{
	g_ptr_array_add(keybinding_groups, group);

	group->name = name;
	group->label = label;
	group->callback = callback;
	group->plugin = plugin;
	group->key_items = g_ptr_array_new();
	return group;
}


/* Lookup a widget in the main window */
#define LW(widget_name) \
	ui_lookup_widget(main_widgets.window, G_STRINGIFY(widget_name))

/* Expansion for group_id = FILE:
 * add_kb_group(&groups[GEANY_KEY_GROUP_FILE],
 * 	keybindings_keyfile_group_name, _("File menu"), callback); */
#define ADD_KB_GROUP(group_id, label, callback) \
	add_kb_group(&groups[GEANY_KEY_GROUP_ ## group_id],\
		keybindings_keyfile_group_name, label, callback, FALSE)

/* Init all fields of keys with default values.
 * The menu_item field is always the main menu item, popup menu accelerators are
 * set in add_popup_menu_accels(). */
static void init_default_kb(void)
{
	static GeanyKeyGroup groups[GEANY_KEY_GROUP_COUNT];
	GeanyKeyGroup *group;

	group = ADD_KB_GROUP(FILE, _("File"), cb_func_file_action);

	keybindings_set_item(group, GEANY_KEYS_FILE_NEW, NULL,
		GDK_n, GDK_CONTROL_MASK, "menu_new", _("New"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FILE_OPEN, NULL,
		GDK_o, GDK_CONTROL_MASK, "menu_open", _("Open"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FILE_OPENSELECTED, NULL,
		GDK_o, GDK_SHIFT_MASK | GDK_CONTROL_MASK, "menu_open_selected",
		_("Open selected file"), LW(menu_open_selected_file1));
	keybindings_set_item(group, GEANY_KEYS_FILE_SAVE, NULL,
		GDK_s, GDK_CONTROL_MASK, "menu_save", _("Save"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FILE_SAVEAS, NULL,
		0, 0, "menu_saveas", _("Save as"), LW(menu_save_as1));
	keybindings_set_item(group, GEANY_KEYS_FILE_SAVEALL, NULL,
		GDK_S, GDK_SHIFT_MASK | GDK_CONTROL_MASK, "menu_saveall", _("Save all"),
		LW(menu_save_all1));
	keybindings_set_item(group, GEANY_KEYS_FILE_PRINT, NULL,
		GDK_p, GDK_CONTROL_MASK, "menu_print", _("Print"), LW(print1));
	keybindings_set_item(group, GEANY_KEYS_FILE_CLOSE, NULL,
		GDK_w, GDK_CONTROL_MASK, "menu_close", _("Close"), LW(menu_close1));
	keybindings_set_item(group, GEANY_KEYS_FILE_CLOSEALL, NULL,
		GDK_w, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "menu_closeall", _("Close all"),
		LW(menu_close_all1));
	keybindings_set_item(group, GEANY_KEYS_FILE_RELOAD, NULL,
		GDK_r, GDK_CONTROL_MASK, "menu_reloadfile", _("Reload file"), LW(menu_reload1));
	keybindings_set_item(group, GEANY_KEYS_FILE_OPENLASTTAB, NULL,
		0, 0, "file_openlasttab", _("Re-open last closed tab"), NULL);

	group = ADD_KB_GROUP(PROJECT, _("Project"), cb_func_project_action);

	keybindings_set_item(group, GEANY_KEYS_PROJECT_PROPERTIES, NULL,
		0, 0, "project_properties", _("Project properties"), LW(project_properties1));

	group = ADD_KB_GROUP(EDITOR, _("Editor"), cb_func_editor_action);

	keybindings_set_item(group, GEANY_KEYS_EDITOR_UNDO, NULL,
		GDK_z, GDK_CONTROL_MASK, "menu_undo", _("Undo"), LW(menu_undo2));
	keybindings_set_item(group, GEANY_KEYS_EDITOR_REDO, NULL,
		GDK_y, GDK_CONTROL_MASK, "menu_redo", _("Redo"), LW(menu_redo2));
	keybindings_set_item(group, GEANY_KEYS_EDITOR_DUPLICATELINE, NULL,
		GDK_d, GDK_CONTROL_MASK, "edit_duplicateline", _("_Duplicate Line or Selection"),
		LW(duplicate_line_or_selection1));
	keybindings_set_item(group, GEANY_KEYS_EDITOR_DELETELINE, NULL,
		GDK_k, GDK_CONTROL_MASK, "edit_deleteline", _("_Delete Current Line(s)"),
		LW(delete_current_line_s_1));
	keybindings_set_item(group, GEANY_KEYS_EDITOR_DELETELINETOEND, NULL,
		GDK_Delete, GDK_SHIFT_MASK | GDK_CONTROL_MASK, "edit_deletelinetoend",
		_("Delete to line end"), NULL);
	/* transpose may fit better in format group */
	keybindings_set_item(group, GEANY_KEYS_EDITOR_TRANSPOSELINE, NULL,
		GDK_t, GDK_CONTROL_MASK, "edit_transposeline", _("_Transpose Current Line"),
		LW(transpose_current_line1));
	keybindings_set_item(group, GEANY_KEYS_EDITOR_SCROLLTOLINE, NULL,
		GDK_l, GDK_SHIFT_MASK | GDK_CONTROL_MASK, "edit_scrolltoline", _("Scroll to current line"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_SCROLLLINEUP, NULL,
		GDK_Up, GDK_MOD1_MASK, "edit_scrolllineup", _("Scroll up the view by one line"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_SCROLLLINEDOWN, NULL,
		GDK_Down, GDK_MOD1_MASK, "edit_scrolllinedown", _("Scroll down the view by one line"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_COMPLETESNIPPET, NULL,
		GDK_Tab, 0, "edit_completesnippet", _("Complete snippet"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_SNIPPETNEXTCURSOR, NULL,
		0, 0, "move_snippetnextcursor", _("Move cursor in snippet"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_SUPPRESSSNIPPETCOMPLETION, NULL,
		0, 0, "edit_suppresssnippetcompletion", _("Suppress snippet completion"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_CONTEXTACTION, NULL,
		0, 0, "popup_contextaction", _("Context Action"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_AUTOCOMPLETE, NULL,
		GDK_space, GDK_CONTROL_MASK, "edit_autocomplete", _("Complete word"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_CALLTIP, NULL,
		GDK_space, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "edit_calltip", _("Show calltip"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_MACROLIST, NULL,
		GDK_Return, GDK_CONTROL_MASK, "edit_macrolist", _("Show macro list"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_WORDPARTCOMPLETION, NULL,
		GDK_Tab, 0, "edit_wordpartcompletion", _("Word part completion"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_MOVELINEUP, NULL,
		0, 0, "edit_movelineup", _("Move line(s) up"), NULL);
	keybindings_set_item(group, GEANY_KEYS_EDITOR_MOVELINEDOWN, NULL,
		0, 0, "edit_movelinedown", _("Move line(s) down"), NULL);

	group = ADD_KB_GROUP(CLIPBOARD, _("Clipboard"), cb_func_clipboard_action);

	keybindings_set_item(group, GEANY_KEYS_CLIPBOARD_CUT, NULL,
		GDK_x, GDK_CONTROL_MASK, "menu_cut", _("Cut"), NULL);
	keybindings_set_item(group, GEANY_KEYS_CLIPBOARD_COPY, NULL,
		GDK_c, GDK_CONTROL_MASK, "menu_copy", _("Copy"), NULL);
	keybindings_set_item(group, GEANY_KEYS_CLIPBOARD_PASTE, NULL,
		GDK_v, GDK_CONTROL_MASK, "menu_paste", _("Paste"), NULL);
	keybindings_set_item(group, GEANY_KEYS_CLIPBOARD_COPYLINE, NULL,
		GDK_c, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "edit_copyline", _("_Copy Current Line(s)"),
		LW(copy_current_line_s_1));
	keybindings_set_item(group, GEANY_KEYS_CLIPBOARD_CUTLINE, NULL,
		GDK_x, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "edit_cutline", _("_Cut Current Line(s)"),
		LW(cut_current_line_s_1));

	group = ADD_KB_GROUP(SELECT, _("Select"), cb_func_select_action);

	keybindings_set_item(group, GEANY_KEYS_SELECT_ALL, NULL,
		GDK_a, GDK_CONTROL_MASK, "menu_selectall", _("Select All"), LW(menu_select_all1));
	keybindings_set_item(group, GEANY_KEYS_SELECT_WORD, NULL,
		GDK_w, GDK_SHIFT_MASK | GDK_MOD1_MASK, "edit_selectword", _("Select current word"), NULL);
	keybindings_set_item(group, GEANY_KEYS_SELECT_LINE, NULL,
		GDK_l, GDK_SHIFT_MASK | GDK_MOD1_MASK, "edit_selectline", _("_Select Current Line(s)"),
		LW(select_current_line_s_1));
	keybindings_set_item(group, GEANY_KEYS_SELECT_PARAGRAPH, NULL,
		GDK_p, GDK_SHIFT_MASK | GDK_MOD1_MASK, "edit_selectparagraph", _("_Select Current Paragraph"),
		LW(select_current_paragraph1));
	keybindings_set_item(group, GEANY_KEYS_SELECT_WORDPARTLEFT, NULL,
		0, 0, "edit_selectwordpartleft", _("Select to previous word part"), NULL);
	keybindings_set_item(group, GEANY_KEYS_SELECT_WORDPARTRIGHT, NULL,
		0, 0, "edit_selectwordpartright", _("Select to next word part"), NULL);

	group = ADD_KB_GROUP(FORMAT, _("Format"), cb_func_format_action);

	keybindings_set_item(group, GEANY_KEYS_FORMAT_TOGGLECASE, NULL,
		GDK_u, GDK_CONTROL_MASK | GDK_MOD1_MASK, "edit_togglecase",
		_("T_oggle Case of Selection"), LW(menu_toggle_case2));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_COMMENTLINETOGGLE, NULL,
		GDK_e, GDK_CONTROL_MASK, "edit_commentlinetoggle", _("Toggle line commentation"),
		LW(menu_toggle_line_commentation1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_COMMENTLINE, NULL,
		0, 0, "edit_commentline", _("Comment line(s)"), LW(menu_comment_line1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_UNCOMMENTLINE, NULL,
		0, 0, "edit_uncommentline", _("Uncomment line(s)"), LW(menu_uncomment_line1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_INCREASEINDENT, NULL,
		GDK_i, GDK_CONTROL_MASK, "edit_increaseindent", _("Increase indent"),
		LW(menu_increase_indent1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_DECREASEINDENT, NULL,
		GDK_u, GDK_CONTROL_MASK, "edit_decreaseindent", _("Decrease indent"),
		LW(menu_decrease_indent1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_INCREASEINDENTBYSPACE, NULL,
		0, 0, "edit_increaseindentbyspace", _("Increase indent by one space"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FORMAT_DECREASEINDENTBYSPACE, NULL,
		0, 0, "edit_decreaseindentbyspace", _("Decrease indent by one space"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FORMAT_AUTOINDENT, NULL,
		0, 0, "edit_autoindent", _("_Smart Line Indent"), LW(smart_line_indent1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_SENDTOCMD1, NULL,
		GDK_1, GDK_CONTROL_MASK, "edit_sendtocmd1", _("Send to Custom Command 1"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FORMAT_SENDTOCMD2, NULL,
		GDK_2, GDK_CONTROL_MASK, "edit_sendtocmd2", _("Send to Custom Command 2"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FORMAT_SENDTOCMD3, NULL,
		GDK_3, GDK_CONTROL_MASK, "edit_sendtocmd3", _("Send to Custom Command 3"), NULL);
	/* may fit better in editor group */
	keybindings_set_item(group, GEANY_KEYS_FORMAT_SENDTOVTE, NULL,
		0, 0, "edit_sendtovte", _("_Send Selection to Terminal"), LW(send_selection_to_vte1));
	keybindings_set_item(group, GEANY_KEYS_FORMAT_REFLOWPARAGRAPH, NULL,
		GDK_j, GDK_CONTROL_MASK, "format_reflowparagraph", _("_Reflow Lines/Block"),
		LW(reflow_lines_block1));

	group = ADD_KB_GROUP(INSERT, _("Insert"), cb_func_insert_action);

	keybindings_set_item(group, GEANY_KEYS_INSERT_DATE, NULL,
		GDK_d, GDK_SHIFT_MASK | GDK_MOD1_MASK, "menu_insert_date", _("Insert date"),
		LW(insert_date_custom1));
	keybindings_set_item(group, GEANY_KEYS_INSERT_ALTWHITESPACE, NULL,
		0, 0, "edit_insertwhitespace", _("_Insert Alternative White Space"),
		LW(insert_alternative_white_space1));
	keybindings_set_item(group, GEANY_KEYS_INSERT_LINEBEFORE, NULL,
		0, 0, "edit_insertlinebefore", _("Insert New Line Before Current"), NULL);
	keybindings_set_item(group, GEANY_KEYS_INSERT_LINEAFTER, NULL,
		0, 0, "edit_insertlineafter", _("Insert New Line After Current"), NULL);

	group = ADD_KB_GROUP(SETTINGS, _("Settings"), NULL);

	keybindings_set_item(group, GEANY_KEYS_SETTINGS_PREFERENCES, cb_func_menu_preferences,
		GDK_p, GDK_CONTROL_MASK | GDK_MOD1_MASK, "menu_preferences", _("Preferences"),
		LW(preferences1));
	keybindings_set_item(group, GEANY_KEYS_SETTINGS_PLUGINPREFERENCES, cb_func_menu_preferences,
		0, 0, "menu_pluginpreferences", _("P_lugin Preferences"), LW(plugin_preferences1));

	group = ADD_KB_GROUP(SEARCH, _("Search"), cb_func_search_action);

	keybindings_set_item(group, GEANY_KEYS_SEARCH_FIND, NULL,
		GDK_f, GDK_CONTROL_MASK, "menu_find", _("Find"), LW(find1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDNEXT, NULL,
		GDK_g, GDK_CONTROL_MASK, "menu_findnext", _("Find Next"), LW(find_next1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDPREVIOUS, NULL,
		GDK_g, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "menu_findprevious", _("Find Previous"),
		LW(find_previous1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDNEXTSEL, NULL,
		0, 0, "menu_findnextsel", _("Find Next _Selection"), LW(find_nextsel1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDPREVSEL, NULL,
		0, 0, "menu_findprevsel", _("Find Pre_vious Selection"), LW(find_prevsel1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_REPLACE, NULL,
		GDK_h, GDK_CONTROL_MASK, "menu_replace", _("Replace"), LW(replace1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDINFILES, NULL, GDK_f,
		GDK_CONTROL_MASK | GDK_SHIFT_MASK, "menu_findinfiles", _("Find in Files"),
		LW(find_in_files1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_NEXTMESSAGE, NULL,
		0, 0, "menu_nextmessage", _("Next Message"), LW(next_message1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_PREVIOUSMESSAGE, NULL,
		0, 0, "menu_previousmessage", _("Previous Message"), LW(previous_message1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDUSAGE, NULL,
		0, 0, "popup_findusage", _("Find Usage"), LW(find_usage1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_FINDDOCUMENTUSAGE, NULL,
		0, 0, "popup_finddocumentusage", _("Find Document Usage"), LW(find_document_usage1));
	keybindings_set_item(group, GEANY_KEYS_SEARCH_MARKALL, NULL,
		GDK_m, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "find_markall", _("_Mark All"), LW(mark_all1));

	group = ADD_KB_GROUP(GOTO, _("Go to"), cb_func_goto_action);

	keybindings_set_item(group, GEANY_KEYS_GOTO_BACK, NULL,
		0, 0, "nav_back", _("Navigate back a location"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_FORWARD, NULL,
		0, 0, "nav_forward", _("Navigate forward a location"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_LINE, NULL,
		GDK_l, GDK_CONTROL_MASK, "menu_gotoline", _("Go to Line"), LW(go_to_line1));
	keybindings_set_item(group, GEANY_KEYS_GOTO_MATCHINGBRACE, NULL,
		GDK_b, GDK_CONTROL_MASK, "edit_gotomatchingbrace",
		_("Go to matching brace"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_TOGGLEMARKER, NULL,
		GDK_m, GDK_CONTROL_MASK, "edit_togglemarker",
		_("Toggle marker"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_NEXTMARKER, NULL,
		GDK_period, GDK_CONTROL_MASK, "edit_gotonextmarker",
		_("_Go to Next Marker"), LW(go_to_next_marker1));
	keybindings_set_item(group, GEANY_KEYS_GOTO_PREVIOUSMARKER, NULL,
		GDK_comma, GDK_CONTROL_MASK, "edit_gotopreviousmarker",
		_("_Go to Previous Marker"), LW(go_to_previous_marker1));
	keybindings_set_item(group, GEANY_KEYS_GOTO_TAGDEFINITION, NULL,
		0, 0, "popup_gototagdefinition", _("Go to Tag Definition"), LW(goto_tag_definition1));
	keybindings_set_item(group, GEANY_KEYS_GOTO_TAGDECLARATION, NULL,
		0, 0, "popup_gototagdeclaration", _("Go to Tag Declaration"), LW(goto_tag_declaration1));
	keybindings_set_item(group, GEANY_KEYS_GOTO_LINESTART, NULL,
		GDK_Home, 0, "edit_gotolinestart", _("Go to Start of Line"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_LINEEND, NULL,
		GDK_End, 0, "edit_gotolineend", _("Go to End of Line"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_LINEENDVISUAL, NULL,
		GDK_End, GDK_MOD1_MASK, "edit_gotolineendvisual", _("Go to End of Display Line"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_PREVWORDPART, NULL,
		GDK_slash, GDK_CONTROL_MASK, "edit_prevwordstart", _("Go to Previous Word Part"), NULL);
	keybindings_set_item(group, GEANY_KEYS_GOTO_NEXTWORDPART, NULL,
		GDK_backslash, GDK_CONTROL_MASK, "edit_nextwordstart", _("Go to Next Word Part"), NULL);

	group = ADD_KB_GROUP(VIEW, _("View"), cb_func_view_action);

	keybindings_set_item(group, GEANY_KEYS_VIEW_TOGGLEALL, NULL,
		0, 0, "menu_toggleall", _("Toggle All Additional Widgets"),
		LW(menu_toggle_all_additional_widgets1));
	keybindings_set_item(group, GEANY_KEYS_VIEW_FULLSCREEN, cb_func_menu_fullscreen,
		GDK_F11, 0, "menu_fullscreen", _("Fullscreen"), LW(menu_fullscreen1));
	keybindings_set_item(group, GEANY_KEYS_VIEW_MESSAGEWINDOW, cb_func_menu_messagewindow,
		0, 0, "menu_messagewindow", _("Toggle Messages Window"),
		LW(menu_show_messages_window1));
	keybindings_set_item(group, GEANY_KEYS_VIEW_SIDEBAR, NULL,
		0, 0, "toggle_sidebar", _("Toggle Sidebar"), LW(menu_show_sidebar1));
	keybindings_set_item(group, GEANY_KEYS_VIEW_ZOOMIN, NULL,
		GDK_plus, GDK_CONTROL_MASK, "menu_zoomin", _("Zoom In"), LW(menu_zoom_in1));
	keybindings_set_item(group, GEANY_KEYS_VIEW_ZOOMOUT, NULL,
		GDK_minus, GDK_CONTROL_MASK, "menu_zoomout", _("Zoom Out"), LW(menu_zoom_out1));
	keybindings_set_item(group, GEANY_KEYS_VIEW_ZOOMRESET, NULL,
		GDK_0, GDK_CONTROL_MASK, "normal_size", _("Zoom Reset"), LW(normal_size1));

	group = ADD_KB_GROUP(FOCUS, _("Focus"), cb_func_switch_action);

	keybindings_set_item(group, GEANY_KEYS_FOCUS_EDITOR, NULL,
		GDK_F2, 0, "switch_editor", _("Switch to Editor"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_SEARCHBAR, NULL,
		GDK_F7, 0, "switch_search_bar", _("Switch to Search Bar"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_MESSAGE_WINDOW, NULL,
		0, 0, "switch_message_window", _("Switch to Message Window"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_COMPILER, NULL,
		0, 0, "switch_compiler", _("Switch to Compiler"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_MESSAGES, NULL,
		0, 0, "switch_messages", _("Switch to Messages"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_SCRIBBLE, NULL,
		GDK_F6, 0, "switch_scribble", _("Switch to Scribble"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_VTE, NULL,
		GDK_F4, 0, "switch_vte", _("Switch to VTE"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_SIDEBAR, NULL,
		0, 0, "switch_sidebar", _("Switch to Sidebar"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_SIDEBAR_SYMBOL_LIST, NULL,
		0, 0, "switch_sidebar_symbol_list", _("Switch to Sidebar Symbol List"), NULL);
	keybindings_set_item(group, GEANY_KEYS_FOCUS_SIDEBAR_DOCUMENT_LIST, NULL,
		0, 0, "switch_sidebar_doc_list", _("Switch to Sidebar Document List"), NULL);

	group = ADD_KB_GROUP(NOTEBOOK, _("Notebook tab"), NULL);

	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_SWITCHTABLEFT, cb_func_switch_tableft,
		GDK_Page_Up, GDK_CONTROL_MASK, "switch_tableft", _("Switch to left document"), NULL);
	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_SWITCHTABRIGHT, cb_func_switch_tabright,
		GDK_Page_Down, GDK_CONTROL_MASK, "switch_tabright", _("Switch to right document"), NULL);
	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_SWITCHTABLASTUSED, cb_func_switch_tablastused,
		GDK_Tab, GDK_CONTROL_MASK, "switch_tablastused", _("Switch to last used document"), NULL);
	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_MOVETABLEFT, cb_func_move_tab,
		GDK_Page_Up, GDK_MOD1_MASK, "move_tableft", _("Move document left"), NULL);
	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_MOVETABRIGHT, cb_func_move_tab,
		GDK_Page_Down, GDK_MOD1_MASK, "move_tabright", _("Move document right"), NULL);
	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_MOVETABFIRST, cb_func_move_tab,
		0, 0, "move_tabfirst", _("Move document first"), NULL);
	keybindings_set_item(group, GEANY_KEYS_NOTEBOOK_MOVETABLAST, cb_func_move_tab,
		0, 0, "move_tablast", _("Move document last"), NULL);

	group = ADD_KB_GROUP(DOCUMENT, _("Document"), cb_func_document_action);

	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_LINEWRAP, NULL,
		0, 0, "menu_linewrap", _("Toggle Line wrapping"), LW(menu_line_wrapping1));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_LINEBREAK, NULL,
		0, 0, "menu_linebreak", _("Toggle Line breaking"), LW(line_breaking1));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_REPLACETABS, NULL,
		0, 0, "menu_replacetabs", _("Replace tabs by space"), LW(menu_replace_tabs));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_REPLACESPACES, NULL,
		0, 0, "menu_replacespaces", _("Replace spaces by tabs"), LW(menu_replace_spaces));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_TOGGLEFOLD, NULL,
		0, 0, "menu_togglefold", _("Toggle current fold"), NULL);
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_FOLDALL, NULL,
		0, 0, "menu_foldall", _("Fold all"), LW(menu_fold_all1));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_UNFOLDALL, NULL,
		0, 0, "menu_unfoldall", _("Unfold all"), LW(menu_unfold_all1));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_RELOADTAGLIST, NULL,
		GDK_r, GDK_SHIFT_MASK | GDK_CONTROL_MASK, "reloadtaglist", _("Reload symbol list"), NULL);
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_REMOVE_MARKERS, NULL,
		0, 0, "remove_markers", _("Remove Markers"), LW(remove_markers1));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_REMOVE_ERROR_INDICATORS, NULL,
		0, 0, "remove_error_indicators", _("Remove Error Indicators"), LW(menu_remove_indicators1));
	keybindings_set_item(group, GEANY_KEYS_DOCUMENT_REMOVE_MARKERS_INDICATORS, NULL,
		0, 0, "remove_markers_and_indicators", _("Remove Markers and Error Indicators"), NULL);

	group = ADD_KB_GROUP(BUILD, _("Build"), cb_func_build_action);

	keybindings_set_item(group, GEANY_KEYS_BUILD_COMPILE, NULL,
		GDK_F8, 0, "build_compile", _("Compile"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_LINK, NULL,
		GDK_F9, 0, "build_link", _("Build"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_MAKE, NULL,
		GDK_F9, GDK_SHIFT_MASK, "build_make", _("Make all"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_MAKEOWNTARGET, NULL,
		GDK_F9, GDK_SHIFT_MASK | GDK_CONTROL_MASK, "build_makeowntarget",
		_("Make custom target"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_MAKEOBJECT, NULL,
		0, 0, "build_makeobject", _("Make object"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_NEXTERROR, NULL,
		0, 0, "build_nexterror", _("Next error"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_PREVIOUSERROR, NULL,
		0, 0, "build_previouserror", _("Previous error"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_RUN, NULL,
		GDK_F5, 0, "build_run", _("Run"), NULL);
	keybindings_set_item(group, GEANY_KEYS_BUILD_OPTIONS, NULL,
		0, 0, "build_options", _("Build options"), NULL);

	group = ADD_KB_GROUP(TOOLS, _("Tools"), NULL);

	keybindings_set_item(group, GEANY_KEYS_TOOLS_OPENCOLORCHOOSER, cb_func_menu_opencolorchooser,
		0, 0, "menu_opencolorchooser", _("Show Color Chooser"), LW(menu_choose_color1));

	group = ADD_KB_GROUP(HELP, _("Help"), NULL);

	keybindings_set_item(group, GEANY_KEYS_HELP_HELP, cb_func_menu_help,
		GDK_F1, 0, "menu_help", _("Help"), LW(help1));
}


/* before the tab changes, add the current document to the MRU list */
static void on_notebook_switch_page(void)
{
	GeanyDocument *old = document_get_current();

	/* when closing current doc, old is NULL.
	 * Don't add to the mru list when switch dialog is visible. */
	if (old && !switch_in_progress)
	{
		g_queue_remove(mru_docs, old);
		g_queue_push_head(mru_docs, old);

		if (g_queue_get_length(mru_docs) > MAX_MRU_DOCS)
			g_queue_pop_tail(mru_docs);
	}
}


/* really this should be just after a document was closed, not idle */
static gboolean on_idle_close(gpointer data)
{
	GeanyDocument *current;

	current = document_get_current();
	if (current && g_queue_peek_head(mru_docs) == current)
		g_queue_pop_head(mru_docs);

	return FALSE;
}


static void on_document_close(GObject *obj, GeanyDocument *doc)
{
	if (! main_status.quitting)
	{
		g_queue_remove(mru_docs, doc);
		g_idle_add(on_idle_close, NULL);
	}
}


void keybindings_init(void)
{
	mru_docs = g_queue_new();
	g_signal_connect(main_widgets.notebook, "switch-page",
		G_CALLBACK(on_notebook_switch_page), NULL);
	g_signal_connect(geany_object, "document-close",
		G_CALLBACK(on_document_close), NULL);

	memset(binding_ids, 0, sizeof binding_ids);
	keybinding_groups = g_ptr_array_sized_new(GEANY_KEY_GROUP_COUNT);
	kb_accel_group = gtk_accel_group_new();

	init_default_kb();
	gtk_window_add_accel_group(GTK_WINDOW(main_widgets.window), kb_accel_group);

	g_signal_connect(main_widgets.window, "key-press-event", G_CALLBACK(on_key_press_event), NULL);
	/* in case the switch dialog misses an event while drawing the dialog */
	g_signal_connect(main_widgets.window, "key-release-event", G_CALLBACK(on_key_release_event), NULL);
}


typedef void (*KBItemCallback) (GeanyKeyGroup *group, GeanyKeyBinding *kb, gpointer user_data);

static void keybindings_foreach(KBItemCallback cb, gpointer user_data)
{
	gsize g, i;
	GeanyKeyGroup *group;
	GeanyKeyBinding *kb;

	foreach_ptr_array(group, g, keybinding_groups)
	{
		foreach_ptr_array(kb, i, group->key_items)
			cb(group, kb, user_data);
	}
}


static void load_kb(GeanyKeyGroup *group, GeanyKeyBinding *kb, gpointer user_data)
{
	GKeyFile *config = user_data;
	gchar *val;
	guint key;
	GdkModifierType mods;

	val = g_key_file_get_string(config, group->name, kb->name, NULL);
	if (val != NULL)
	{
		gtk_accelerator_parse(val, &key, &mods);
		kb->key = key;
		kb->mods = mods;
		g_free(val);
	}
}


static void load_user_kb(void)
{
	gchar *configfile = g_strconcat(app->configdir, G_DIR_SEPARATOR_S, "keybindings.conf", NULL);
	GKeyFile *config = g_key_file_new();

	/* now load user defined keys */
	if (g_key_file_load_from_file(config, configfile, G_KEY_FILE_KEEP_COMMENTS, NULL))
	{
		keybindings_foreach(load_kb, config);
	}
	g_free(configfile);
	g_key_file_free(config);
}


static void apply_kb_accel(GeanyKeyGroup *group, GeanyKeyBinding *kb, gpointer user_data)
{
	if (kb->key != 0 && kb->menu_item)
	{
		gtk_widget_add_accelerator(kb->menu_item, "activate", kb_accel_group,
			kb->key, kb->mods, GTK_ACCEL_VISIBLE);
	}
}


void keybindings_load_keyfile(void)
{
	load_user_kb();
	add_popup_menu_accels();

	/* set menu accels now, after user keybindings have been read */
	keybindings_foreach(apply_kb_accel, NULL);
}


static void add_menu_accel(GeanyKeyGroup *group, guint kb_id, GtkWidget *menuitem)
{
	GeanyKeyBinding *kb = keybindings_get_item(group, kb_id);

	if (kb->key != 0)
		gtk_widget_add_accelerator(menuitem, "activate", kb_accel_group,
			kb->key, kb->mods, GTK_ACCEL_VISIBLE);
}


#define GEANY_ADD_POPUP_ACCEL(kb_id, wid) \
	add_menu_accel(group, kb_id, ui_lookup_widget(main_widgets.editor_menu, G_STRINGIFY(wid)))

/* set the menu item accelerator shortcuts (just for visibility, they are handled anyway) */
static void add_popup_menu_accels(void)
{
	GeanyKeyGroup *group;

	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_EDITOR);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_EDITOR_UNDO, undo1);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_EDITOR_REDO, redo1);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_EDITOR_CONTEXTACTION, context_action1);

	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_SELECT);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_SELECT_ALL, menu_select_all2);

	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_INSERT);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_INSERT_DATE, insert_date_custom2);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_INSERT_ALTWHITESPACE, insert_alternative_white_space1);

	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_FILE);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_FILE_OPENSELECTED, menu_open_selected_file2);

	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_SEARCH);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_SEARCH_FINDUSAGE, find_usage1);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_SEARCH_FINDDOCUMENTUSAGE, find_document_usage1);

	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_GOTO);
	GEANY_ADD_POPUP_ACCEL(GEANY_KEYS_GOTO_TAGDEFINITION, goto_tag_definition1);

	/* Format and Commands share the menu bar submenus */
	/* Build menu items are set if the build menus are created */
}


static void set_keyfile_kb(GeanyKeyGroup *group, GeanyKeyBinding *kb, gpointer user_data)
{
	GKeyFile *config = user_data;
	gchar *val;

	val = gtk_accelerator_name(kb->key, kb->mods);
	g_key_file_set_string(config, group->name, kb->name, val);
	g_free(val);
}


/* just write the content of the keys array to the config file */
void keybindings_write_to_file(void)
{
	gchar *configfile = g_strconcat(app->configdir, G_DIR_SEPARATOR_S, "keybindings.conf", NULL);
	gchar *data;
	GKeyFile *config = g_key_file_new();

 	/* add comment if the file is newly created */
	if (! g_key_file_load_from_file(config, configfile, G_KEY_FILE_KEEP_COMMENTS, NULL))
	{
		g_key_file_set_comment(config, NULL, NULL,
			"Keybindings for Geany\nThe format looks like \"<Control>a\" or \"<Shift><Alt>F1\".\n"
			"But you can also change the keys in Geany's preferences dialog.", NULL);
	}

	keybindings_foreach(set_keyfile_kb, config);

	/* write the file */
	data = g_key_file_to_data(config, NULL, NULL);
	utils_write_file(configfile, data);

	g_free(data);
	g_free(configfile);
	g_key_file_free(config);
}


void keybindings_free(void)
{
	GeanyKeyGroup *group;
	gsize g;

	foreach_ptr_array(group, g, keybinding_groups)
		keybindings_free_group(group);

	g_ptr_array_free(keybinding_groups, TRUE);
	g_queue_free(mru_docs);
}


gchar *keybindings_get_label(GeanyKeyBinding *kb)
{
	return utils_str_remove_chars(g_strdup(kb->label), "_");
}


static void fill_shortcut_labels_treeview(GtkWidget *tree)
{
	gsize g, i;
	GeanyKeyBinding *kb;
	GeanyKeyGroup *group;
	GtkListStore *store;
	GtkTreeIter iter;

	store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, PANGO_TYPE_WEIGHT);

	foreach_ptr_array(group, g, keybinding_groups)
	{
		if (g > 0)
		{
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, -1);
		}
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 0, group->label, 2, PANGO_WEIGHT_BOLD, -1);

		foreach_ptr_array(kb, i, group->key_items)
		{
			gchar *shortcut, *label;

			label = keybindings_get_label(kb);
			shortcut = gtk_accelerator_get_label(kb->key, kb->mods);

			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 0, label, 1, shortcut, 2, PANGO_WEIGHT_NORMAL, -1);

			g_free(shortcut);
			g_free(label);
		}
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree), GTK_TREE_MODEL(store));
	g_object_unref(store);
}


static GtkWidget *create_dialog(void)
{
	GtkWidget *dialog, *tree, *label, *swin, *vbox;
	GtkCellRenderer *text_renderer;
	GtkTreeViewColumn *column;

	dialog = gtk_dialog_new_with_buttons(_("Keyboard Shortcuts"), GTK_WINDOW(main_widgets.window),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_STOCK_EDIT, GTK_RESPONSE_APPLY,
				GTK_STOCK_CLOSE, GTK_RESPONSE_CANCEL, NULL);
	vbox = ui_dialog_vbox_new(GTK_DIALOG(dialog));
	gtk_box_set_spacing(GTK_BOX(vbox), 6);
	gtk_widget_set_name(dialog, "GeanyDialog");

	gtk_window_set_default_size(GTK_WINDOW(dialog), -1, GEANY_DEFAULT_DIALOG_HEIGHT);

	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);

	label = gtk_label_new(_("The following keyboard shortcuts are configurable:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

	tree = gtk_tree_view_new();
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tree), TRUE);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);

	text_renderer = gtk_cell_renderer_text_new();
	/* we can't use "weight-set", see http://bugzilla.gnome.org/show_bug.cgi?id=355214 */
	column = gtk_tree_view_column_new_with_attributes(
		NULL, text_renderer, "text", 0, "weight", 2, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	text_renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(NULL, text_renderer, "text", 1, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	fill_shortcut_labels_treeview(tree);

	swin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swin), GTK_POLICY_NEVER,
		GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(swin), GTK_SHADOW_ETCHED_IN);
	gtk_container_add(GTK_CONTAINER(swin), tree);

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
	gtk_box_pack_start(GTK_BOX(vbox), swin, TRUE, TRUE, 0);

	return dialog;
}


/* non-modal keyboard shortcuts dialog, so user can edit whilst seeing the shortcuts */
static GtkWidget *key_dialog = NULL;

static void on_dialog_response(GtkWidget *dialog, gint response, gpointer user_data)
{
	if (response == GTK_RESPONSE_APPLY)
	{
		GtkWidget *wid;

		prefs_show_dialog();
		/* select the KB page */
		wid = ui_lookup_widget(ui_widgets.prefs_dialog, "frame22");
		if (wid != NULL)
		{
			GtkNotebook *nb = GTK_NOTEBOOK(ui_lookup_widget(ui_widgets.prefs_dialog, "notebook2"));

			if (nb != NULL)
				gtk_notebook_set_current_page(nb, gtk_notebook_page_num(nb, wid));
		}
	}
	gtk_widget_destroy(dialog);
	key_dialog = NULL;
}


void keybindings_show_shortcuts(void)
{
	if (key_dialog)
		gtk_widget_destroy(key_dialog);	/* in case the key_dialog is still visible */

	key_dialog = create_dialog();
	g_signal_connect(key_dialog, "response", G_CALLBACK(on_dialog_response), NULL);
	gtk_widget_show_all(key_dialog);
}


static gboolean check_fixed_kb(guint keyval, guint state)
{
	/* check alt-0 to alt-9 for setting current notebook page */
	if (state == GDK_MOD1_MASK && keyval >= GDK_0 && keyval <= GDK_9)
	{
		gint page = keyval - GDK_0 - 1;
		gint npages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(main_widgets.notebook));

		/* alt-0 is for the rightmost tab */
		if (keyval == GDK_0)
			page = npages - 1;
		/* invert the order if tabs are added on the other side */
		if (swap_alt_tab_order && ! file_prefs.tab_order_ltr)
			page = (npages - 1) - page;

		gtk_notebook_set_current_page(GTK_NOTEBOOK(main_widgets.notebook), page);
		return TRUE;
	}
	if (keyval == GDK_Page_Up || keyval == GDK_Page_Down)
	{
		/* switch to first or last document */
		if (state == (GDK_CONTROL_MASK | GDK_SHIFT_MASK))
		{
			if (keyval == GDK_Page_Up)
				gtk_notebook_set_current_page(GTK_NOTEBOOK(main_widgets.notebook), 0);
			if (keyval == GDK_Page_Down)
				gtk_notebook_set_current_page(GTK_NOTEBOOK(main_widgets.notebook),
					gtk_notebook_get_n_pages(GTK_NOTEBOOK(main_widgets.notebook)) - 1);
			return TRUE;
		}
	}
	return FALSE;
}


static gboolean check_snippet_completion(GeanyDocument *doc)
{
	GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));

	g_return_val_if_fail(doc, FALSE);

	/* keybinding only valid when scintilla widget has focus */
	if (focusw == GTK_WIDGET(doc->editor->sci))
	{
		ScintillaObject *sci = doc->editor->sci;
		gint pos = sci_get_current_position(sci);

		if (editor_prefs.complete_snippets)
			return editor_complete_snippet(doc->editor, pos);
	}
	return FALSE;
}


/* Transforms a GdkEventKey event into a GdkEventButton event */
static void trigger_button_event(GtkWidget *widget, guint32 event_time)
{
	GdkEventButton *event;
	gboolean ret;

	event = g_new0(GdkEventButton, 1);

	if (GTK_IS_TEXT_VIEW(widget))
		event->window = gtk_text_view_get_window(GTK_TEXT_VIEW(widget), GTK_TEXT_WINDOW_TEXT);
	else
		event->window = widget->window;
	event->time = event_time;
	event->type = GDK_BUTTON_PRESS;
	event->button = 3;

	g_signal_emit_by_name(widget, "button-press-event", event, &ret);
	g_signal_emit_by_name(widget, "button-release-event", event, &ret);

	g_free(event);
}


/* Special case for the Menu key and Shift-F10 to show the right-click popup menu for various
 * widgets. Without this special handling, the notebook tab list of the documents' notebook
 * would be shown. As a very special case, we differentiate between the Menu key and Shift-F10
 * if pressed in the editor widget: the Menu key opens the popup menu, Shift-F10 opens the
 * notebook tab list. */
static gboolean check_menu_key(GeanyDocument *doc, guint keyval, guint state, guint32 event_time)
{
	if ((keyval == GDK_Menu && state == 0) || (keyval == GDK_F10 && state == GDK_SHIFT_MASK))
	{
		GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));
		if (doc != NULL)
		{
			if (focusw == doc->priv->tag_tree)
			{
				trigger_button_event(focusw, event_time);
				return TRUE;
			}
			if (focusw == GTK_WIDGET(doc->editor->sci))
			{
				if (keyval == GDK_Menu)
				{	/* show editor popup menu */
					trigger_button_event(focusw, event_time);
					return TRUE;
				}
				else
				{	/* show tab bar menu */
					trigger_button_event(main_widgets.notebook, event_time);
					return TRUE;
				}
			}
		}
		if (focusw == tv.tree_openfiles
		 || focusw == msgwindow.tree_status
		 || focusw == msgwindow.tree_compiler
		 || focusw == msgwindow.tree_msg
		 || focusw == msgwindow.scribble
#ifdef HAVE_VTE
		 || (vte_info.have_vte && focusw == vc->vte)
#endif
		)
		{
			trigger_button_event(focusw, event_time);
			return TRUE;
		}
	}
	return FALSE;
}


#ifdef HAVE_VTE
static gboolean on_menu_expose_event(GtkWidget *widget, GdkEventExpose *event,
		gpointer user_data)
{
	if (!GTK_WIDGET_SENSITIVE(widget))
		gtk_widget_set_sensitive(GTK_WIDGET(widget), TRUE);
	return FALSE;
}


static gboolean set_sensitive(gpointer widget)
{
	gtk_widget_set_sensitive(GTK_WIDGET(widget), TRUE);
	return FALSE;
}


static gboolean check_vte(GdkModifierType state, guint keyval)
{
	guint i;
	GeanyKeyBinding *kb;
	GeanyKeyGroup *group;
	GtkWidget *widget;

	if (gtk_window_get_focus(GTK_WINDOW(main_widgets.window)) != vc->vte)
		return FALSE;
	/* let VTE copy/paste override any user keybinding */
	if (state == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) && (keyval == GDK_c || keyval == GDK_v))
		return TRUE;
	if (! vc->enable_bash_keys)
		return FALSE;
	/* prevent menubar flickering: */
	if (state == GDK_SHIFT_MASK && (keyval >= GDK_a && keyval <= GDK_z))
		return FALSE;
	if (state == 0 && (keyval < GDK_F1 || keyval > GDK_F35))	/* e.g. backspace */
		return FALSE;

	/* make focus commands override any bash commands */
	group = g_ptr_array_index(keybinding_groups, GEANY_KEY_GROUP_FOCUS);
	foreach_ptr_array(kb, i, group->key_items)
	{
		if (state == kb->mods && keyval == kb->key)
			return FALSE;
	}

	/* Temporarily disable the menus to prevent conflicting menu accelerators
	 * from overriding the VTE bash shortcuts.
	 * Note: maybe there's a better way of doing this ;-) */
	widget = ui_lookup_widget(main_widgets.window, "menubar1");
	gtk_widget_set_sensitive(widget, FALSE);
	{
		/* make the menubar sensitive before it is redrawn */
		static gboolean connected = FALSE;
		if (!connected)
			g_signal_connect(widget, "expose-event", G_CALLBACK(on_menu_expose_event), NULL);
	}

	widget = main_widgets.editor_menu;
	gtk_widget_set_sensitive(widget, FALSE);
	g_idle_add(set_sensitive, widget);
	return TRUE;
}
#endif


/* Map the keypad keys to their equivalent functions (taken from ScintillaGTK.cxx) */
static guint key_kp_translate(guint key_in)
{
	switch (key_in)
	{
		case GDK_KP_Down:
			return GDK_Down;
		case GDK_KP_Up:
			return GDK_Up;
		case GDK_KP_Left:
			return GDK_Left;
		case GDK_KP_Right:
			return GDK_Right;
		case GDK_KP_Home:
			return GDK_Home;
		case GDK_KP_End:
			return GDK_End;
		case GDK_KP_Page_Up:
			return GDK_Page_Up;
		case GDK_KP_Page_Down:
			return GDK_Page_Down;
		case GDK_KP_Delete:
			return GDK_Delete;
		case GDK_KP_Insert:
			return GDK_Insert;
		default:
			return key_in;
	}
}


/* Check if event keypress matches keybinding combo */
gboolean keybindings_check_event(GdkEventKey *ev, GeanyKeyBinding *kb)
{
	guint state, keyval;

	if (ev->keyval == 0)
		return FALSE;

	keyval = ev->keyval;
	state = ev->state & gtk_accelerator_get_default_mod_mask();
	/* hack to get around that CTRL+Shift+r results in GDK_R not GDK_r */
	if ((ev->state & GDK_SHIFT_MASK) || (ev->state & GDK_LOCK_MASK))
		if (keyval >= GDK_A && keyval <= GDK_Z)
			keyval += GDK_a - GDK_A;

	if (keyval >= GDK_KP_Space && keyval < GDK_KP_Equal)
		keyval = key_kp_translate(keyval);

	return (keyval == kb->key && state == kb->mods);
}


/* central keypress event handler, almost all keypress events go to this function */
static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *ev, gpointer user_data)
{
	guint state, keyval;
	gsize g, i;
	GeanyDocument *doc;
	GeanyKeyGroup *group;
	GeanyKeyBinding *kb;

	if (ev->keyval == 0)
		return FALSE;

	doc = document_get_current();
	if (doc)
		document_check_disk_status(doc, FALSE);

	keyval = ev->keyval;
	state = ev->state & gtk_accelerator_get_default_mod_mask();
	/* hack to get around that CTRL+Shift+r results in GDK_R not GDK_r */
	if ((ev->state & GDK_SHIFT_MASK) || (ev->state & GDK_LOCK_MASK))
		if (keyval >= GDK_A && keyval <= GDK_Z)
			keyval += GDK_a - GDK_A;

	if (keyval >= GDK_KP_Space && keyval < GDK_KP_Equal)
		keyval = key_kp_translate(keyval);

	/*geany_debug("%d (%d) %d (%d)", keyval, ev->keyval, state, ev->state);*/

	/* special cases */
#ifdef HAVE_VTE
	if (vte_info.have_vte && check_vte(state, keyval))
		return FALSE;
#endif
	if (check_menu_key(doc, keyval, state, ev->time))
		return TRUE;

	foreach_ptr_array(group, g, keybinding_groups)
	{
		foreach_ptr_array(kb, i, group->key_items)
		{
			if (keyval == kb->key && state == kb->mods)
			{
				/* call the corresponding callback function for this shortcut */
				if (kb->callback)
				{
					kb->callback(kb->id);
					return TRUE;
				}
				else if (group->callback)
				{
					if (group->callback(kb->id))
						return TRUE;
					else
						continue;	/* not handled */
				}
				g_warning("No callback for keybinding %s: %s!", group->name, kb->name);
			}
		}
	}
	/* fixed keybindings can be overridden by user bindings, so check them last */
	if (check_fixed_kb(keyval, state))
		return TRUE;
	return FALSE;
}


static gboolean is_modifier_key(guint keyval)
{
	switch (keyval)
	{
		case GDK_Shift_L:
		case GDK_Shift_R:
		case GDK_Control_L:
		case GDK_Control_R:
		case GDK_Meta_L:
		case GDK_Meta_R:
		case GDK_Alt_L:
		case GDK_Alt_R:
		case GDK_Super_L:
		case GDK_Super_R:
		case GDK_Hyper_L:
		case GDK_Hyper_R:
			return TRUE;
		default:
			return FALSE;
	}
}


GeanyKeyBinding *keybindings_lookup_item(guint group_id, guint key_id)
{
	GeanyKeyGroup *group;

	g_return_val_if_fail(group_id < keybinding_groups->len, NULL);

	group = g_ptr_array_index(keybinding_groups, group_id);

	g_return_val_if_fail(group, NULL);
	return keybindings_get_item(group, key_id);
}


/** Mimics a (built-in only) keybinding action.
 * 	Example: @code keybindings_send_command(GEANY_KEY_GROUP_FILE, GEANY_KEYS_FILE_OPEN); @endcode
 * 	@param group_id @ref GeanyKeyGroupID keybinding group index that contains the @a key_id keybinding.
 * 	@param key_id @ref GeanyKeyBindingID keybinding index. */
void keybindings_send_command(guint group_id, guint key_id)
{
	GeanyKeyBinding *kb;

	g_return_if_fail(group_id < GEANY_KEY_GROUP_COUNT); /* can't use this for plugin groups */

	kb = keybindings_lookup_item(group_id, key_id);
	if (kb)
	{
		if (kb->callback)
			kb->callback(key_id);
		else
		{
			GeanyKeyGroup *group = g_ptr_array_index(keybinding_groups, group_id);

			if (group->callback)
				group->callback(key_id);
		}
	}
}


/* These are the callback functions, either each group or each shortcut has it's
 * own function. */


static gboolean cb_func_file_action(guint key_id)
{
	switch (key_id)
	{
		case GEANY_KEYS_FILE_NEW:
			document_new_file(NULL, NULL, NULL);
			break;
		case GEANY_KEYS_FILE_OPEN:
			on_open1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_OPENSELECTED:
			on_menu_open_selected_file1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_OPENLASTTAB:
		{
			gchar *utf8_filename = g_queue_peek_head(ui_prefs.recent_queue);
			gchar *locale_filename = utils_get_locale_from_utf8(utf8_filename);
			document_open_file(locale_filename, FALSE, NULL, NULL);
			g_free(locale_filename);
			break;
		}
		case GEANY_KEYS_FILE_SAVE:
			on_save1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_SAVEAS:
			on_save_as1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_SAVEALL:
			on_save_all1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_CLOSE:
			on_close1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_CLOSEALL:
			on_close_all1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_RELOAD:
			on_toolbutton_reload_clicked(NULL, NULL);
			break;
		case GEANY_KEYS_FILE_PRINT:
			on_print1_activate(NULL, NULL);
			break;
	}
	return TRUE;
}


static gboolean cb_func_project_action(guint key_id)
{
	switch (key_id)
	{
		case GEANY_KEYS_PROJECT_PROPERTIES:
			if (app->project)
				on_project_properties1_activate(NULL, NULL);
			break;
	}
	return TRUE;
}


static void cb_func_menu_preferences(guint key_id)
{
	switch (key_id)
	{
		case GEANY_KEYS_SETTINGS_PREFERENCES:
			on_preferences1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_SETTINGS_PLUGINPREFERENCES:
			on_plugin_preferences1_activate(NULL, NULL);
			break;
	}
}


static void cb_func_menu_help(G_GNUC_UNUSED guint key_id)
{
	on_help1_activate(NULL, NULL);
}


static gboolean cb_func_search_action(guint key_id)
{
	GeanyDocument *doc = document_get_current();
	ScintillaObject *sci;

	if (key_id == GEANY_KEYS_SEARCH_FINDINFILES)
	{
		on_find_in_files1_activate(NULL, NULL);	/* works without docs too */
		return TRUE;
	}
	if (!doc)
		return TRUE;
	sci = doc->editor->sci;

	switch (key_id)
	{
		case GEANY_KEYS_SEARCH_FIND:
			on_find1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_FINDNEXT:
			on_find_next1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_FINDPREVIOUS:
			on_find_previous1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_FINDPREVSEL:
			on_find_prevsel1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_FINDNEXTSEL:
			on_find_nextsel1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_REPLACE:
			on_replace1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_NEXTMESSAGE:
			on_next_message1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_PREVIOUSMESSAGE:
			on_previous_message1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_FINDUSAGE:
			on_find_usage1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_FINDDOCUMENTUSAGE:
			on_find_document_usage1_activate(NULL, NULL); break;
		case GEANY_KEYS_SEARCH_MARKALL:
		{
			gchar *text = get_current_word_or_sel(doc, TRUE);

			if (sci_has_selection(sci))
				search_mark_all(doc, text, SCFIND_MATCHCASE);
			else
			{
				/* clears markers if text is null */
				search_mark_all(doc, text, SCFIND_MATCHCASE | SCFIND_WHOLEWORD);
			}
			g_free(text);
			break;
		}
	}
	return TRUE;
}


static void cb_func_menu_opencolorchooser(G_GNUC_UNUSED guint key_id)
{
	on_show_color_chooser1_activate(NULL, NULL);
}


static gboolean cb_func_view_action(guint key_id)
{
	switch (key_id)
	{
		case GEANY_KEYS_VIEW_TOGGLEALL:
			on_menu_toggle_all_additional_widgets1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_VIEW_SIDEBAR:
			on_menu_show_sidebar1_toggled(NULL, NULL);
			break;
		case GEANY_KEYS_VIEW_ZOOMIN:
			on_zoom_in1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_VIEW_ZOOMOUT:
			on_zoom_out1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_VIEW_ZOOMRESET:
			on_normal_size1_activate(NULL, NULL);
			break;
		default:
			break;
	}
	return TRUE;
}


static void cb_func_menu_fullscreen(G_GNUC_UNUSED guint key_id)
{
	GtkCheckMenuItem *c = GTK_CHECK_MENU_ITEM(
		ui_lookup_widget(main_widgets.window, "menu_fullscreen1"));

	gtk_check_menu_item_set_active(c, ! gtk_check_menu_item_get_active(c));
}


static void cb_func_menu_messagewindow(G_GNUC_UNUSED guint key_id)
{
	GtkCheckMenuItem *c = GTK_CHECK_MENU_ITEM(
		ui_lookup_widget(main_widgets.window, "menu_show_messages_window1"));

	gtk_check_menu_item_set_active(c, ! gtk_check_menu_item_get_active(c));
}


static gboolean cb_func_build_action(guint key_id)
{
	GtkWidget *item;
	BuildMenuItems *menu_items;
	GeanyDocument *doc = document_get_current();

	if (doc == NULL)
		return TRUE;

	if (!GTK_WIDGET_IS_SENSITIVE(ui_lookup_widget(main_widgets.window, "menu_build1")))
		return TRUE;

	menu_items = build_get_menu_items(doc->file_type->id);
	/* TODO make it a table??*/
	switch (key_id)
	{
		case GEANY_KEYS_BUILD_COMPILE:
			item = menu_items->menu_item[GEANY_GBG_FT][GBO_TO_CMD(GEANY_GBO_COMPILE)];
			break;
		case GEANY_KEYS_BUILD_LINK:
			item = menu_items->menu_item[GEANY_GBG_FT][GBO_TO_CMD(GEANY_GBO_BUILD)];
			break;
		case GEANY_KEYS_BUILD_MAKE:
			item = menu_items->menu_item[GEANY_GBG_NON_FT][GBO_TO_CMD(GEANY_GBO_MAKE_ALL)];
			break;
		case GEANY_KEYS_BUILD_MAKEOWNTARGET:
			item = menu_items->menu_item[GEANY_GBG_NON_FT][GBO_TO_CMD(GEANY_GBO_CUSTOM)];
			break;
		case GEANY_KEYS_BUILD_MAKEOBJECT:
			item = menu_items->menu_item[GEANY_GBG_NON_FT][GBO_TO_CMD(GEANY_GBO_MAKE_OBJECT)];
			break;
		case GEANY_KEYS_BUILD_NEXTERROR:
			item = menu_items->menu_item[GBG_FIXED][GBF_NEXT_ERROR];
			break;
		case GEANY_KEYS_BUILD_PREVIOUSERROR:
			item = menu_items->menu_item[GBG_FIXED][GBF_PREV_ERROR];
			break;
		case GEANY_KEYS_BUILD_RUN:
			item = menu_items->menu_item[GEANY_GBG_EXEC][GBO_TO_CMD(GEANY_GBO_EXEC)];
			break;
		case GEANY_KEYS_BUILD_OPTIONS:
			item = menu_items->menu_item[GBG_FIXED][GBF_COMMANDS];
			break;
		default:
			item = NULL;
	}
	/* Note: For Build menu items it's OK (at the moment) to assume they are in the correct
	 * sensitive state, but some other menus don't update the sensitive status until
	 * they are redrawn. */
	if (item && GTK_WIDGET_IS_SENSITIVE(item))
		gtk_menu_item_activate(GTK_MENU_ITEM(item));
	return TRUE;
}


static gboolean read_current_word(GeanyDocument *doc, gboolean sci_word)
{
	if (doc == NULL)
		return FALSE;

	if (sci_word)
	{
		editor_find_current_word_sciwc(doc->editor, -1,
			editor_info.current_word, GEANY_MAX_WORD_LENGTH);
	}
	else
	{
		editor_find_current_word(doc->editor, -1,
			editor_info.current_word, GEANY_MAX_WORD_LENGTH, NULL);
	}

	return (*editor_info.current_word != 0);
}


static gboolean check_current_word(GeanyDocument *doc, gboolean sci_word)
{
	if (! read_current_word(doc, sci_word))
	{
		utils_beep();
		return FALSE;
	}
	return TRUE;
}


static gchar *get_current_word_or_sel(GeanyDocument *doc, gboolean sci_word)
{
	ScintillaObject *sci = doc->editor->sci;

	if (sci_has_selection(sci))
		return sci_get_selection_contents(sci);

	return read_current_word(doc, sci_word) ? g_strdup(editor_info.current_word) : NULL;
}


static void focus_sidebar(void)
{
	if (ui_prefs.sidebar_visible)
	{
		gint page_num = gtk_notebook_get_current_page(GTK_NOTEBOOK(main_widgets.sidebar_notebook));
		GtkWidget *page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(main_widgets.sidebar_notebook), page_num);

		/* gtk_widget_grab_focus() won't work because of the scrolled window containers */
		gtk_widget_child_focus(page, GTK_DIR_TAB_FORWARD);
	}
}


static void focus_msgwindow(void)
{
	if (ui_prefs.msgwindow_visible)
	{
		gint page_num = gtk_notebook_get_current_page(GTK_NOTEBOOK(msgwindow.notebook));
		GtkWidget *page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(msgwindow.notebook), page_num);

		gtk_widget_grab_focus(gtk_bin_get_child(GTK_BIN(page)));
	}
}


static gboolean cb_func_switch_action(guint key_id)
{
	switch (key_id)
	{
		case GEANY_KEYS_FOCUS_EDITOR:
		{
			GeanyDocument *doc = document_get_current();
			if (doc != NULL)
			{
				GtkWidget *sci = GTK_WIDGET(doc->editor->sci);
				if (GTK_WIDGET_HAS_FOCUS(sci))
					ui_update_statusbar(doc, -1);
				else
					gtk_widget_grab_focus(sci);
			}
			break;
		}
		case GEANY_KEYS_FOCUS_SCRIBBLE:
			msgwin_switch_tab(MSG_SCRATCH, TRUE);
			break;
		case GEANY_KEYS_FOCUS_SEARCHBAR:
			if (toolbar_prefs.visible)
			{
				GtkWidget *search_entry = toolbar_get_widget_child_by_name("SearchEntry");
				if (search_entry != NULL)
					gtk_widget_grab_focus(search_entry);
			}
			break;
		case GEANY_KEYS_FOCUS_SIDEBAR:
			focus_sidebar();
			break;
		case GEANY_KEYS_FOCUS_VTE:
			msgwin_switch_tab(MSG_VTE, TRUE);
			break;
		case GEANY_KEYS_FOCUS_COMPILER:
			msgwin_switch_tab(MSG_COMPILER, TRUE);
			break;
		case GEANY_KEYS_FOCUS_MESSAGES:
			msgwin_switch_tab(MSG_MESSAGE, TRUE);
			break;
		case GEANY_KEYS_FOCUS_MESSAGE_WINDOW:
			focus_msgwindow();
			break;
		case GEANY_KEYS_FOCUS_SIDEBAR_DOCUMENT_LIST:
			sidebar_focus_openfiles_tab();
			break;
		case GEANY_KEYS_FOCUS_SIDEBAR_SYMBOL_LIST:
			sidebar_focus_symbols_tab();
			break;
	}
	return TRUE;
}


static void switch_notebook_page(gint direction)
{
	gint page_count, cur_page;
	gboolean parent_is_notebook = FALSE;
	GtkNotebook *notebook;
	GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));

	/* check whether the current widget is a GtkNotebook or a child of a GtkNotebook */
	do
	{
		parent_is_notebook = GTK_IS_NOTEBOOK(focusw);
	}
	while (! parent_is_notebook && (focusw = gtk_widget_get_parent(focusw)) != NULL);

	/* if we found a GtkNotebook widget, use it. Otherwise fallback to the documents notebook */
	if (parent_is_notebook)
		notebook = GTK_NOTEBOOK(focusw);
	else
		notebook = GTK_NOTEBOOK(main_widgets.notebook);

	/* now switch pages */
	page_count = gtk_notebook_get_n_pages(notebook);
	cur_page = gtk_notebook_get_current_page(notebook);

	if (direction == GTK_DIR_LEFT)
	{
		if (cur_page > 0)
			gtk_notebook_set_current_page(notebook, cur_page - 1);
		else
			gtk_notebook_set_current_page(notebook, page_count - 1);
	}
	else if (direction == GTK_DIR_RIGHT)
	{
		if (cur_page < page_count - 1)
			gtk_notebook_set_current_page(notebook, cur_page + 1);
		else
			gtk_notebook_set_current_page(notebook, 0);
	}
}


static void cb_func_switch_tableft(G_GNUC_UNUSED guint key_id)
{
	switch_notebook_page(GTK_DIR_LEFT);
}


static void cb_func_switch_tabright(G_GNUC_UNUSED guint key_id)
{
	switch_notebook_page(GTK_DIR_RIGHT);
}


static gboolean on_key_release_event(GtkWidget *widget, GdkEventKey *ev, gpointer user_data)
{
	/* user may have rebound keybinding to a different modifier than Ctrl, so check all */
	if (switch_in_progress && is_modifier_key(ev->keyval))
	{
		switch_in_progress = FALSE;

		if (switch_dialog)
		{
			gtk_widget_destroy(switch_dialog);
			switch_dialog = NULL;
		}

		mru_pos = 0;
	}
	return FALSE;
}


static GtkWidget *ui_minimal_dialog_new(GtkWindow *parent, const gchar *title)
{
	GtkWidget *dialog;

	dialog = gtk_window_new(GTK_WINDOW_POPUP);

	if (parent)
	{
		gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
		gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);
	}
	gtk_window_set_title(GTK_WINDOW(dialog), title);
	gtk_window_set_type_hint(GTK_WINDOW(dialog), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

	gtk_widget_set_name(dialog, "GeanyDialog");
	return dialog;
}


static GtkWidget *create_switch_dialog(void)
{
	GtkWidget *dialog, *widget, *vbox;

	dialog = ui_minimal_dialog_new(GTK_WINDOW(main_widgets.window), _("Switch to Document"));
	gtk_window_set_decorated(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 150, -1);

	vbox = gtk_vbox_new(FALSE, 6);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 12);
	gtk_container_add(GTK_CONTAINER(dialog), vbox);

	widget = gtk_image_new_from_stock(GTK_STOCK_JUMP_TO, GTK_ICON_SIZE_BUTTON);
	gtk_container_add(GTK_CONTAINER(vbox), widget);

	widget = geany_wrap_label_new(NULL);
	gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_CENTER);
	gtk_container_add(GTK_CONTAINER(vbox), widget);
	switch_dialog_label = widget;

	g_signal_connect(dialog, "key-release-event", G_CALLBACK(on_key_release_event), NULL);
	return dialog;
}


static void update_filename_label(void)
{
	if (!switch_dialog)
	{
		switch_dialog = create_switch_dialog();
		gtk_widget_show_all(switch_dialog);
	}

	gtk_label_set_text(GTK_LABEL(switch_dialog_label), DOC_FILENAME(document_get_current()));
}


static gboolean on_switch_timeout(G_GNUC_UNUSED gpointer data)
{
	if (!switch_in_progress || switch_dialog)
	{
		return FALSE;
	}

	update_filename_label();
	return FALSE;
}


static void cb_func_switch_tablastused(G_GNUC_UNUSED guint key_id)
{
	GeanyDocument *last_doc = g_queue_peek_nth(mru_docs, mru_pos);

	if (! DOC_VALID(last_doc))
	{
		utils_beep();
		mru_pos = 0;
		last_doc = g_queue_peek_nth(mru_docs, mru_pos);
	}
	if (! DOC_VALID(last_doc))
		return;

	gtk_notebook_set_current_page(GTK_NOTEBOOK(main_widgets.notebook),
		document_get_notebook_page(last_doc));

	/* if there's a modifier key, we can switch back in MRU order each time unless
	 * the key is released */
	if (!switch_in_progress)
	{
		switch_in_progress = TRUE;

		/* because switch_in_progress was not set when we called
		 * gtk_notebook_set_current_page() above, this function inserted last_doc
		 * into the queue => on mru_pos = 0 there is last_doc, on mru_pos = 1
		 * there is the currently displayed doc, so we want to continue from 2
		 * next time this function is called */
		mru_pos = 2;

		/* delay showing dialog to give user time to let go of any modifier keys */
		g_timeout_add(600, on_switch_timeout, NULL);
	}
	else
	{
		update_filename_label();
		mru_pos += 1;
	}
}


/* move document left/right/first/last */
static void cb_func_move_tab(guint key_id)
{
	GtkWidget *sci;
	GtkNotebook *nb = GTK_NOTEBOOK(main_widgets.notebook);
	gint cur_page = gtk_notebook_get_current_page(nb);
	GeanyDocument *doc = document_get_current();

	if (doc == NULL)
		return;

	sci = GTK_WIDGET(doc->editor->sci);

	switch (key_id)
	{
		case GEANY_KEYS_NOTEBOOK_MOVETABLEFT:
			gtk_notebook_reorder_child(nb, sci, cur_page - 1);	/* notebook wraps around by default */
			break;
		case GEANY_KEYS_NOTEBOOK_MOVETABRIGHT:
		{
			gint npage = cur_page + 1;

			if (npage == gtk_notebook_get_n_pages(nb))
				npage = 0;	/* wraparound */
			gtk_notebook_reorder_child(nb, sci, npage);
			break;
		}
		case GEANY_KEYS_NOTEBOOK_MOVETABFIRST:
			gtk_notebook_reorder_child(nb, sci, (file_prefs.tab_order_ltr) ? 0 : -1);
			break;
		case GEANY_KEYS_NOTEBOOK_MOVETABLAST:
			gtk_notebook_reorder_child(nb, sci, (file_prefs.tab_order_ltr) ? -1 : 0);
			break;
	}
	return;
}


static void goto_matching_brace(GeanyDocument *doc)
{
	gint pos, new_pos;
	gint after_brace;

	if (doc == NULL)
		return;

	pos = sci_get_current_position(doc->editor->sci);
	after_brace = pos > 0 && utils_isbrace(sci_get_char_at(doc->editor->sci, pos - 1), TRUE);
	pos -= after_brace;	/* set pos to the brace */

	new_pos = sci_find_matching_brace(doc->editor->sci, pos);
	if (new_pos != -1)
	{	/* set the cursor at/after the brace */
		sci_set_current_position(doc->editor->sci, new_pos + (!after_brace), FALSE);
		editor_display_current_line(doc->editor, 0.5F);
	}
}


static gboolean cb_func_clipboard_action(guint key_id)
{
	GeanyDocument *doc = document_get_current();

	if (doc == NULL)
		return TRUE;

	switch (key_id)
	{
		case GEANY_KEYS_CLIPBOARD_CUT:
			on_cut1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_CLIPBOARD_COPY:
			on_copy1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_CLIPBOARD_PASTE:
			on_paste1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_CLIPBOARD_COPYLINE:
			sci_send_command(doc->editor->sci, SCI_LINECOPY);
			break;
		case GEANY_KEYS_CLIPBOARD_CUTLINE:
			sci_send_command(doc->editor->sci, SCI_LINECUT);
			break;
	}
	return TRUE;
}


static void goto_tag(GeanyDocument *doc, gboolean definition)
{
	gchar *text = get_current_word_or_sel(doc, FALSE);

	if (text)
		symbols_goto_tag(text, definition);
	else
		utils_beep();

	g_free(text);
}


/* Common function for goto keybindings, useful even when sci doesn't have focus. */
static gboolean cb_func_goto_action(guint key_id)
{
	gint cur_line;
	GeanyDocument *doc = document_get_current();

	if (doc == NULL)
		return TRUE;

	cur_line = sci_get_current_line(doc->editor->sci);

	switch (key_id)
	{
		case GEANY_KEYS_GOTO_BACK:
			navqueue_go_back();
			return TRUE;
		case GEANY_KEYS_GOTO_FORWARD:
			navqueue_go_forward();
			return TRUE;
		case GEANY_KEYS_GOTO_LINE:
		{
			if (toolbar_prefs.visible)
			{
				GtkWidget *wid = toolbar_get_widget_child_by_name("GotoEntry");

				/* use toolbar item if shown & not in the drop down overflow menu */
				if (wid && GTK_WIDGET_MAPPED(wid))
				{
					gtk_widget_grab_focus(wid);
					return TRUE;
				}
			}
			on_go_to_line_activate(NULL, NULL);
			return TRUE;
		}
		case GEANY_KEYS_GOTO_MATCHINGBRACE:
			goto_matching_brace(doc);
			return TRUE;
		case GEANY_KEYS_GOTO_TOGGLEMARKER:
		{
			sci_toggle_marker_at_line(doc->editor->sci, cur_line, 1);
			return TRUE;
		}
		case GEANY_KEYS_GOTO_NEXTMARKER:
		{
			gint mline = sci_marker_next(doc->editor->sci, cur_line + 1, 1 << 1, TRUE);

			if (mline != -1)
			{
				sci_set_current_line(doc->editor->sci, mline);
				editor_display_current_line(doc->editor, 0.5F);
			}
			return TRUE;
		}
		case GEANY_KEYS_GOTO_PREVIOUSMARKER:
		{
			gint mline = sci_marker_previous(doc->editor->sci, cur_line - 1, 1 << 1, TRUE);

			if (mline != -1)
			{
				sci_set_current_line(doc->editor->sci, mline);
				editor_display_current_line(doc->editor, 0.5F);
			}
			return TRUE;
		}
		case GEANY_KEYS_GOTO_TAGDEFINITION:
			goto_tag(doc, TRUE);
			return TRUE;
		case GEANY_KEYS_GOTO_TAGDECLARATION:
			goto_tag(doc, FALSE);
			return TRUE;
	}
	/* only check editor-sensitive keybindings when editor has focus so home,end still
	 * work in other widgets */
	if (gtk_window_get_focus(GTK_WINDOW(main_widgets.window)) != GTK_WIDGET(doc->editor->sci))
		return FALSE;

	switch (key_id)
	{
		case GEANY_KEYS_GOTO_LINESTART:
			sci_send_command(doc->editor->sci, editor_prefs.smart_home_key ? SCI_VCHOME : SCI_HOME);
			break;
		case GEANY_KEYS_GOTO_LINEEND:
			sci_send_command(doc->editor->sci, SCI_LINEEND);
			break;
		case GEANY_KEYS_GOTO_LINEENDVISUAL:
			sci_send_command(doc->editor->sci, SCI_LINEENDDISPLAY);
			break;
		case GEANY_KEYS_GOTO_PREVWORDPART:
			sci_send_command(doc->editor->sci, SCI_WORDPARTLEFT);
			break;
		case GEANY_KEYS_GOTO_NEXTWORDPART:
			sci_send_command(doc->editor->sci, SCI_WORDPARTRIGHT);
			break;
	}
	return TRUE;
}


static void duplicate_lines(GeanyEditor *editor)
{
	if (sci_get_lines_selected(editor->sci) > 1)
	{	/* ignore extra_line because of selecting lines from the line number column */
		editor_select_lines(editor, FALSE);
		sci_selection_duplicate(editor->sci);
	}
	else if (sci_has_selection(editor->sci))
		sci_selection_duplicate(editor->sci);
	else
		sci_line_duplicate(editor->sci);
}


static void delete_lines(GeanyEditor *editor)
{
	editor_select_lines(editor, TRUE); /* include last line (like cut lines, copy lines do) */
	sci_clear(editor->sci);	/* (SCI_LINEDELETE only does 1 line) */
}


static void move_lines(GeanyEditor *editor, gboolean down)
{
	ScintillaObject *sci = editor->sci;
	gchar *text;
	gint pos, line, len;

	sci_start_undo_action(sci);
	editor_select_lines(editor, FALSE);
	len = sci_get_selected_text_length(sci);

	pos = sci_get_selection_start(sci);
	line = sci_get_line_from_position(sci, pos);
	if (down)
		line++;
	else
		line--;

	text = sci_get_selection_contents(sci);
	sci_clear(sci);

	pos = sci_get_position_from_line(sci, line);
	sci_insert_text(sci, pos, text);
	g_free(text);

	sci_set_current_position(sci, pos, TRUE);
	sci_set_selection_end(sci, pos + len - 1);

	sci_end_undo_action(sci);
}


/* common function for editor keybindings, only valid when scintilla has focus. */
static gboolean cb_func_editor_action(guint key_id)
{
	GeanyDocument *doc = document_get_current();
	GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));

	/* edit keybindings only valid when scintilla widget has focus */
	if (doc == NULL || focusw != GTK_WIDGET(doc->editor->sci))
		return FALSE; /* also makes tab work outside editor */

	switch (key_id)
	{
		case GEANY_KEYS_EDITOR_UNDO:
			on_undo1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_EDITOR_REDO:
			on_redo1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_EDITOR_SCROLLTOLINE:
			editor_scroll_to_line(doc->editor, -1, 0.5F);
			break;
		case GEANY_KEYS_EDITOR_SCROLLLINEUP:
			sci_send_command(doc->editor->sci, SCI_LINESCROLLUP);
			break;
		case GEANY_KEYS_EDITOR_SCROLLLINEDOWN:
			sci_send_command(doc->editor->sci, SCI_LINESCROLLDOWN);
			break;
		case GEANY_KEYS_EDITOR_DUPLICATELINE:
			duplicate_lines(doc->editor);
			break;
		case GEANY_KEYS_EDITOR_SNIPPETNEXTCURSOR:
			editor_goto_next_snippet_cursor(doc->editor);
			break;
		case GEANY_KEYS_EDITOR_DELETELINE:
			delete_lines(doc->editor);
			break;
		case GEANY_KEYS_EDITOR_DELETELINETOEND:
			sci_send_command(doc->editor->sci, SCI_DELLINERIGHT);
			break;
		case GEANY_KEYS_EDITOR_TRANSPOSELINE:
			sci_send_command(doc->editor->sci, SCI_LINETRANSPOSE);
			break;
		case GEANY_KEYS_EDITOR_AUTOCOMPLETE:
			editor_start_auto_complete(doc->editor, sci_get_current_position(doc->editor->sci), TRUE);
			break;
		case GEANY_KEYS_EDITOR_CALLTIP:
			editor_show_calltip(doc->editor, -1);
			break;
		case GEANY_KEYS_EDITOR_MACROLIST:
			editor_show_macro_list(doc->editor);
			break;
		case GEANY_KEYS_EDITOR_CONTEXTACTION:
			if (check_current_word(doc, FALSE))
				on_context_action1_activate(GTK_MENU_ITEM(ui_lookup_widget(main_widgets.editor_menu,
					"context_action1")), NULL);
			break;
		case GEANY_KEYS_EDITOR_COMPLETESNIPPET:
			/* allow tab to be overloaded */
			return check_snippet_completion(doc);

		case GEANY_KEYS_EDITOR_SUPPRESSSNIPPETCOMPLETION:
		{
			GeanyKeyBinding *kb = keybindings_lookup_item(GEANY_KEY_GROUP_EDITOR,
				GEANY_KEYS_EDITOR_COMPLETESNIPPET);

			switch (kb->key)
			{
				case GDK_space:
					sci_add_text(doc->editor->sci, " ");
					break;
				case GDK_Tab:
					sci_send_command(doc->editor->sci, SCI_TAB);
					break;
				default:
					break;
			}
			break;
		}
		case GEANY_KEYS_EDITOR_WORDPARTCOMPLETION:
			return editor_complete_word_part(doc->editor);

		case GEANY_KEYS_EDITOR_MOVELINEUP:
			move_lines(doc->editor, FALSE);
			break;
		case GEANY_KEYS_EDITOR_MOVELINEDOWN:
			move_lines(doc->editor, TRUE);
			break;
	}
	return TRUE;
}


static void join_lines(GeanyEditor *editor)
{
	gint start, end, i;

	start = sci_get_line_from_position(editor->sci,
		sci_get_selection_start(editor->sci));
	end = sci_get_line_from_position(editor->sci,
		sci_get_selection_end(editor->sci));

	/* if there is only one line in selection, join it with the following one */
	if (end == start)
		end = start + 1;

	/*
	 * remove trailing spaces for every line except the last one
	 * so that these spaces won't appear within text after joining
	 */
	for (i = start; i < end; i++)
		editor_strip_line_trailing_spaces(editor, i);

	/* remove starting spaces from second and following lines due to the same reason */
	for (i = start + 1; i <= end; i++)
		sci_set_line_indentation(editor->sci, i, 0);

	/*
	 * SCI_LINESJOIN automatically adds spaces between joined lines, including
	 * empty ones. We should drop empty lines if we want only one space to be
	 * inserted (see also example below). I don't think we should care of that.
	 */

	sci_set_target_start(editor->sci,
		sci_get_position_from_line(editor->sci, start));
	sci_set_target_end(editor->sci,
		sci_get_position_from_line(editor->sci, end));
	sci_lines_join(editor->sci);

	/*
	 * Example: joining
	 *
	 * [TAB]if (something_wrong)
	 * [TAB]{
	 * [TAB][TAB]
	 * [TAB][TAB]exit(1);[SPACE][SPACE]
	 * [TAB]}[SPACE]
	 *
	 * gives
	 *
	 * [TAB]if (something_wrong) {  exit(1); }[SPACE]
	 */
}


static void split_lines(GeanyEditor *editor, gint column)
{
	gint start, indent, linescount, i, end;
	gchar c;
	ScintillaObject *sci = editor->sci;

	/* don't include trailing newlines */
	end = sci_get_selection_end(sci);
	while ((c = sci_get_char_at(sci, end - 1)) == '\n' || c == '\r') end--;
	sci_set_selection_end(sci, end);

	start = sci_get_line_from_position(editor->sci,
		sci_get_selection_start(editor->sci));

	/*
	 * If several lines are selected, first join them.
	 * This allows to reformat text paragraphs easily.
	 */
	if (sci_get_lines_selected(editor->sci) > 1)
		join_lines(editor);

	/*
	 * If this line is short enough, just return
	 */
	if (column > sci_get_line_end_position(editor->sci, start) -
		sci_get_position_from_line(editor->sci, start))
	{
		return;
	}

	/*
	 * We have to manipulate line indentation so that indentation
	 * of the resulting lines would be consistent. For example,
	 * the result of splitting "[TAB]very long content":
	 *
	 * +-------------+-------------+
	 * |   proper    |    wrong    |
	 * +-------------+-------------+
	 * | [TAB]very   | [TAB]very   |
	 * | [TAB]long   | long        |
	 * | [TAB]content| content     |
	 * +-------------+-------------+
	 */
	indent = sci_get_line_indentation(editor->sci, start);
	sci_set_line_indentation(editor->sci, start, 0);

	/*
	 * Use sci_get_line_count() to determine how many new lines
	 * appeared during splitting. SCI_LINESSPLIT should better return
	 * this value itself...
	 */
	sci_target_from_selection(editor->sci);
	linescount = sci_get_line_count(editor->sci);
	sci_lines_split(editor->sci,
		(column - indent) *	sci_text_width(editor->sci, STYLE_DEFAULT, " "));
	linescount = sci_get_line_count(editor->sci) - linescount;

	/* Fix indentation. */
	for (i = start; i <= start + linescount; i++)
		sci_set_line_indentation(editor->sci, i, indent);

	/* Remove trailing spaces. */
	if (editor_prefs.newline_strip || file_prefs.strip_trailing_spaces)
	{
		for (i = start; i <= start + linescount; i++)
			editor_strip_line_trailing_spaces(editor, i);
	}
}


/* if cursor < anchor, swap them */
static void sci_fix_selection(ScintillaObject *sci)
{
	gint start, end;

	start = sci_get_selection_start(sci);
	end = sci_get_selection_end(sci);
	sci_set_selection(sci, start, end);
}


static void reflow_paragraph(GeanyEditor *editor)
{
	ScintillaObject *sci = editor->sci;
	gboolean sel;
	gint column = -1;
	const GeanyEditorPrefs *eprefs = editor_get_prefs(editor);

	if (editor->line_breaking)
	{
		/* use line break column if enabled */
		column = eprefs->line_break_column;
	}
	else if (eprefs->long_line_type != 2)
	{
		/* use long line if enabled */
		column = eprefs->long_line_column;
	}
	else
	{
		/* do nothing if no column is defined */
		utils_beep();
		return;
	}
	sci_start_undo_action(sci);
	sel = sci_has_selection(sci);
	if (!sel)
	{
		gint line, pos;

		editor_select_indent_block(editor);

		/* deselect last line break */
		pos = sci_get_selection_end(sci);
		line = sci_get_line_from_position(sci, pos);
		if (line < sci_get_line_count(sci) - 1)
		{
			/* not last line */
			pos = sci_get_line_end_position(sci, line - 1);
			sci_set_selection_end(sci, pos);
		}
	}
	sci_fix_selection(sci);
	split_lines(editor, column);
	if (!sel)
		sci_set_anchor(sci, -1);

	sci_end_undo_action(sci);
}


/* common function for format keybindings, only valid when scintilla has focus. */
static gboolean cb_func_format_action(guint key_id)
{
	GeanyDocument *doc = document_get_current();
	GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));

	/* keybindings only valid when scintilla widget has focus */
	if (doc == NULL || focusw != GTK_WIDGET(doc->editor->sci))
		return TRUE;

	switch (key_id)
	{
		case GEANY_KEYS_FORMAT_COMMENTLINETOGGLE:
			on_menu_toggle_line_commentation1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_COMMENTLINE:
			on_menu_comment_line1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_UNCOMMENTLINE:
			on_menu_uncomment_line1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_INCREASEINDENT:
			on_menu_increase_indent1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_DECREASEINDENT:
			on_menu_decrease_indent1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_INCREASEINDENTBYSPACE:
			editor_indentation_by_one_space(doc->editor, -1, FALSE);
			break;
		case GEANY_KEYS_FORMAT_DECREASEINDENTBYSPACE:
			editor_indentation_by_one_space(doc->editor, -1, TRUE);
			break;
		case GEANY_KEYS_FORMAT_AUTOINDENT:
			editor_smart_line_indentation(doc->editor, -1);
			break;
		case GEANY_KEYS_FORMAT_TOGGLECASE:
			on_toggle_case1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_SENDTOCMD1:
			if (ui_prefs.custom_commands && g_strv_length(ui_prefs.custom_commands) > 0)
				tools_execute_custom_command(doc, ui_prefs.custom_commands[0]);
			break;
		case GEANY_KEYS_FORMAT_SENDTOCMD2:
			if (ui_prefs.custom_commands && g_strv_length(ui_prefs.custom_commands) > 1)
				tools_execute_custom_command(doc, ui_prefs.custom_commands[1]);
			break;
		case GEANY_KEYS_FORMAT_SENDTOCMD3:
			if (ui_prefs.custom_commands && g_strv_length(ui_prefs.custom_commands) > 2)
				tools_execute_custom_command(doc, ui_prefs.custom_commands[2]);
			break;
		case GEANY_KEYS_FORMAT_SENDTOVTE:
			on_send_selection_to_vte1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_FORMAT_REFLOWPARAGRAPH:
			reflow_paragraph(doc->editor);
			break;
	}
	return TRUE;
}


/* common function for select keybindings, only valid when scintilla has focus. */
static gboolean cb_func_select_action(guint key_id)
{
	GeanyDocument *doc;
	ScintillaObject *sci;
	GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));
	GtkWidget *toolbar_search_entry = toolbar_get_widget_child_by_name("SearchEntry");
	GtkWidget *toolbar_goto_entry = toolbar_get_widget_child_by_name("GotoEntry");

	/* special case for Select All in the scribble widget */
	if (key_id == GEANY_KEYS_SELECT_ALL && focusw == msgwindow.scribble)
	{
		g_signal_emit_by_name(msgwindow.scribble, "select-all", TRUE);
		return TRUE;
	}
	/* special case for Select All in the VTE widget */
#ifdef HAVE_VTE
	else if (key_id == GEANY_KEYS_SELECT_ALL && vte_info.have_vte && focusw == vc->vte)
	{
		vte_select_all();
		return TRUE;
	}
#endif
	/* special case for Select All in the toolbar search widget */
	else if (key_id == GEANY_KEYS_SELECT_ALL && focusw == toolbar_search_entry)
	{
		gtk_editable_select_region(GTK_EDITABLE(toolbar_search_entry), 0, -1);
		return TRUE;
	}
	else if (key_id == GEANY_KEYS_SELECT_ALL && focusw == toolbar_goto_entry)
	{
		gtk_editable_select_region(GTK_EDITABLE(toolbar_goto_entry), 0, -1);
		return TRUE;
	}

	doc = document_get_current();
	/* keybindings only valid when scintilla widget has focus */
	if (doc == NULL || focusw != GTK_WIDGET(doc->editor->sci))
		return TRUE;
	sci = doc->editor->sci;

	switch (key_id)
	{
		case GEANY_KEYS_SELECT_ALL:
			on_menu_select_all1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_SELECT_WORD:
			editor_select_word(doc->editor);
			break;
		case GEANY_KEYS_SELECT_LINE:
			editor_select_lines(doc->editor, FALSE);
			break;
		case GEANY_KEYS_SELECT_PARAGRAPH:
			editor_select_paragraph(doc->editor);
			break;
		case GEANY_KEYS_SELECT_WORDPARTLEFT:
			sci_send_command(sci, SCI_WORDPARTLEFTEXTEND);
			break;
		case GEANY_KEYS_SELECT_WORDPARTRIGHT:
			sci_send_command(sci, SCI_WORDPARTRIGHTEXTEND);
			break;
	}
	return TRUE;
}


static gboolean cb_func_document_action(guint key_id)
{
	GeanyDocument *doc = document_get_current();

	if (doc == NULL)
		return TRUE;

	switch (key_id)
	{
		case GEANY_KEYS_DOCUMENT_REPLACETABS:
			on_replace_tabs_activate(NULL, NULL);
			break;
		case GEANY_KEYS_DOCUMENT_REPLACESPACES:
			on_replace_spaces_activate(NULL, NULL);
			break;
		case GEANY_KEYS_DOCUMENT_LINEBREAK:
			on_line_breaking1_activate(NULL, NULL);
			ui_document_show_hide(doc);
			break;
		case GEANY_KEYS_DOCUMENT_LINEWRAP:
			on_line_wrapping1_toggled(NULL, NULL);
			ui_document_show_hide(doc);
			break;
		case GEANY_KEYS_DOCUMENT_RELOADTAGLIST:
			document_update_tag_list(doc, TRUE);
			break;
		case GEANY_KEYS_DOCUMENT_FOLDALL:
			editor_fold_all(doc->editor);
			break;
		case GEANY_KEYS_DOCUMENT_UNFOLDALL:
			editor_unfold_all(doc->editor);
			break;
		case GEANY_KEYS_DOCUMENT_TOGGLEFOLD:
			if (editor_prefs.folding)
			{
				gint line = sci_get_current_line(doc->editor->sci);
				editor_toggle_fold(doc->editor, line, 0);
				break;
			}
		case GEANY_KEYS_DOCUMENT_REMOVE_MARKERS:
			on_remove_markers1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_DOCUMENT_REMOVE_ERROR_INDICATORS:
			on_menu_remove_indicators1_activate(NULL, NULL);
			break;
		case GEANY_KEYS_DOCUMENT_REMOVE_MARKERS_INDICATORS:
			on_remove_markers1_activate(NULL, NULL);
			on_menu_remove_indicators1_activate(NULL, NULL);
			break;
	}
	return TRUE;
}


static void insert_line_after(GeanyEditor *editor)
{
	ScintillaObject *sci = editor->sci;

	sci_send_command(sci, SCI_LINEEND);
	sci_send_command(sci, SCI_NEWLINE);
}


static void insert_line_before(GeanyEditor *editor)
{
	ScintillaObject *sci = editor->sci;
	gint line = sci_get_current_line(sci);
	gint indentpos = sci_get_line_indent_position(sci, line);

	sci_set_current_position(sci, indentpos, TRUE);
	sci_send_command(sci, SCI_NEWLINE);
	sci_send_command(sci, SCI_LINEUP);
}


/* common function for insert keybindings, only valid when scintilla has focus. */
static gboolean cb_func_insert_action(guint key_id)
{
	GeanyDocument *doc = document_get_current();
	GtkWidget *focusw = gtk_window_get_focus(GTK_WINDOW(main_widgets.window));

	/* keybindings only valid when scintilla widget has focus */
	if (doc == NULL || focusw != GTK_WIDGET(doc->editor->sci))
		return TRUE;

	switch (key_id)
	{
		case GEANY_KEYS_INSERT_ALTWHITESPACE:
			editor_insert_alternative_whitespace(doc->editor);
			break;
		case GEANY_KEYS_INSERT_DATE:
			gtk_menu_item_activate(GTK_MENU_ITEM(
				ui_lookup_widget(main_widgets.window, "insert_date_custom1")));
			break;
		case GEANY_KEYS_INSERT_LINEAFTER:
			insert_line_after(doc->editor);
			break;
		case GEANY_KEYS_INSERT_LINEBEFORE:
			insert_line_before(doc->editor);
			break;
	}
	return TRUE;
}


/* update key combination */
void keybindings_update_combo(GeanyKeyBinding *kb, guint key, GdkModifierType mods)
{
	GtkWidget *widget = kb->menu_item;

	if (widget && kb->key)
		gtk_widget_remove_accelerator(widget, kb_accel_group, kb->key, kb->mods);

	kb->key = key;
	kb->mods = mods;

	if (widget && kb->key)
		gtk_widget_add_accelerator(widget, "activate", kb_accel_group,
			kb->key, kb->mods, GTK_ACCEL_VISIBLE);
}


/* used for plugins, can be called repeatedly. */
GeanyKeyGroup *keybindings_set_group(GeanyKeyGroup *group, const gchar *section_name,
		const gchar *label, gsize count, GeanyKeyGroupCallback callback)
{
	g_return_val_if_fail(section_name, NULL);
	g_return_val_if_fail(count, NULL);

	/* prevent conflict with core bindings */
	g_return_val_if_fail(!g_str_equal(section_name, keybindings_keyfile_group_name), NULL);

	if (!group)
	{
		group = g_new0(GeanyKeyGroup, 1);
		add_kb_group(group, section_name, label, callback, TRUE);
	}
	g_free(group->plugin_keys);
	group->plugin_keys = g_new0(GeanyKeyBinding, count);
	group->plugin_key_count = count;
	g_ptr_array_set_size(group->key_items, 0);
	return group;
}


void keybindings_free_group(GeanyKeyGroup *group)
{
	GeanyKeyBinding *kb;

	g_ptr_array_free(group->key_items, TRUE);

	if (group->plugin)
	{
		foreach_c_array(kb, group->plugin_keys, group->plugin_key_count)
		{
			g_free(kb->name);
			g_free(kb->label);
		}
		g_free(group->plugin_keys);
		g_ptr_array_remove_fast(keybinding_groups, group);
		g_free(group);
	}
}
