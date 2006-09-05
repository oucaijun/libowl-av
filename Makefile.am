SUBDIRS = src

EXTRA_DIST = 

# Extra clean files so that maintainer-clean removes *everything*
MAINTAINERCLEANFILES = aclocal.m4 compile config.guess config.sub configure depcomp intltool-extract intltool-merge intltool-update install-sh ltmain.sh Makefile.in missing

snapshot:
        $(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`

