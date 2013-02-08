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

#ifndef _GTK_MR_SCHEME_WINDOW_H_
#define _GTK_MR_SCHEME_WINDOW_H_

#include "gtk-mr-scheme.h"
#include <gtk/gtkwindow.h>

G_BEGIN_DECLS

#define GTK_TYPE_MR_SCHEME_WINDOW             (gtk_mr_scheme_window_get_type ())
#define GTK_MR_SCHEME_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MR_SCHEME_WINDOW, GtkMrSchemeWindow))
#define GTK_MR_SCHEME_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MR_SCHEME_WINDOW, GtkMrSchemeWindowClass))
#define GTK_IS_MR_SCHEME_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MR_SCHEME_WINDOW))
#define GTK_IS_MR_SCHEME_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MR_SCHEME_WINDOW))
#define GTK_MR_SCHEME_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MR_SCHEME_WINDOW, GtkMrSchemeWindowClass))

#define GTK_MR_SCHEME_WINDOW_TITLE            "(Desktop (MrScheme))"

typedef struct _GtkMrSchemeWindowClass GtkMrSchemeWindowClass;
typedef struct _GtkMrSchemeWindow GtkMrSchemeWindow;



struct _GtkMrSchemeWindowClass
{
	GtkWindowClass parent_class;
	gint numberOfInstances;
};

struct _GtkMrSchemeWindow
{
	GtkWindow parent_instance;
	GtkWidget*   viewContainer;
	GtkMrScheme* mrSchemeView;

	gchar *fileName;
};

GType      gtk_mr_scheme_window_get_type (void) G_GNUC_CONST;

/*
 * Create a new empty mrscheme window
 * */
GtkWidget* gtk_mr_scheme_window_new (void);

/*
 * Create a new mrscheme window and load it with
 * the cont of a file.
 * */
GtkWidget* gtk_mr_scheme_window_new_from_file (const char* fileName);

G_END_DECLS

#endif /* _GTK_MR_SCHEME_WINDOW_H_ */
