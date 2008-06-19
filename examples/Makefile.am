INCLUDES = -I$(top_srcdir)				

AM_CPPFLAGS = -DPKGDATADIR=\"$(pkgdatadir)\"

AM_CFLAGS = -Wall -export-dynamic $(VIDEO_CFLAGS)

bin_PROGRAMS = video

video_SOURCES = video.c	\
	bacon-volume.c	\
	bacon-volume.h  \
	owl-overlay-bin.c	\
	owl-overlay-bin.h

video_LDADD = $(VIDEO_LIBS) $(top_builddir)/src/libowl-video-widget.la

dist_pkgdata_DATA = gtk-fullscreen.png

MAINTAINERCLEANFILES = config.h.in Makefile.in
