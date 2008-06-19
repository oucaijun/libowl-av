AC_PREREQ(2.53)
AC_INIT(owl-video-widget, 0.1, http://www.openedhand.com/)
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR(src/owl-video-widget.c)
AM_CONFIG_HEADER(src/config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_CC_STDC
AC_STDC_HEADERS
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(VIDEO, gtk+-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
VIDEO_LIBS="$VIDEO_LIBS -lgstinterfaces-0.10"

AC_SUBST(VIDEO_CFLAGS)
AC_SUBST(VIDEO_LIBS)

AC_CONFIG_FILES([
		Makefile
		owl-video-widget.pc
		src/Makefile
])

AC_OUTPUT

echo " "
AC_MSG_RESULT([Installing in: ${prefix}])

