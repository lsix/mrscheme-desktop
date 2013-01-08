## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(gtk2_CFLAGS) $(webkit_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = mrscheme_desktop

mrscheme_desktop_SOURCES = \
	main.c \
	gtk-mr-scheme.c \
	gtk-mr-scheme.h \
	gtk-mr-scheme-window.c \
	gtk-mr-scheme-window.h

mrscheme_desktop_LDFLAGS = \
	-Wl,--export-dynamic

mrscheme_desktop_LDADD = $(gtk2_LIBS) $(webkit_LIBS) $(LIBINTL)


