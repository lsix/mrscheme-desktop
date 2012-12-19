/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) 2012 Lancelot SIX <lancelot.six@lip6.fr>
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

#include "config.h"

#include <config.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#include "gtk-mr-scheme.h" 

struct _MrSchemeDesktop
{
	GtkWidget   *window;
	GtkMrScheme *mrScheme;
	GtkAction   *save;
	GtkAction   *open;
	GtkAction   *quit;
	GtkAction   *run;
};

static struct _MrSchemeDesktop instance;

/*******************************************************************************
 *                                                                             *
 *                                                                             *
 *                         Fonctions utilitaires                               *
 *                                                                             *
 *                                                                             *
 ******************************************************************************/

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


void load_scm_file(GObject *object, gpointer data)
{
	GtkWidget *dialog = gtk_file_chooser_dialog_new(_("Open"),
	                                                GTK_WINDOW (instance.window),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
	                                                NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar* filepath = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char *fileContent = read_file (filepath);

		gtk_mr_scheme_set_scm_program (instance.mrScheme, fileContent);
		
		g_free(filepath);
		free(fileContent);
	}

	gtk_widget_destroy (dialog);
}

void save_scm_file(GObject *object, gpointer data)
{
	gchar *val = gtk_mr_scheme_get_scm_program (instance.mrScheme);
	
	GtkWidget *dialog = gtk_file_chooser_dialog_new(_("Save as"),
	                                                GTK_WINDOW (instance.window),
							GTK_FILE_CHOOSER_ACTION_SAVE,
							GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
							GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
							NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT )
	{
		gchar *filepath = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		FILE  *f       = fopen(filepath, "w");
		fprintf (f, "%s", val);
		fclose (f);
		g_free (filepath);
	}
	
	free (val);
	gtk_widget_destroy (dialog);
}

void run_scm_code(GObject *object, gpointer data)
{
	gtk_mr_scheme_execute_program (instance.mrScheme);
}

/*******************************************************************************
 *                                                                             *
 *                                                                             *
 *                   Gestion du GUI et construction du process                 *
 *                                                                             *
 *                                                                             *
 ******************************************************************************/

static GtkWidget*
create_menu_bar(void)
{
	GtkWidget *mBar = gtk_menu_bar_new ();
	GtkWidget *file = gtk_menu_item_new_with_mnemonic(_("_File"));
	GtkWidget *fMen = gtk_menu_new ();
	GtkWidget *exec = gtk_menu_item_new_with_mnemonic (_("E_xecute"));
	GtkWidget *eMen = gtk_menu_new ();
	GtkWidget *fSep = gtk_separator_menu_item_new ();

	// Initialise la hierarchie
	gtk_menu_shell_append (GTK_MENU_SHELL (mBar), file);
	gtk_menu_shell_append (GTK_MENU_SHELL (mBar), exec);
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file), fMen);
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (instance.open));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (instance.save));
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), fSep);
	gtk_menu_shell_append (GTK_MENU_SHELL (fMen), gtk_action_create_menu_item (instance.quit));
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (exec), eMen);
	gtk_menu_shell_append (GTK_MENU_SHELL (eMen), gtk_action_create_menu_item (instance.run));

	return mBar;
}

static GtkWidget*
create_tool_bar (void)
{
	GtkWidget   *toolBar = gtk_toolbar_new ();
	GtkToolItem *sep     = gtk_separator_tool_item_new ();

	gtk_toolbar_set_style(GTK_TOOLBAR(toolBar), GTK_TOOLBAR_ICONS);

	gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (instance.open)), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (instance.save)), -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolBar), sep,     -1);
	gtk_toolbar_insert (GTK_TOOLBAR (toolBar), GTK_TOOL_ITEM (gtk_action_create_tool_item (instance.run)),  -1);

	return toolBar;
}

static GtkWidget*
create_window (void)
{
	GtkWidget *vBox;
	GtkWidget *scrWin;

	GtkAccelGroup *accelGrp;

	// Initialise l'objet metier
	instance.mrScheme = GTK_MR_SCHEME (gtk_mr_scheme_new ());
	
	// Construit l'interface graphique
	instance.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (instance.window), "(Desktop (MrScheme))");
	gtk_window_set_default_size (GTK_WINDOW (instance.window), 800, 600);

	// gestion des racourcis clavier
	accelGrp = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (instance.window), accelGrp);

	instance.open = gtk_action_new ("Open", _("_Open"), _("Open a scm file"),   GTK_STOCK_OPEN);
	instance.save = gtk_action_new ("Save", _("_Save"), _("Save the scm file"), GTK_STOCK_SAVE);
	instance.run  = gtk_action_new ("Run",  _("_Run"),  _("Run the code"),      GTK_STOCK_EXECUTE);
	instance.quit = gtk_action_new ("Quit", _("_Quit"), _("Quit the application"), GTK_STOCK_QUIT);

	/* Exit when the window is closed */
	g_signal_connect (instance.window, "destroy",  G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (instance.quit,   "activate", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (instance.open,   "activate", G_CALLBACK (load_scm_file), NULL);
	g_signal_connect (instance.save,   "activate", G_CALLBACK (save_scm_file), NULL);
	g_signal_connect (instance.run,    "activate", G_CALLBACK (run_scm_code),  NULL);

	/* Associate key shortcuts */
	gtk_action_set_accel_path (instance.open, "<MrSchemeCode>/open");
	gtk_action_set_accel_path (instance.save, "<MrSchemeCode>/save");
	gtk_action_set_accel_path (instance.run,  "<MrSchemeCode>/run");
	gtk_action_set_accel_path (instance.quit, "<MrScheme>/quit");
	gtk_accel_map_add_entry ("<MrSchemeCode>/open", GDK_o, GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrSchemeCode>/save", GDK_s, GDK_CONTROL_MASK);
	gtk_accel_map_add_entry ("<MrSchemeCode>/run", GDK_r, GDK_MOD1_MASK);
	gtk_accel_map_add_entry ("<MrScheme>/quit", GDK_q, GDK_CONTROL_MASK);
	gtk_action_set_accel_group (instance.open, accelGrp);
	gtk_action_set_accel_group (instance.save, accelGrp);
	gtk_action_set_accel_group (instance.run, accelGrp);
	gtk_action_set_accel_group (instance.quit, accelGrp);

	vBox = gtk_vbox_new (false, 0);
	gtk_container_add(GTK_CONTAINER (instance.window), vBox);

	gtk_box_pack_start (GTK_BOX (vBox), create_menu_bar (), false, true, 0);
	gtk_box_pack_start (GTK_BOX (vBox), create_tool_bar (), false, true, 0);
	
	scrWin = gtk_scrolled_window_new (NULL,NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrWin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_box_pack_start (GTK_BOX (vBox), scrWin, true, true, 0);

	gtk_container_add (GTK_CONTAINER (scrWin), GTK_WIDGET (instance.mrScheme));

	gtk_widget_show_all (instance.window);
	
	return instance.window;
}


int
main (int argc, char *argv[])
{
 	GtkWidget *window;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_init (&argc, &argv);

	window = create_window ();
	gtk_widget_show (window);

	gtk_main ();

	return 0;
}
