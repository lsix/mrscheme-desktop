/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mrscheme-desktop
 * Copyright (C) lancelot SIX 2013 <lancelot@lancelotsix.com>
 * 
 * mrscheme-desktop is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mrscheme-desktop is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtkaboutdialog.h>
#include <stdlib.h>
#include <string.h>

#include "authors.h"
#include "config.h"
#include "gtk-mr-scheme-window.h"

G_DEFINE_TYPE (GtkMrSchemeWindow, gtk_mr_scheme_window, GTK_TYPE_WINDOW);

/******************************************************************************
 *                                                                            *
 *                              Utility functions                             *
 *                                                                            *
 ******************************************************************************/

/*
 * Creates a new string containing a followed by b.
 * The string have to be freed using g_free
 * */
gchar*
concat_strings(const gchar*a, const gchar* b)
{
	gchar* ret = g_malloc ((strlen (a) + strlen (b) + 1)*sizeof (gchar));
	strcpy (ret, a);
	strcat (ret, b);
	return ret;
}

/*
 * Read a file into a string and return it.
 * The string have to be freed using free.
 * */
char*
read_file(char *filePath)
{
	char *ret;
	long input_file_size;
	FILE *input_file = fopen(filePath, "r");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);
	ret = malloc(input_file_size * (sizeof(char)));
	int nb_read = fread(ret, sizeof(char), input_file_size, input_file);
	if (nb_read < sizeof(char)*input_file_size)
		fprintf( stderr, "Error reading input file");
	fclose(input_file);
	return ret;
}

/*
 * Update the titlebar of the GtkMrSchemeWindow according to the
 * filename associated with it.
 * */
void
gtk_mr_scheme_window_update_title (GtkMrSchemeWindow *window, bool modified)
{
	gchar *newTitle;

	newTitle = g_malloc ((
	                      strlen (PACKAGE_NAME)
	                      + (window->fileName==NULL ? 0 : 3 + strlen (window->fileName))
	                      + (modified ? 1 : 0)) * sizeof (gchar));
	strcpy (newTitle, PACKAGE_NAME);
	if (window->fileName != NULL)
	{
		strcat (newTitle, " - ");
		strcat (newTitle, window->fileName);
	}
	if (modified)
	{
		strcat (newTitle, "*");
	}

	gtk_window_set_title (GTK_WINDOW (window), newTitle);
	g_free (newTitle);
}

/*
 * Update the fielname of associated with the window
 * */
void
gtk_mr_scheme_window_set_filename(GtkMrSchemeWindow* window, const gchar* filename)
{
	if (filename == NULL) return;
	
	if (window->fileName != NULL && (strcmp (filename, window->fileName) != 0))
	{
		g_free (window->fileName);
		window->fileName = NULL;
	}

	if (window->fileName == NULL)
	{
		window->fileName = g_malloc ((strlen (filename)+1)*sizeof (gchar));
		strcpy (window->fileName, filename);
	}
	gtk_mr_scheme_window_update_title (GTK_MR_SCHEME_WINDOW (window), false);
}

/*
 * Writes the content of the window into the file designated by fileName and
 * update the window title to reflect it.
 * */
void
save_file_as(GtkMrSchemeWindow* window, const gchar* fileName)
{
	FILE *f;
	gchar *content;

	content = gtk_mr_scheme_get_scm_program (window->mrSchemeView);
	f = fopen (fileName, "w");
	fprintf (f, "%s", content);
	fclose (f);
	g_free (content);

	gtk_mr_scheme_window_set_filename (window, fileName);
}

/******************************************************************************
 *                                                                            *
 *                  Callbacks function associated with actions                *
 *                                                                            *
 ******************************************************************************/

void
local_view_ready (GObject* object, gpointer data)
{
	GtkMrSchemeWindow* gtk_mr_scheme_window = GTK_MR_SCHEME_WINDOW (data);
	GList *children, *iter;
	children = gtk_container_get_children (GTK_CONTAINER (gtk_mr_scheme_window->viewContainer));
	for (iter = children; iter != NULL; iter = g_list_next(iter) )
	{
		gtk_container_remove (GTK_CONTAINER (gtk_mr_scheme_window->viewContainer),
		                      GTK_WIDGET (iter->data));
		
	}
	g_list_free (children);
	
	gtk_container_add (GTK_CONTAINER (gtk_mr_scheme_window->viewContainer), GTK_WIDGET (gtk_mr_scheme_window->mrSchemeView));

	// Preload file if needed
	if (gtk_mr_scheme_window->fileName != NULL)
	{
		char *fileContent = read_file (gtk_mr_scheme_window->fileName);

		gtk_mr_scheme_set_scm_program (GTK_MR_SCHEME ( gtk_mr_scheme_window->mrSchemeView ),
		                               fileContent);

		free (fileContent);

		gtk_mr_scheme_window_set_filename (gtk_mr_scheme_window, gtk_mr_scheme_window->fileName);

	}
	
	gtk_widget_set_visible (GTK_WIDGET (gtk_mr_scheme_window->mrSchemeView), true);

}

/*
 * Spawns a new editor window
 * */
void
new_mr_scheme_window(GObject *object, gpointer data)
{
	GtkWidget* newwin = gtk_mr_scheme_window_new ();
	gtk_widget_show_all (newwin);
}

/*
 * Close the active mrscheme window
 * */
void
close_mr_scheme_window(GObject* object, gpointer data)
{
	GtkWidget* window = GTK_WIDGET (data);
	gtk_widget_destroy (window);
}

/*
 * Loads a scheme file into the editor of the current window
 * */
void
load_scm_file(GObject *object, gpointer data)
{
	GtkWidget*         window = GTK_WIDGET (data);
	GtkMrSchemeWindow* mrwin  = GTK_MR_SCHEME_WINDOW (data);
	GtkWidget*         dialog = gtk_file_chooser_dialog_new(_("Open"),
	                                                GTK_WINDOW (window),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	                                                NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar* filepath = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char *fileContent = read_file (filepath);

		gtk_mr_scheme_set_scm_program (GTK_MR_SCHEME ( mrwin->mrSchemeView ),
		                               fileContent);

		gtk_mr_scheme_window_set_filename (mrwin, filepath);
		
		g_free(filepath);
		free(fileContent);

		gtk_mr_scheme_window_update_title (mrwin, false);
	}

	gtk_widget_destroy (dialog);
}

/*
 * Save the content of the editor of the current window into
 * a new file. The filename is prompted with a gtk dialog.
 * */
void save_as_scm_file(GObject *object, gpointer data)
{
	GtkWidget*         window = GTK_WIDGET (data);
	GtkMrSchemeWindow* mrwin  = GTK_MR_SCHEME_WINDOW (data);
	
	GtkWidget *dialog = gtk_file_chooser_dialog_new(_("Save as"),
	                                                GTK_WINDOW (window),
							GTK_FILE_CHOOSER_ACTION_SAVE,
							GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
							GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
							NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT )
	{
		gchar *filepath = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		save_file_as (mrwin, filepath);
		g_free (filepath);
	}
	
	gtk_widget_destroy (dialog);
}

/*
 * Save the content of the editor of the current window into
 * the file that was previously used either when loading or
 * saving a file.
 * */
void save_scm_file(GObject *object, gpointer data)
{
	GtkMrSchemeWindow* mrwin = GTK_MR_SCHEME_WINDOW (data);
	// If no filename is known, run the save as action
	if (GTK_MR_SCHEME_WINDOW (data)->fileName == NULL)
	{
		save_as_scm_file (object, data);
	}
	// otherwise directly save the file according to the
	// previously used file name
	else
	{
		save_file_as (mrwin, mrwin->fileName);
	}
}

/*
 * Run the code contained into the current window
 * */
void run_scm_code(GObject *object, gpointer data)
{
	GtkMrSchemeWindow* mrwin = GTK_MR_SCHEME_WINDOW (data);
	gtk_mr_scheme_execute_program (mrwin->mrSchemeView);
}

void show_about_info (GObject *object, gpointer data)
{
	GtkWidget *dialog = gtk_about_dialog_new ();
	gtk_about_dialog_set_license      (GTK_ABOUT_DIALOG (dialog),
	                                   "This software is distributed under the terms of the GPL V3.0.\n You may find a copy of this lisence at http://www.gnu.org/licenses/gpl.html");
	gtk_about_dialog_set_version      (GTK_ABOUT_DIALOG (dialog),
	                                   PACKAGE_VERSION);
	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (dialog),
	                                   PACKAGE_NAME);
	gtk_about_dialog_set_website      (GTK_ABOUT_DIALOG (dialog),
	                                   PACKAGE_URL);
	gtk_about_dialog_set_authors      (GTK_ABOUT_DIALOG (dialog),
	                                   authors);
	gtk_about_dialog_set_comments     (GTK_ABOUT_DIALOG (dialog),
	                                   _("Desktop version of MrScheme.\n Original project availalble at https://www-licence.ufr-info-p6.jussieu.fr/lmd/licence/2012/ue/LI101-2012oct/MrScheme/mrscheme.html"));
	
	gtk_dialog_run ( GTK_DIALOG(dialog)); // Just ignore return value since just one is acceptable
	gtk_widget_destroy (dialog);
}

/*
 * Updates the window title when the content of the code editor changed
 * (adds a star at its end to nofify the user that the changes on the buffer
 * are unsaved)
 * */
void on_code_changed_callback (GObject *object, gpointer data)
{
	gtk_mr_scheme_window_update_title (GTK_MR_SCHEME_WINDOW (data), true);
}

/******************************************************************************
 *                                                                            *
 *                             Public interface                               *
 *                                                                            *
 ******************************************************************************/

static void
gtk_mr_scheme_window_init (GtkMrSchemeWindow *gtk_mr_scheme_window)
{
	// Main attributes for the widget
	GtkWidget*     vBox;
	GtkAccelGroup* accelGrp;
	GtkWidget*     loadingMsg;

	// Actions associated with the widget
	GtkAction*     open;
	GtkAction*     save;
	GtkAction*     run;
	GtkAction*     quit;
	GtkAction*     saveAs;
	GtkAction*     newWin;
	GtkAction*     close;

	// widgets for the menubar
	GtkWidget *mBar = gtk_menu_bar_new ();
	GtkWidget *file = gtk_menu_item_new_with_mnemonic(_("_File"));
	GtkWidget *fMen = gtk_menu_new ();
	GtkWidget *exec = gtk_menu_item_new_with_mnemonic (_("E_xecute"));
	GtkWidget *eMen = gtk_menu_new ();
	GtkWidget *help = gtk_menu_item_new_with_mnemonic (_("_Help"));
	GtkWidget *hMen = gtk_menu_new ();

	// Menu items without actions.
	GtkWidget *about;

	gtk_mr_scheme_window->fileName = NULL;
	
	// widgets for the toolbar_item_type
	//GtkWidget   *toolBar = gtk_toolbar_new ();

	// Initialize the main component
	gtk_mr_scheme_window->mrSchemeView = GTK_MR_SCHEME (gtk_mr_scheme_new ());
	
	// Initialize the top window
	gtk_window_set_title (GTK_WINDOW (gtk_mr_scheme_window), PACKAGE_NAME);
	gtk_window_set_default_size (GTK_WINDOW (gtk_mr_scheme_window), 800, 600);
	GTK_WINDOW (gtk_mr_scheme_window)->type = GTK_WINDOW_TOPLEVEL;


	accelGrp = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (gtk_mr_scheme_window), accelGrp);

	open   = gtk_action_new ("Open",   _("_Open"),       _("Open a scm file"),             GTK_STOCK_OPEN);
	save   = gtk_action_new ("Save",   _("_Save"),       _("Save the scm file"),           GTK_STOCK_SAVE);
	run    = gtk_action_new ("Run",    _("_Run"),        _("Run the code"),                GTK_STOCK_EXECUTE);
	quit   = gtk_action_new ("Quit",   _("_Quit"),       _("Quit the application"),        GTK_STOCK_QUIT);
	saveAs = gtk_action_new ("Saveas", _("Save _as..."), _("Save the current program as"), GTK_STOCK_SAVE_AS);
	newWin = gtk_action_new ("New",    _("_New"),        _("Create a new file"),           GTK_STOCK_NEW);
	close  = gtk_action_new ("Close",  _("_Close"),      _("Close surrent window"),        GTK_STOCK_CLOSE);

	about = gtk_menu_item_new_with_mnemonic ( _("_About (Desktop (MrScheme))"));
	
	/* Exit when the window is closed */
	g_signal_connect (gtk_mr_scheme_window, "destroy",  G_CALLBACK (close_mr_scheme_window), gtk_mr_scheme_window);
	g_signal_connect (quit,      "activate", G_CALLBACK (gtk_main_quit),          gtk_mr_scheme_window);
	g_signal_connect (open,      "activate", G_CALLBACK (load_scm_file),          gtk_mr_scheme_window);
	g_signal_connect (save,      "activate", G_CALLBACK (save_scm_file),          gtk_mr_scheme_window);
	g_signal_connect (run,       "activate", G_CALLBACK (run_scm_code),           gtk_mr_scheme_window);
	g_signal_connect (saveAs,    "activate", G_CALLBACK (save_as_scm_file),       gtk_mr_scheme_window);
	g_signal_connect (close,     "activate", G_CALLBACK (close_mr_scheme_window), gtk_mr_scheme_window);
	g_signal_connect (newWin,    "activate", G_CALLBACK (new_mr_scheme_window),   NULL);
	g_signal_connect (about,     "activate", G_CALLBACK (show_about_info),        gtk_mr_scheme_window);

	/* Connect signals associated with the mrScheme view widget */
	g_signal_connect (gtk_mr_scheme_window->mrSchemeView, "mrschemeready",        G_CALLBACK (local_view_ready), gtk_mr_scheme_window);
	g_signal_connect (gtk_mr_scheme_window->mrSchemeView, "content-changed",      G_CALLBACK (on_code_changed_callback), gtk_mr_scheme_window);
	g_signal_connect (gtk_mr_scheme_window->mrSchemeView, "content-load-request", G_CALLBACK (load_scm_file), gtk_mr_scheme_window);
	g_signal_connect (gtk_mr_scheme_window->mrSchemeView, "content-save-request", G_CALLBACK (save_as_scm_file), gtk_mr_scheme_window);
	g_signal_connect (gtk_mr_scheme_window->mrSchemeView, "new-file-request",     G_CALLBACK (new_mr_scheme_window), NULL);
	
	/* Associate key shortcuts */
	gtk_action_set_accel_path (open,   "<MrSchemeCode>/open");
	gtk_action_set_accel_path (save,   "<MrSchemeCode>/save");
	gtk_action_set_accel_path (saveAs, "<MrSchemeCode>/saveAs");
	gtk_action_set_accel_path (run,    "<MrSchemeCode>/run");
	gtk_action_set_accel_path (close,  "<MrSchemeCode>/close");
	gtk_action_set_accel_path (quit,   "<MrScheme>/quit");
	gtk_action_set_accel_path (newWin, "<MrScheme>/new");
	gtk_accel_map_add_entry ("<MrSchemeCode>/open",   GDK_o, GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrSchemeCode>/save",   GDK_s, GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrSchemeCode>/saveAs", GDK_s, GDK_SHIFT_MASK | GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrSchemeCode>/run",    GDK_r, GDK_MOD1_MASK);
	gtk_accel_map_add_entry ("<MrSchemeCode>/close",  GDK_w, GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrScheme>/new",        GDK_n, GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrScheme>/quit",       GDK_q, GDK_CONTROL_MASK);
	gtk_action_set_accel_group (open,   accelGrp);
	gtk_action_set_accel_group (save,   accelGrp);
	gtk_action_set_accel_group (saveAs, accelGrp);
	gtk_action_set_accel_group (run,    accelGrp);
	gtk_action_set_accel_group (quit,   accelGrp);
	gtk_action_set_accel_group (newWin, accelGrp);
	gtk_action_set_accel_group (close,  accelGrp);

	// Builds the UI
	vBox = gtk_vbox_new (false, 0);
	gtk_container_add(GTK_CONTAINER (gtk_mr_scheme_window), vBox);

	gtk_menu_shell_append (GTK_MENU_SHELL (mBar), file);
	gtk_menu_shell_append (GTK_MENU_SHELL (mBar), exec);
	gtk_menu_shell_append (GTK_MENU_SHELL (mBar), help);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file), fMen);
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (newWin));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_separator_menu_item_new ());
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (open));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (saveAs));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (save));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_separator_menu_item_new ());
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (close));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (quit));
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (exec), eMen);
	gtk_menu_shell_append (GTK_MENU_SHELL (eMen), gtk_action_create_menu_item (run));
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help), hMen);
	gtk_menu_shell_append (GTK_MENU_SHELL (hMen), about);

	gtk_box_pack_start (GTK_BOX (vBox), mBar, false, true, 0);

	//gtk_toolbar_set_style(GTK_TOOLBAR(toolBar), GTK_TOOLBAR_ICONS);

	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (newWin)), -1);
	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), gtk_separator_tool_item_new (),                       -1);
	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (open)),   -1);
	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (saveAs)), -1);
	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (save)),   -1);
	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), gtk_separator_tool_item_new (),                       -1);
	//gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (run)),    -1);
	//gtk_box_pack_start (GTK_BOX (vBox), toolBar, false, true, 0);

	gtk_mr_scheme_window->viewContainer = gtk_scrolled_window_new (NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (gtk_mr_scheme_window->viewContainer),
	                                GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_box_pack_start (GTK_BOX (vBox), gtk_mr_scheme_window->viewContainer, true, true, 0);

	loadingMsg = gtk_label_new ( _("Loading....\nIf this message does not disappeare, please check your internet connection or\nvisit MrScheme website at\nhttps://www-licence.ufr-info-p6.jussieu.fr/lmd/licence/2012/ue/LI101-2012oct/MrScheme/mrscheme.html") );
	gtk_label_set_selectable (GTK_LABEL (loadingMsg), true);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (gtk_mr_scheme_window->viewContainer), loadingMsg);
	//gtk_container_add (GTK_CONTAINER (gtk_mr_scheme_window->viewContainer), loadingMsg);
	// Increments the number of availalble instances
	GTK_MR_SCHEME_WINDOW_GET_CLASS (gtk_mr_scheme_window)->numberOfInstances++;

}

static void
gtk_mr_scheme_window_finalize (GObject *object)
{
	/* TODO: check that the finalize method is properly called */
	GTK_MR_SCHEME_WINDOW (object)->mrSchemeView = NULL;
	if (GTK_MR_SCHEME_WINDOW (object)->fileName != NULL)
		g_free (GTK_MR_SCHEME_WINDOW (object)->fileName);
	
	G_OBJECT_CLASS (gtk_mr_scheme_window_parent_class)->finalize (object);

	if (--(GTK_MR_SCHEME_WINDOW_GET_CLASS (object)->numberOfInstances) == 0)
	{
		gtk_main_quit ();
	}
}

static void
gtk_mr_scheme_window_class_init (GtkMrSchemeWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GtkWindowClass* parent_class = GTK_WINDOW_CLASS (klass);

	object_class->finalize = gtk_mr_scheme_window_finalize;
	klass->numberOfInstances = 0;
}


// Implements the public interface
GtkWidget*
gtk_mr_scheme_window_new (void)
{
	return GTK_WIDGET ( g_object_new (GTK_TYPE_MR_SCHEME_WINDOW, NULL));
}

GtkWidget *
gtk_mr_scheme_window_new_from_file (const char* fileName)
{
	GtkMrSchemeWindow *window = GTK_MR_SCHEME_WINDOW (g_object_new (GTK_TYPE_MR_SCHEME_WINDOW, NULL));
	window->fileName = g_malloc ((strlen(fileName)+1)*sizeof (gchar));
	strcpy (window->fileName, fileName);
	return GTK_WIDGET (window);
}
