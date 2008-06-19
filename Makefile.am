SUBDIRS = src

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = owl-video-widget.pc

if WITH_VALA
vapidir = $(VAPIDIR)
vapi_DATA = owl-video-widget.vapi
endif

EXTRA_DIST = owl-video-widget.pc.in owl-video-widget.vapi

# Extra clean files so that maintainer-clean removes *everything*
MAINTAINERCLEANFILES = aclocal.m4 compile config.guess config.sub configure depcomp intltool-extract intltool-merge intltool-update install-sh ltmain.sh Makefile.in missing

snapshot:
        $(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`

