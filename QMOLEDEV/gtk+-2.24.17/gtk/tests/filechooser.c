/* GTK - The GIMP Toolkit
 * autotestfilechooser.c: Automated unit tests for the GtkFileChooser widget
 * Copyright (C) 2005, Novell, Inc.
 *
 * Authors:
 *   Federico Mena-Quintero <federico@novell.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* TODO:
 *
 * - In test_reload_sequence(), test that the selection is preserved properly
 *   between unmap/map.
 *
 * - More tests!
 */

#define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED
#undef GTK_DISABLE_DEPRECATED
#define SLEEP_DURATION  100

#include "config.h"
#include <string.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include "gtk/gtkfilechooserprivate.h"
#include "gtk/gtkfilechooserdefault.h"
#include "gtk/gtkfilechooserentry.h"

static void
log_test (gboolean passed, const char *test_name, ...)
{
  va_list args;
  char *str;

  va_start (args, test_name);
  str = g_strdup_vprintf (test_name, args);
  va_end (args);

  if (g_test_verbose())
    g_printf ("%s: %s\n", passed ? "PASSED" : "FAILED", str);
  g_free (str);
}

static const char *get_action_name (GtkFileChooserAction action);

typedef void (* SetFilenameFn) (GtkFileChooser *chooser, gpointer data);
typedef void (* CompareFilenameFn) (GtkFileChooser *chooser, gpointer data);

struct test_set_filename_closure {
  GtkWidget *chooser;
  GtkWidget *accept_button;
  gboolean focus_button;
};

static gboolean
set_filename_timeout_cb (gpointer data)
{
  struct test_set_filename_closure *closure;

  closure = data;

  if (closure->focus_button)
    gtk_widget_grab_focus (closure->accept_button);

  gtk_button_clicked (GTK_BUTTON (closure->accept_button));

  return FALSE;
}


static guint wait_for_idle_id = 0;

static gboolean
wait_for_idle_idle (gpointer data)
{
  wait_for_idle_id = 0;

  return FALSE;
}

static void
wait_for_idle (void)
{
  wait_for_idle_id = g_idle_add_full (G_PRIORITY_LOW + 100,
				      wait_for_idle_idle,
				      NULL, NULL);

  while (wait_for_idle_id)
    gtk_main_iteration ();
}

static void
test_set_filename (GtkFileChooserAction action,
		   gboolean focus_button,
		   SetFilenameFn set_filename_fn,const
		   CompareFilenameFn compare_filename_fn,
		   gpointer data)
{
  GtkWidget *chooser;
  struct test_set_filename_closure closure;

  chooser = gtk_file_chooser_dialog_new ("hello", NULL, action,
					 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					 NULL);

  closure.chooser = chooser;
  closure.accept_button = gtk_dialog_add_button (GTK_DIALOG (chooser), GTK_STOCK_OK, GTK_RESPONSE_ACCEPT);
  closure.focus_button = focus_button;

  gtk_dialog_set_default_response (GTK_DIALOG (chooser), GTK_RESPONSE_ACCEPT);

  (* set_filename_fn) (GTK_FILE_CHOOSER (chooser), data);

  gdk_threads_add_timeout_full (G_MAXINT, SLEEP_DURATION, set_filename_timeout_cb, &closure, NULL);
  gtk_dialog_run (GTK_DIALOG (chooser));

  (* compare_filename_fn) (GTK_FILE_CHOOSER (chooser), data);

  gtk_widget_destroy (chooser);
}

static void
set_filename_cb (GtkFileChooser *chooser, gpointer data)
{
  const char *filename;

  filename = data;
  gtk_file_chooser_set_filename (chooser, filename);
}

static void
compare_filename_cb (GtkFileChooser *chooser, gpointer data)
{
  const char *filename;
  char *out_filename;

  filename = data;
  out_filename = gtk_file_chooser_get_filename (chooser);

  g_assert_cmpstr (out_filename, ==, filename);

  if (out_filename)
    g_free (out_filename);
}

typedef struct
{
  const char *test_name;
  GtkFileChooserAction action;
  const char *filename;
  gboolean focus_button;
} TestSetFilenameSetup;

static void
test_black_box_set_filename (gconstpointer data)
{
  const TestSetFilenameSetup *setup = data;

  test_set_filename (setup->action, setup->focus_button, set_filename_cb, compare_filename_cb, (char *) setup->filename);
}

struct current_name_closure {
	const char *path;
	const char *current_name;
};

static void
set_current_name_cb (GtkFileChooser *chooser, gpointer data)
{
  struct current_name_closure *closure;

  closure = data;

  gtk_file_chooser_set_current_folder (chooser, closure->path);
  gtk_file_chooser_set_current_name (chooser, closure->current_name);
}

static void
compare_current_name_cb (GtkFileChooser *chooser, gpointer data)
{
  struct current_name_closure *closure;
  char *out_filename;
  char *filename;

  closure = data;

  out_filename = gtk_file_chooser_get_filename (chooser);

  g_assert (out_filename != NULL);

  filename = g_build_filename (closure->path, closure->current_name, NULL);
  g_assert_cmpstr (filename, ==, out_filename);

  g_free (filename);
  g_free (out_filename);
}

typedef struct
{
  const char *test_name;
  GtkFileChooserAction action;
  const char *current_name;
  gboolean focus_button;
} TestSetCurrentNameSetup;

static void
test_black_box_set_current_name (gconstpointer data)
{
  const TestSetCurrentNameSetup *setup = data;
  struct current_name_closure closure;
  char *cwd;

  cwd = g_get_current_dir ();

  closure.path = cwd;
  closure.current_name = setup->current_name;

  test_set_filename (setup->action, setup->focus_button, set_current_name_cb, compare_current_name_cb, &closure);

  g_free (cwd);
}

/* FIXME: fails in CREATE_FOLDER mode when FOLDER_NAME == "/" */

#if 0
#define FILE_NAME "/nonexistent"
#define FILE_NAME_2 "/nonexistent2"
#define FOLDER_NAME "/etc"
#define FOLDER_NAME_2 "/usr"
#else
#define FILE_NAME "/etc/passwd"
#define FILE_NAME_2 "/etc/group"
#define FOLDER_NAME "/etc"
#define FOLDER_NAME_2 "/usr"
#endif

#define CURRENT_NAME "parangaricutirimicuaro.txt"
#define CURRENT_NAME_FOLDER "parangaricutirimicuaro"

/* https://bugzilla.novell.com/show_bug.cgi?id=184875
 * http://bugzilla.gnome.org/show_bug.cgi?id=347066
 * http://bugzilla.gnome.org/show_bug.cgi?id=346058
 */

static void
setup_set_filename_tests (void)
{
  static TestSetFilenameSetup tests[] =
    {
      { "/GtkFileChooser/black_box/set_filename/open/no_focus",		 GTK_FILE_CHOOSER_ACTION_OPEN,		FILE_NAME,  FALSE },
      { "/GtkFileChooser/black_box/set_filename/open/focus",		 GTK_FILE_CHOOSER_ACTION_OPEN,		FILE_NAME,  TRUE  },
      { "/GtkFileChooser/black_box/set_filename/save/no_focus",		 GTK_FILE_CHOOSER_ACTION_SAVE,		FILE_NAME,  FALSE },
      { "/GtkFileChooser/black_box/set_filename/save/focus",		 GTK_FILE_CHOOSER_ACTION_SAVE,		FILE_NAME,  TRUE  },
      { "/GtkFileChooser/black_box/set_filename/select_folder/no_focus", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,	FOLDER_NAME,FALSE },
      { "/GtkFileChooser/black_box/set_filename/select_folder/focus",	 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,	FOLDER_NAME,TRUE  },
      { "/GtkFileChooser/black_box/set_filename/create_folder/no_focus", GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER,	FOLDER_NAME,FALSE },
      { "/GtkFileChooser/black_box/set_filename/create_folder/focus",	 GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER,	FOLDER_NAME,TRUE  },
    };
  int i;

  for (i = 0; i < G_N_ELEMENTS (tests); i++)
    g_test_add_data_func (tests[i].test_name, &tests[i], test_black_box_set_filename);
}

static void
setup_set_current_name_tests (void)
{
  static TestSetCurrentNameSetup tests[] =
    {
      { "/GtkFileChooser/black_box/set_current_name/save/no_focus",	     GTK_FILE_CHOOSER_ACTION_SAVE,	    CURRENT_NAME,	 FALSE },
      { "/GtkFileChooser/black_box/set_current_name/save/focus",	     GTK_FILE_CHOOSER_ACTION_SAVE,	    CURRENT_NAME,	 TRUE  },
      { "/GtkFileChooser/black_box/set_current_name/create_folder/no_focus", GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, CURRENT_NAME_FOLDER, FALSE },
      { "/GtkFileChooser/black_box/set_current_name/create_folder/focus",    GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER, CURRENT_NAME_FOLDER, TRUE  },
    };
  int i;

  for (i = 0; i < G_N_ELEMENTS (tests); i++)
    g_test_add_data_func (tests[i].test_name, &tests[i], test_black_box_set_current_name);
}

typedef struct
{
  const char *shortname;
  GtkFileChooserAction action;
  const char *initial_current_folder;
  const char *initial_filename;
  gboolean open_dialog;
  const char *tweak_current_folder;
  const char *tweak_filename;
  gint dialog_response;
  const char *final_current_folder;
  const char *final_filename;
} FileChooserButtonTest;

static char *
make_button_test_name (FileChooserButtonTest *t)
{
  return g_strdup_printf ("/GtkFileChooserButton/%s", t->shortname);
#if 0
  GString *s = g_string_new ("/GtkFileChooserButton");

  g_string_append_printf (s, "/%s/%s/%s/%s",
			  get_action_name (t->action),
			  t->initial_current_folder ? "set_initial_folder" : "no_default_folder",
			  t->initial_filename ? "set_initial_filename" : "no_initial_filename",
			  t->open_dialog ? "open_dialog" : "no_dialog");

  if (t->tweak_current_folder)
    g_string_append (s, "/tweak_current_folder");

  if (t->tweak_filename)
    g_string_append (s, "/tweak_filename");

  if (t->open_dialog)
    g_string_append_printf (s, "/%s",
			    t->dialog_response == GTK_RESPONSE_ACCEPT ? "accept" : "cancel");

  if (t->final_current_folder)
    g_string_append (s, "/final_current_folder");

  if (t->final_filename)
    g_string_append (s, "/final_filename");

  return g_string_free (s, FALSE);
#endif
}

/* Copied from gtkfilechooserutils.c:_gtk_file_chooser_delegate_get_quark() */
static GQuark
delegate_get_quark (void)
{
  static GQuark quark = 0;

  if (G_UNLIKELY (quark == 0))
    quark = g_quark_from_static_string ("gtk-file-chooser-delegate");

  return quark;
}

static gboolean
sleep_timeout_cb (gpointer data)
{
  gtk_main_quit ();
  return FALSE;
}

static void
sleep_in_main_loop (void)
{
  /* process all pending idles and events */
  while (g_main_context_pending (NULL))
    g_main_context_iteration (NULL, FALSE);
  /* sleeping probably isn't strictly necessary here */
  gdk_threads_add_timeout_full (G_MAXINT, 250, sleep_timeout_cb, NULL, NULL);
  gtk_main ();
  /* process any pending idles or events that arrived during sleep */
  while (g_main_context_pending (NULL))
    g_main_context_iteration (NULL, FALSE);
}

static void
test_file_chooser_button (gconstpointer data)
{
  const FileChooserButtonTest *setup = data;
  GtkWidget *window;
  GtkWidget *fc_button;
  GtkWidget *fc_dialog;
  int iterations;
  int i;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  fc_button = gtk_file_chooser_button_new (setup->action == GTK_FILE_CHOOSER_ACTION_OPEN ? "Select a file" : "Select a folder",
					   setup->action);
  gtk_container_add (GTK_CONTAINER (window), fc_button);

  if (setup->initial_current_folder)
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (fc_button), setup->initial_current_folder);

  if (setup->initial_filename)
    gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fc_button), setup->initial_filename);

  gtk_widget_show_all (window);
  wait_for_idle ();

  /* If there is a dialog to be opened, we actually test going through it a
   * couple of times.  This ensures that any state that the button frobs for
   * each appearance of the dialog will make sense.
   */
  if (setup->open_dialog)
    iterations = 2;
  else
    iterations = 1;

  for (i = 0; i < iterations; i++)
    {
      if (setup->open_dialog)
	{
	  GList *children;

	  /* Hack our way into the file chooser button; get its GtkButton child and click it */
	  children = gtk_container_get_children (GTK_CONTAINER (fc_button));
	  g_assert (children && GTK_IS_BUTTON (children->data));
	  gtk_button_clicked (GTK_BUTTON (children->data));
	  g_list_free (children);

	  sleep_in_main_loop ();

	  /* Give me the internal dialog, damnit */
	  fc_dialog = g_object_get_qdata (G_OBJECT (fc_button), delegate_get_quark ());
	  g_assert (GTK_IS_FILE_CHOOSER (fc_dialog));
	  g_assert (GTK_IS_DIALOG (fc_dialog));
	}

      /* Okay, now frob the button and its optional dialog */

      if (setup->tweak_current_folder)
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (fc_button), setup->tweak_current_folder);

      if (setup->tweak_filename)
	gtk_file_chooser_select_filename (GTK_FILE_CHOOSER (fc_button), setup->tweak_filename);

      sleep_in_main_loop ();

      if (setup->open_dialog)
	{
	  gtk_dialog_response (GTK_DIALOG (fc_dialog), setup->dialog_response);
	  wait_for_idle ();
	}

      if (setup->final_current_folder)
	{
	  char *folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (fc_button));

	  g_assert_cmpstr (folder, ==, setup->final_current_folder);
	  g_free (folder);
	}

      if (setup->final_filename)
	{
	  char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fc_button));

	  g_assert_cmpstr (filename, ==, setup->final_filename);
	  g_free (filename);
	}
    }

  gtk_widget_destroy (window);
}

static FileChooserButtonTest button_tests[] =
  {
    /* OPEN tests without dialog */

    {
      "open-1",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL, 			/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-2",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME, 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-3",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME,		/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-4",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME, 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME_2,		/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME_2		/* final_filename */
    },
    {
      "open-5",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-6",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      FOLDER_NAME_2,		/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      FOLDER_NAME_2,		/* final_current_folder */
      NULL			/* final_filename */
    },

    /* SELECT_FOLDER tests without dialog */

    {
      "select-folder-1",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      NULL, 			/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-2",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME, 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-3",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME, 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME_2		/* final_filename */
    },
    {
      "select-folder-4",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-5",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-6",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      FOLDER_NAME_2,		/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME_2		/* final_filename */
    },
    {
      "select-folder-7",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      FOLDER_NAME_2,		/* tweak_current_folder */
      NULL,			/* tweak_filename */
      0,			/* dialog_response */
      FOLDER_NAME_2,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-8",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,	 		/* initial_filename */
      FALSE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      0,			/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME_2		/* final_filename */
    },

    /* OPEN tests with dialog, cancelled */

    {
      "open-dialog-cancel-1",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL, 			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-dialog-cancel-2",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-dialog-cancel-3",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-dialog-cancel-4",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-dialog-cancel-5",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-dialog-cancel-6",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },

    /* OPEN tests with dialog, cancelled via closing the dialog (not by selecting the Cancel button) */

    {
      "open-dialog-close-1",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL, 			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-dialog-close-2",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-dialog-close-3",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-dialog-close-4",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "open-dialog-close-5",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-dialog-close-6",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },

    /* SELECT_FOLDER tests with dialog, cancelled */

    {
      "select-folder-dialog-cancel-1",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      NULL, 			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-cancel-2",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-cancel-3",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-cancel-4",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-cancel-5",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-cancel-6",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-cancel-7",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-cancel-8",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_CANCEL,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },

    /* SELECT_FOLDER tests with dialog, cancelled via closing the dialog (not selecting the Cancel button) */

    {
      "select-folder-dialog-close-1",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      NULL, 			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-close-2",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-close-3",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-close-4",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-close-5",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-close-6",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-close-7",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      FOLDER_NAME,		/* final_current_folder */
      NULL			/* final_filename */
    },
    {
      "select-folder-dialog-close-8",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_DELETE_EVENT,/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },

    /* OPEN tests with dialog */

    {
      "open-dialog-1",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME,		/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-dialog-2",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },
    {
      "open-dialog-3",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      NULL,			/* initial_current_folder */
      FILE_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME_2		/* final_filename */
    },
    {
      "open-dialog-4",
      GTK_FILE_CHOOSER_ACTION_OPEN,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FILE_NAME,		/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FILE_NAME			/* final_filename */
    },

    /* SELECT_FOLDER tests with dialog */

    {
      "select-folder-dialog-1",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-2",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      NULL,			/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME		/* final_filename */
    },
    {
      "select-folder-dialog-3",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      NULL,			/* initial_current_folder */
      FOLDER_NAME,		/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME_2		/* final_filename */
    },
    {
      "select-folder-dialog-4",
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      FOLDER_NAME,		/* initial_current_folder */
      NULL,			/* initial_filename */
      TRUE,			/* open_dialog */
      NULL,			/* tweak_current_folder */
      FOLDER_NAME_2,		/* tweak_filename */
      GTK_RESPONSE_ACCEPT,	/* dialog_response */
      NULL,			/* final_current_folder */
      FOLDER_NAME_2		/* final_filename */
    },

  };

static void
setup_file_chooser_button_tests (void)
{
  int i;

  for (i = 0; i < G_N_ELEMENTS (button_tests); i++)
    {
      char *test_name;

      test_name = make_button_test_name (&button_tests[i]);
      g_test_add_data_func (test_name, &button_tests[i], test_file_chooser_button);
      g_free (test_name);
    }
}

struct confirm_overwrite_closure {
  GtkWidget *chooser;
  GtkWidget *accept_button;
  gint confirm_overwrite_signal_emitted;
  gchar *extension;
};

static GtkFileChooserConfirmation
confirm_overwrite_cb (GtkFileChooser *chooser, gpointer data)
{
  struct confirm_overwrite_closure *closure = data;

  if (g_test_verbose())
    printf ("bling!\n");
  closure->confirm_overwrite_signal_emitted += 1;

  return GTK_FILE_CHOOSER_CONFIRMATION_ACCEPT_FILENAME;
}

static void
overwrite_response_cb (GtkFileChooser *chooser, gint response, gpointer data)
{
  struct confirm_overwrite_closure *closure = data;
  char *filename;

  if (g_test_verbose())
    printf ("plong!\n");

  if (response != GTK_RESPONSE_ACCEPT)
    return;

  filename = gtk_file_chooser_get_filename (chooser);

  if (!g_str_has_suffix (filename, closure->extension))
    {
      char *basename;

      basename = g_path_get_basename (filename);
      g_free (filename);

      filename = g_strconcat (basename, closure->extension, NULL);
      gtk_file_chooser_set_current_name (chooser, filename);

      g_signal_stop_emission_by_name (chooser, "response");
      gtk_dialog_response (GTK_DIALOG (chooser), GTK_RESPONSE_ACCEPT);
    }
}

static gboolean
confirm_overwrite_timeout_cb (gpointer data)
{
  struct confirm_overwrite_closure *closure;

  closure = data;
  gtk_button_clicked (GTK_BUTTON (closure->accept_button));

  return FALSE;
}

/* http://bugzilla.gnome.org/show_bug.cgi?id=347883 */
static gboolean
test_confirm_overwrite_for_path (const char *path, gboolean append_extension)
{
  gboolean passed;
  struct confirm_overwrite_closure closure;
  char *filename;

  passed = TRUE;

  closure.extension = NULL;
  closure.confirm_overwrite_signal_emitted = 0;
  closure.chooser = gtk_file_chooser_dialog_new ("hello", NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
						 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						 NULL);
  closure.accept_button = gtk_dialog_add_button (GTK_DIALOG (closure.chooser),
                                                 GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT);
  gtk_dialog_set_default_response (GTK_DIALOG (closure.chooser), GTK_RESPONSE_ACCEPT);

  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (closure.chooser), TRUE);

  g_signal_connect (closure.chooser, "confirm-overwrite",
		    G_CALLBACK (confirm_overwrite_cb), &closure);

  if (append_extension)
    {
      char *extension;

      filename = g_path_get_dirname (path);
      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (closure.chooser), filename);
      g_free (filename);

      filename = g_path_get_basename (path);
      extension = strchr (filename, '.');

      if (extension)
        {
          closure.extension = g_strdup (extension);
          *extension = '\0';
        }

      gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (closure.chooser), filename);
      g_free (filename);

      g_signal_connect (closure.chooser, "response",
                        G_CALLBACK (overwrite_response_cb), &closure);
    }
  else
    {
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (closure.chooser), path);
    }

  gdk_threads_add_timeout_full (G_MAXINT, SLEEP_DURATION, confirm_overwrite_timeout_cb, &closure, NULL);
  gtk_dialog_run (GTK_DIALOG (closure.chooser));

  filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (closure.chooser));
  passed = passed && filename && (strcmp (filename, path) == 0);
  g_free (filename);
  
  gtk_widget_destroy (closure.chooser);

  passed = passed && (1 == closure.confirm_overwrite_signal_emitted);

  log_test (passed, "Confirm overwrite for %s", path);

  return passed;
}

static void
test_confirm_overwrite (void)
{
  gboolean passed = TRUE;

  /* first test for a file we know will always exist */
  passed = passed && test_confirm_overwrite_for_path ("/etc/passwd", FALSE); 
  g_assert (passed);
  passed = passed && test_confirm_overwrite_for_path ("/etc/resolv.conf", TRUE); 
  g_assert (passed);
}

static const GtkFileChooserAction open_actions[] = {
  GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER
};

static const GtkFileChooserAction save_actions[] = {
  GTK_FILE_CHOOSER_ACTION_SAVE,
  GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER
};


static gboolean
has_action (const GtkFileChooserAction *actions,
	    int n_actions,
	    GtkFileChooserAction sought_action)
{
  int i;

  for (i = 0; i < n_actions; i++)
    if (actions[i] == sought_action)
      return TRUE;

  return FALSE;
}

static const char *
get_action_name (GtkFileChooserAction action)
{
  switch (action)
    {
    case GTK_FILE_CHOOSER_ACTION_OPEN:          return "OPEN";
    case GTK_FILE_CHOOSER_ACTION_SAVE:          return "SAVE";
    case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER: return "SELECT_FOLDER";
    case GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER: return "CREATE_FOLDER";

    default:
      g_assert_not_reached ();
      return NULL;
    }
}

static GtkFileChooserDefault *
get_impl_from_dialog (GtkWidget *dialog)
{
  GtkFileChooserDialog *d;
  GtkFileChooserDialogPrivate *dialog_priv;
  GtkFileChooserWidget *chooser_widget;
  GtkFileChooserWidgetPrivate *widget_priv;
  GtkFileChooserDefault *impl;

  d = GTK_FILE_CHOOSER_DIALOG (dialog);
  dialog_priv = d->priv;
  chooser_widget = GTK_FILE_CHOOSER_WIDGET (dialog_priv->widget);
  if (!chooser_widget)
    g_error ("BUG: dialog_priv->widget is not a GtkFileChooserWidget");

  widget_priv = chooser_widget->priv;
  impl = (GtkFileChooserDefault *) (widget_priv->impl);
  if (!impl)
    g_error ("BUG: widget_priv->impl is not a GtkFileChooserDefault");

  return impl;
}
#ifdef BROKEN_TESTS
static gboolean
test_widgets_for_current_action (GtkFileChooserDialog *dialog,
				 GtkFileChooserAction  expected_action)
{
  GtkFileChooserDefault *impl;
  gboolean passed;

  if (gtk_file_chooser_get_action (GTK_FILE_CHOOSER (dialog)) != expected_action)
    return FALSE;

  impl = get_impl_from_dialog (GTK_WIDGET (dialog));

  g_assert (impl->action == expected_action);

  passed = TRUE;

  /* OPEN implies that the "new folder" button is hidden; otherwise it is shown */
  if (impl->action == GTK_FILE_CHOOSER_ACTION_OPEN)
    passed = passed && !gtk_widget_get_visible (impl->browse_new_folder_button);
  else
    passed = passed && gtk_widget_get_visible (impl->browse_new_folder_button);

  /* Check that the widgets are present/visible or not */
  if (has_action (open_actions, G_N_ELEMENTS (open_actions), impl->action))
    {
      passed = passed && (impl->save_widgets == NULL
			  && (impl->location_mode == LOCATION_MODE_PATH_BAR
			      ? impl->location_entry == NULL
			      : impl->location_entry != NULL)
			  && impl->save_folder_label == NULL
			  && impl->save_folder_combo == NULL
			  && impl->save_expander == NULL
			  && GTK_IS_CONTAINER (impl->browse_widgets) && gtk_widget_is_drawable (impl->browse_widgets));
    }
  else if (has_action (save_actions, G_N_ELEMENTS (save_actions), impl->action))
    {
      /* FIXME: we can't use GTK_IS_FILE_CHOOSER_ENTRY() because it uses
       * _gtk_file_chooser_entry_get_type(), which is a non-exported symbol.
       * So, we just test impl->location_entry for being non-NULL
       */
      passed = passed && (GTK_IS_CONTAINER (impl->save_widgets) && gtk_widget_is_drawable (impl->save_widgets)
			  && impl->location_entry != NULL && gtk_widget_is_drawable (impl->location_entry)
			  && GTK_IS_LABEL (impl->save_folder_label) && gtk_widget_is_drawable (impl->save_folder_label)
			  && GTK_IS_COMBO_BOX (impl->save_folder_combo) && gtk_widget_is_drawable (impl->save_folder_combo)
			  && GTK_IS_EXPANDER (impl->save_expander) && gtk_widget_is_drawable (impl->save_expander)
			  && GTK_IS_CONTAINER (impl->browse_widgets));

      /* FIXME: we are in a SAVE mode; test the visibility and sensitivity of
       * the children that change depending on the state of the expander.
       */
    }
  else
    {
      g_error ("BAD TEST: test_widgets_for_current_action() doesn't know about %s", get_action_name (impl->action));
      passed = FALSE;
    }

  return passed;
}

typedef gboolean (* ForeachActionCallback) (GtkFileChooserDialog *dialog,
					    GtkFileChooserAction  action,
					    gpointer              user_data);

static gboolean
foreach_action (GtkFileChooserDialog *dialog,
		ForeachActionCallback callback,
		gpointer              user_data)
{
  GEnumClass *enum_class;
  int i;

  enum_class = g_type_class_peek (GTK_TYPE_FILE_CHOOSER_ACTION);
  if (!enum_class)
    g_error ("BUG: get_action_name(): no GEnumClass for GTK_TYPE_FILE_CHOOSER_ACTION");

  for (i = 0; i < enum_class->n_values; i++)
    {
      GEnumValue *enum_value;
      GtkFileChooserAction action;
      gboolean passed;

      enum_value = enum_class->values + i;
      action = enum_value->value;

      passed = (* callback) (dialog, action, user_data);
      if (!passed)
	return FALSE;
    }

  return TRUE;
}

struct action_closure {
  GtkFileChooserAction from_action;
};

static gboolean
switch_from_to_action_cb (GtkFileChooserDialog *dialog,
			  GtkFileChooserAction  action,
			  gpointer              user_data)
{
  struct action_closure *closure;
  gboolean passed;

  closure = user_data;

  gtk_file_chooser_set_action (GTK_FILE_CHOOSER (dialog), closure->from_action);

  passed = test_widgets_for_current_action (dialog, closure->from_action);
  log_test (passed, "switch_from_to_action_cb(): reset to action %s", get_action_name (closure->from_action));
  if (!passed)
    return FALSE;

  gtk_file_chooser_set_action (GTK_FILE_CHOOSER (dialog), action);

  passed = test_widgets_for_current_action (dialog, action);
  log_test (passed, "switch_from_to_action_cb(): transition from %s to %s",
	    get_action_name (closure->from_action),
	    get_action_name (action));
  return passed;
}

static gboolean
switch_from_action_cb (GtkFileChooserDialog *dialog,
		       GtkFileChooserAction  action,
		       gpointer              user_data)
{
  struct action_closure closure;

  closure.from_action = action;

  return foreach_action (dialog, switch_from_to_action_cb, &closure);
}

static void
test_action_widgets (void)
{
  GtkWidget *dialog;
  GtkFileChooserAction action;
  gboolean passed;

  dialog = gtk_file_chooser_dialog_new ("Test file chooser",
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					GTK_STOCK_OK,
					GTK_RESPONSE_ACCEPT,
					NULL);
  gtk_widget_show_now (dialog);

  action = gtk_file_chooser_get_action (GTK_FILE_CHOOSER (dialog));

  passed = test_widgets_for_current_action (GTK_FILE_CHOOSER_DIALOG (dialog), action);
  log_test (passed, "test_action_widgets(): widgets for initial action %s", get_action_name (action));
  g_assert (passed);

  passed = foreach_action (GTK_FILE_CHOOSER_DIALOG (dialog), switch_from_action_cb, NULL);
  log_test (passed, "test_action_widgets(): all transitions through property change");
  g_assert (passed);

  gtk_widget_destroy (dialog);
}
#endif

#ifdef BROKEN_TESTS
static gboolean
test_reload_sequence (gboolean set_folder_before_map)
{
  GtkWidget *dialog;
  GtkFileChooserDefault *impl;
  gboolean passed;
  char *folder;
  char *current_working_dir;

  passed = TRUE;

  current_working_dir = g_get_current_dir ();

  dialog = gtk_file_chooser_dialog_new ("Test file chooser",
					NULL,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					GTK_STOCK_OK,
					GTK_RESPONSE_ACCEPT,
					NULL);
  impl = get_impl_from_dialog (dialog);

  if (set_folder_before_map)
    {
      gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), g_get_home_dir ());

      wait_for_idle ();

      passed = passed && (impl->current_folder != NULL
			  && impl->browse_files_model != NULL
			  && (impl->load_state == LOAD_PRELOAD || impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
			  && impl->reload_state == RELOAD_HAS_FOLDER
			  && (impl->load_state == LOAD_PRELOAD ? (impl->load_timeout_id != 0) : TRUE)
			  && ((impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
			      ? (impl->load_timeout_id == 0 && impl->sort_model != NULL)
			      : TRUE));

      wait_for_idle ();

      folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (dialog));
      passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);
      g_free (folder);
    }
  else
    {
      /* Initially, no folder is not loaded or pending */
      passed = passed && (impl->current_folder == NULL
			  && impl->sort_model == NULL
			  && impl->browse_files_model == NULL
			  && impl->load_state == LOAD_EMPTY
			  && impl->reload_state == RELOAD_EMPTY
			  && impl->load_timeout_id == 0);

      wait_for_idle ();

      folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (dialog));
      passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);
    }

  log_test (passed, "test_reload_sequence(): initial status");

  /* After mapping, it is loading some folder, either the one that was explicitly set or the default one */

  gtk_widget_show_now (dialog);

  wait_for_idle ();

  passed = passed && (impl->current_folder != NULL
		      && impl->browse_files_model != NULL
		      && (impl->load_state == LOAD_PRELOAD || impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
		      && impl->reload_state == RELOAD_HAS_FOLDER
		      && (impl->load_state == LOAD_PRELOAD ? (impl->load_timeout_id != 0) : TRUE)
		      && ((impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
			  ? (impl->load_timeout_id == 0 && impl->sort_model != NULL)
			  : TRUE));

  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (dialog));
  if (set_folder_before_map)
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);
  else
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);

  g_free (folder);

  log_test (passed, "test_reload_sequence(): status after map");

  /* Unmap it; we should still have a folder */

  gtk_widget_hide (dialog);

  wait_for_idle ();

  passed = passed && (impl->current_folder != NULL
		      && impl->browse_files_model != NULL
		      && (impl->load_state == LOAD_PRELOAD || impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
		      && impl->reload_state == RELOAD_WAS_UNMAPPED
		      && (impl->load_state == LOAD_PRELOAD ? (impl->load_timeout_id != 0) : TRUE)
		      && ((impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
			  ? (impl->load_timeout_id == 0 && impl->sort_model != NULL)
			  : TRUE));

  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (dialog));
  if (set_folder_before_map)
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);
  else
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);

  g_free (folder);

  log_test (passed, "test_reload_sequence(): status after unmap");

  /* Map it again! */

  gtk_widget_show_now (dialog);

  wait_for_idle ();

  passed = passed && (impl->current_folder != NULL
		      && impl->browse_files_model != NULL
		      && (impl->load_state == LOAD_PRELOAD || impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
		      && impl->reload_state == RELOAD_HAS_FOLDER
		      && (impl->load_state == LOAD_PRELOAD ? (impl->load_timeout_id != 0) : TRUE)
		      && ((impl->load_state == LOAD_LOADING || impl->load_state == LOAD_FINISHED)
			  ? (impl->load_timeout_id == 0 && impl->sort_model != NULL)
			  : TRUE));

  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (dialog));
  if (set_folder_before_map)
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);
  else
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);

  g_free (folder);

  log_test (passed, "test_reload_sequence(): status after re-map");

  gtk_widget_destroy (dialog);
  g_free (current_working_dir);

  return passed;
}

static void
test_reload (void)
{
  gboolean passed;

  passed = test_reload_sequence (FALSE);
  log_test (passed, "test_reload(): create and use the default folder");
  g_assert (passed);

  passed = test_reload_sequence (TRUE);
  log_test (passed, "test_reload(): set a folder explicitly before mapping");
  g_assert (passed);
}
#endif

static gboolean
test_button_folder_states_for_action (GtkFileChooserAction action, gboolean use_dialog, gboolean set_folder_on_dialog)
{
  gboolean passed;
  GtkWidget *window;
  GtkWidget *button;
  char *folder;
  GtkWidget *dialog;
  char *current_working_dir;
  gboolean must_have_cwd;

  passed = TRUE;

  current_working_dir = g_get_current_dir ();
  must_have_cwd = !(use_dialog && set_folder_on_dialog);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  if (use_dialog)
    {
      dialog = gtk_file_chooser_dialog_new ("Test", NULL, action,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					    NULL);
      button = gtk_file_chooser_button_new_with_dialog (dialog);

      if (set_folder_on_dialog)
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), g_get_home_dir ());
    }
  else
    {
      button = gtk_file_chooser_button_new ("Test", action);
      dialog = NULL; /* keep gcc happy */
    }

  gtk_container_add (GTK_CONTAINER (window), button);

  /* Pre-map; no folder is set */
  wait_for_idle ();

  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (button));
  if (must_have_cwd)
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);
  else
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);

  log_test (passed, "test_button_folder_states_for_action(): %s, use_dialog=%d, set_folder_on_dialog=%d, pre-map, %s",
	    get_action_name (action),
	    use_dialog,
	    set_folder_on_dialog,
	    must_have_cwd ? "must have $cwd" : "must have explicit folder");

  /* Map; folder should be set */

  gtk_widget_show_all (window);
  gtk_widget_show_now (window);

  wait_for_idle ();

  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (button));

  if (must_have_cwd)
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);
  else
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);

  log_test (passed, "test_button_folder_states_for_action(): %s, use_dialog=%d, set_folder_on_dialog=%d, mapped, %s",
	    get_action_name (action),
	    use_dialog,
	    set_folder_on_dialog,
	    must_have_cwd ? "must have $cwd" : "must have explicit folder");
  g_free (folder);

  /* Unmap; folder should be set */

  gtk_widget_hide (window);
  wait_for_idle ();
  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (button));

  if (must_have_cwd)
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);
  else
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);

  log_test (passed, "test_button_folder_states_for_action(): %s, use_dialog=%d, set_folder_on_dialog=%d, unmapped, %s",
	    get_action_name (action),
	    use_dialog,
	    set_folder_on_dialog,
	    must_have_cwd ? "must have $cwd" : "must have explicit folder");
  g_free (folder);

  /* Re-map; folder should be set */

  gtk_widget_show_now (window);
  folder = gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER (button));

  if (must_have_cwd)
    passed = passed && (g_strcmp0 (folder, current_working_dir) == 0);
  else
    passed = passed && (g_strcmp0 (folder, g_get_home_dir()) == 0);
  wait_for_idle ();
  log_test (passed, "test_button_folder_states_for_action(): %s, use_dialog=%d, set_folder_on_dialog=%d, re-mapped, %s",
	    get_action_name (action),
	    use_dialog,
	    set_folder_on_dialog,
	    must_have_cwd ? "must have $cwd" : "must have explicit folder");
  g_free (folder);

  g_free (current_working_dir);

  gtk_widget_destroy (window);

  return passed;
}

static void
test_button_folder_states (void)
{
  /* GtkFileChooserButton only supports OPEN and SELECT_FOLDER */
  static const GtkFileChooserAction actions_to_test[] = {
    GTK_FILE_CHOOSER_ACTION_OPEN,
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER
  };
  gboolean passed;
  int i;

  passed = TRUE;

  for (i = 0; i < G_N_ELEMENTS (actions_to_test); i++)
    {
      passed = passed && test_button_folder_states_for_action (actions_to_test[i], FALSE, FALSE);
      g_assert (passed);
      passed = passed && test_button_folder_states_for_action (actions_to_test[i], TRUE, FALSE);
      g_assert (passed);
      passed = passed && test_button_folder_states_for_action (actions_to_test[i], TRUE, TRUE);
      g_assert (passed);
      log_test (passed, "test_button_folder_states(): action %s", get_action_name (actions_to_test[i]));
    }

  log_test (passed, "test_button_folder_states(): all supported actions");
}

static void
test_folder_switch_and_filters (void)
{
  gboolean passed;
  char *cwd;
  char *base_dir;
  GFile *cwd_file;
  GFile *base_dir_file;
  GtkWidget *dialog;
  GtkFileFilter *all_filter;
  GtkFileFilter *txt_filter;
  GtkFileChooserDefault *impl;

  passed = TRUE;

  cwd = g_get_current_dir ();
  base_dir = g_build_filename (cwd, "file-chooser-test-dir", NULL);

  dialog = gtk_file_chooser_dialog_new ("Test", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					NULL);
  impl = get_impl_from_dialog (dialog);

  cwd_file = g_file_new_for_path (cwd);
  base_dir_file = g_file_new_for_path (base_dir);

  passed = passed && gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), base_dir);
  g_assert (passed);

  /* All files filter */

  all_filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (all_filter, "All files");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), all_filter);

  /* *.txt filter */

  txt_filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (all_filter, "*.txt");
  gtk_file_filter_add_pattern (txt_filter, "*.txt");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), txt_filter);

  /* Test filter set */

  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (dialog), all_filter);
  passed = passed && (gtk_file_chooser_get_filter (GTK_FILE_CHOOSER (dialog)) == all_filter);
  g_assert (passed);

  gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (dialog), txt_filter);
  passed = passed && (gtk_file_chooser_get_filter (GTK_FILE_CHOOSER (dialog)) == txt_filter);
  log_test (passed, "test_folder_switch_and_filters(): set and get filter");
  g_assert (passed);

  gtk_widget_show (dialog);

  /* Test that filter is unchanged when we switch folders */

  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), cwd);
  sleep_in_main_loop ();
  passed = passed && (gtk_file_chooser_get_filter (GTK_FILE_CHOOSER (dialog)) == txt_filter);
  g_assert (passed);

  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), base_dir);
  sleep_in_main_loop ();

  g_signal_emit_by_name (impl->browse_path_bar, "path-clicked",
			 cwd_file,
			 base_dir_file,
			 FALSE);
  sleep_in_main_loop ();
  passed = passed && (gtk_file_chooser_get_filter (GTK_FILE_CHOOSER (dialog)) == txt_filter);
  log_test (passed, "test_folder_switch_and_filters(): filter after changing folder");
  g_assert (passed);

  /* cleanups */
  g_free (cwd);
  g_free (base_dir);
  g_object_unref (cwd_file);
  g_object_unref (base_dir_file);

  gtk_widget_destroy (dialog);

  log_test (passed, "test_folder_switch_and_filters(): all filter tests");
}

extern void pixbuf_init (void);

int
main (int    argc,
      char **argv)
{
  pixbuf_init ();
  /* initialize test program */
  gtk_test_init (&argc, &argv);

  /* Register tests */

  setup_file_chooser_button_tests ();
#ifdef BROKEN_TESTS
  setup_set_filename_tests ();
  setup_set_current_name_tests ();

  g_test_add_func ("/GtkFileChooser/confirm_overwrite", test_confirm_overwrite);
  g_test_add_func ("/GtkFileChooser/action_widgets", test_action_widgets);
  g_test_add_func ("/GtkFileChooser/reload", test_reload);
  g_test_add_func ("/GtkFileChooser/button_folder_states", test_button_folder_states);
  g_test_add_func ("/GtkFileChooser/folder_switch_and_filters", test_folder_switch_and_filters);
#endif

  /* run and check selected tests */
  return g_test_run();
}
