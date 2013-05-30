dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT([(Desktop (MrScheme))], 0.4, lancelot@lancelotsix.com, mrscheme-desktop, https://github.com/lancelotsix/mrscheme-desktop)

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 -Wall -Werror])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CXX

IT_PROG_INTLTOOL([0.35.0])

AC_CHECK_HEADER_STDBOOL
AC_FUNC_MALLOC
AC_TYPE_SIZE_T

GETTEXT_PACKAGE=mr_scheme
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])

AM_GLIB_GNU_GETTEXT

LT_INIT

PKG_CHECK_MODULES(gtk2, [gtk+-2.0])
PKG_CHECK_MODULES(webkit, [webkit-1.0])
PKG_CHECK_MODULES(curl, [libcurl])


AC_OUTPUT([ po/Makefile.in
Makefile
src/Makefile
])
