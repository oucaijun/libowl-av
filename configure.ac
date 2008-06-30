AC_PREREQ(2.53)
AC_INIT(libowl-av, 0.1, http://www.openedhand.com/)
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR(libowl-av/owl-video-widget.c)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_CC_STDC
AC_STDC_HEADERS
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(LIBOWL_AV, gtk+-2.0 \
                             gstreamer-0.10 \
                             gstreamer-plugins-base-0.10)
LIBOWL_AV_LIBS="$LIBOWL_AV_LIBS -lgstinterfaces-0.10"

PKG_CHECK_MODULES(VALA, [vala-1.0 >= 0.3.1], have_vala=yes, have_vala=no)

AM_CONDITIONAL(WITH_VALA, [test "x$have_vala" = "xyes"])

if test "x$have_vala" = "xyes"; then
    VAPIDIR=`pkg-config --variable=vapidir vala-1.0`
    AC_SUBST(VAPIDIR)
fi

AC_SUBST(LIBOWL_AV_CFLAGS)
AC_SUBST(LIBOWL_AV_LIBS)

AC_CONFIG_FILES([
	Makefile
	libowl-av.pc
	src/Makefile
])

AC_OUTPUT

echo " "
AC_MSG_RESULT([Installing in: ${prefix}])

