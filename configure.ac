dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(mrscheme_desktop, 0.1)

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 -Wall -Werror])
AM_SILENT_RULES([yes])

AC_PROG_CC

IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=mr_scheme
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])

AM_GLIB_GNU_GETTEXT

LT_INIT

PKG_CHECK_MODULES(gtk2, [gtk+-2.0])
PKG_CHECK_MODULES(webkit, [webkit-1.0])


AC_OUTPUT([ po/Makefile.in
Makefile
src/Makefile

])
