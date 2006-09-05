INCLUDES = 					\
	-I$(top_srcdir)				
AM_CPPFLAGS=-DPKGDATADIR=\"$(pkgdatadir)\"				\
	-DDATADIR=\""$(datadir)"\"

AM_CFLAGS = -Wall -export-dynamic $(VIDEO_CFLAGS)

lib_LTLIBRARIES = libowl-video-widget.la


bin_PROGRAMS = video

video_SOURCES = video.c	\
	bacon-volume.c	\
	bacon-volume.h

libowl_video_widget_la_SOURCES = \
	owl-video-widget.c	\
	owl-video-widget.h	\
	owl-overlay-bin.c	\
	owl-overlay-bin.h

libowl_video_widget_la_LIBADD = $(VIDEO_LIBS)
libowl_video_widgetincludedir = $(includedir)/owl/
libowl_video_widgetinclude_HEADERS = \
	owl-video-widget.h	\
	owl-overlay-bin.h

video_LDADD   = $(VIDEO_LIBS) libowl-video-widget.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libowl-video-widget.pc

EXTRA_DIST=libowl-video-widget.pc.in

MAINTAINERCLEANFILES = config.h.in Makefile.in
