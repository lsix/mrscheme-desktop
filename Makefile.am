## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = po src 

mrscheme_desktopdocdir = ${prefix}/doc/mrscheme_desktop
mrscheme_desktopdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = config.rpath m4/ChangeLog  $(mrscheme_desktopdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(mrscheme_desktopdocdir)

ACLOCAL_AMFLAGS = -I m4

