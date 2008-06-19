INCLUDES = -I$(top_srcdir)				

AM_CFLAGS = -Wall -export-dynamic $(VIDEO_WIDGET_CFLAGS)

lib_LTLIBRARIES = libowl-video-widget.la

libowl_video_widget_la_SOURCES = \
	owl-video-widget.c	\
	owl-video-widget.h

libowl_video_widget_la_LIBADD = $(VIDEO_WIDGET_LIBS)
libowl_video_widgetincludedir = $(includedir)/owl/
libowl_video_widgetinclude_HEADERS = owl-video-widget.h

video_LDADD = $(VIDEO_WIDGET_LIBS) libowl-video-widget.la

MAINTAINERCLEANFILES = config.h.in Makefile.in
