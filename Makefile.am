SUBDIRS = libowl-av

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libowl-av.pc

if WITH_VALA
vapidir = $(VAPIDIR)
vapi_DATA = libowl-av.vapi
endif

EXTRA_DIST = libowl-av.pc.in libowl-av.vapi

# Extra clean files so that maintainer-clean removes *everything*
MAINTAINERCLEANFILES = aclocal.m4 compile config.guess config.sub configure depcomp intltool-extract intltool-merge intltool-update install-sh ltmain.sh Makefile.in missing

snapshot:
        $(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`

