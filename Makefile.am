## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = po src

mrschemedir = $(datadir)/$(PACKAGE)
nobase_dist_mrscheme_DATA = \
	MrScheme/LICENSE \
	MrScheme/TODO \
	MrScheme/VERSION \
	MrScheme/mrscheme.html \
	MrScheme/mrscheme.css \
	MrScheme/mrscheme.js \
	MrScheme/parser101/lexer101.js \
	MrScheme/parser101/parser101.js \
	MrScheme/eval101/eval101.js \
	MrScheme/eval101/prims101.js \
	MrScheme/eval101/tree101.js \
	MrScheme/eval101/ecotree.css \
	MrScheme/eval101/ecotree.js \
	MrScheme/eval101/numericaltower101.js \
	MrScheme/eval101/bignum/README \
	MrScheme/eval101/bignum/biginteger.js \
	MrScheme/eval101/bignum/schemeNumber.js \
	MrScheme/editor101/theme101.css \
	MrScheme/editor101/scheme101-mode.js \
	MrScheme/editor101/codemirror/src/css/baboon.png \
	MrScheme/editor101/codemirror/src/css/baboon_vector.svg \
	MrScheme/editor101/codemirror/src/css/docs.css \
	MrScheme/editor101/codemirror/src/lib/codemirror.css \
	MrScheme/editor101/codemirror/src/lib/codemirror.js \
	MrScheme/editor101/codemirror/src/lib/overlay.js \
	MrScheme/editor101/codemirror/src/lib/runmode.js \
	MrScheme/jquery101/jquery-1.6.2.js \
	MrScheme/jquery101/apprise-1.5.full.js \
	MrScheme/jquery101/apprise.css \
	MrScheme/jquery101/tooltips.css \
	MrScheme/jquery101/tooltips.js \
	MrScheme/media101/elder.png \
	MrScheme/media101/ftendturn.png \
	MrScheme/media101/ftfileimport.png \
	MrScheme/media101/ftfileexport.png \
	MrScheme/media101/ftkate.png \
	MrScheme/media101/ftkexi.png \
	MrScheme/media101/fthelp.png \
	MrScheme/message101/message101.js \
	MrScheme/message101/messages-fr.js \
	MrScheme/canvas101/canvas101.css \
	MrScheme/canvas101/images101.js \
	MrScheme/canvas101/jcanvas.js \
	MrScheme/type101/type101.js \
	MrScheme/doc101/MrScheme-help.css \
	MrScheme/doc101/MrScheme-help-fr.html \
	MrScheme/save101/BlobBuilder.js \
	MrScheme/save101/FileSaver.js

mrscheme_desktopdocdir = ${prefix}/doc/mrscheme_desktop
mrscheme_desktopdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = m4/ChangeLog  $(mrscheme_desktopdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(mrscheme_desktopdocdir)

ACLOCAL_AMFLAGS = -I m4

