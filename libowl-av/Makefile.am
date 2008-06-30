INCLUDES = -I$(top_srcdir)				

AM_CFLAGS = -Wall -export-dynamic $(LIBOWL_AV_CFLAGS)

lib_LTLIBRARIES = libowl-av.la

BUILT_SOURCES = owl-marshal.c owl-marshal.h

libowl_av_la_SOURCES = \
	owl-audio-player.c	\
	owl-audio-player.h	\
	owl-tag-reader.c	\
	owl-tag-reader.h	\
	owl-video-widget.c	\
	owl-video-widget.h	\
	$(BUILT_SOURCES)

owl-marshal.h: owl-marshal.list
	$(GLIB_GENMARSHAL) --prefix=owl_marshal $< --header > $@

owl-marshal.c: owl-marshal.list
	$(GLIB_GENMARSHAL) --prefix=owl_marshal $< --body > $@

EXTRA_DIST = owl-marshal.list
CLEANFILES = $(BUILT_SOURCES)

libowl_av_la_LIBADD = $(LIBOWL_AV_LIBS)
libowl_avincludedir = $(includedir)/libowl-av/
libowl_avinclude_HEADERS = owl-video-widget.h \
			   owl-tag-reader.h   \
			   owl-audio-player.h

MAINTAINERCLEANFILES = config.h.in Makefile.in
