/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mrscheme-desktop
 * Copyright (C) Lancelot SIX 2012 <lancelot.six@lip6.fr>
 * 
 * mrscheme-desktop is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mrscheme-desktop is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#ifndef _GTK_MR_SCHEME_H_
#define _GTK_MR_SCHEME_H_

#include <webkit/webkit.h>

G_BEGIN_DECLS

#define GTK_TYPE_MR_SCHEME             (gtk_mr_scheme_get_type ())
#define GTK_MR_SCHEME(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MR_SCHEME, GtkMrScheme))
#define GTK_MR_SCHEME_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MR_SCHEME, GtkMrSchemeClass))
#define GTK_IS_MR_SCHEME(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MR_SCHEME))
#define GTK_IS_MR_SCHEME_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MR_SCHEME))
#define GTK_MR_SCHEME_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MR_SCHEME, GtkMrSchemeClass))

#define MRECHEME_WEB_URI "https://www-licence.ufr-info-p6.jussieu.fr/lmd/licence/2012/ue/LI101-2012oct/MrScheme/mrscheme.html"

typedef struct _GtkMrSchemeClass GtkMrSchemeClass;
typedef struct _GtkMrScheme GtkMrScheme;

struct _GtkMrSchemeClass
{
	WebKitWebViewClass parent_class;
};

struct _GtkMrScheme
{
	WebKitWebView parent_instance;

 
};

GType      gtk_mr_scheme_get_type (void) G_GNUC_CONST;

/*
 * Create a new instance of a widget showing mrScheme
 * */
GtkWidget* gtk_mr_scheme_new (void);

/*
 * Returns the current state of the code as shown in the
 * mrScheme view
 * */
gchar*     gtk_mr_scheme_get_scm_program (GtkMrScheme *mrScheme);

/*
 * Replace the current code loaded in the mrScheme view
 * by scm_prog
 * */
void       gtk_mr_scheme_set_scm_program (GtkMrScheme *mrScheme, gchar* scm_prog);

/*
 * Run the program contained in the mrScheme view
 * */
void       gtk_mr_scheme_execute_program (GtkMrScheme *mrScheme);

G_END_DECLS

#endif /* _GTK_MR_SCHEME_H_ */
