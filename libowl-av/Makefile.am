INCLUDES = -I$(top_srcdir)				

AM_CFLAGS = -Wall -export-dynamic $(LIBOWL_AV_CFLAGS)

lib_LTLIBRARIES = libowl-av.la

libowl_av_la_SOURCES = \
	owl-audio-player.c	\
	owl-audio-player.h	\
	owl-video-widget.c	\
	owl-video-widget.h

libowl_av_la_LIBADD = $(LIBOWL_AV_LIBS)
libowl_avincludedir = $(includedir)/libowl-av/
libowl_avinclude_HEADERS = owl-video-widget.h \
			   owl-audio-player.h

MAINTAINERCLEANFILES = config.h.in Makefile.in
