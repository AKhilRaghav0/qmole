# Makefile.in generated by automake 1.11.1 from Makefile.am.
# babl/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
# 2003, 2004, 2005, 2006, 2007, 2008, 2009  Free Software Foundation,
# Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.






pkgdatadir = $(datadir)/babl
pkgincludedir = $(includedir)/babl
pkglibdir = $(libdir)/babl
pkglibexecdir = $(libexecdir)/babl
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = arm-apple-darwin11.0.0
host_triplet = arm-apple-darwin11.0.0
subdir = babl
DIST_COMMON = $(library_include_HEADERS) $(srcdir)/Makefile.am \
	$(srcdir)/Makefile.in $(srcdir)/babl-version.h.in \
	gettimeofday.c
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/m4/introspection.m4 \
	$(top_srcdir)/m4/libtool.m4 $(top_srcdir)/m4/ltoptions.m4 \
	$(top_srcdir)/m4/ltsugar.m4 $(top_srcdir)/m4/ltversion.m4 \
	$(top_srcdir)/m4/lt~obsolete.m4 $(top_srcdir)/acinclude.m4 \
	$(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES = babl-version.h
CONFIG_CLEAN_VPATH_FILES =
am__vpath_adj_setup = srcdirstrip=`echo "$(srcdir)" | sed 's|.|.|g'`;
am__vpath_adj = case $$p in \
    $(srcdir)/*) f=`echo "$$p" | sed "s|^$$srcdirstrip/||"`;; \
    *) f=$$p;; \
  esac;
am__strip_dir = f=`echo $$p | sed -e 's|^.*/||'`;
am__install_max = 40
am__nobase_strip_setup = \
  srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*|]/\\\\&/g'`
am__nobase_strip = \
  for p in $$list; do echo "$$p"; done | sed -e "s|$$srcdirstrip/||"
am__nobase_list = $(am__nobase_strip_setup); \
  for p in $$list; do echo "$$p $$p"; done | \
  sed "s| $$srcdirstrip/| |;"' / .*\//!s/ .*/ ./; s,\( .*\)/[^/]*$$,\1,' | \
  $(AWK) 'BEGIN { files["."] = "" } { files[$$2] = files[$$2] " " $$1; \
    if (++n[$$2] == $(am__install_max)) \
      { print $$2, files[$$2]; n[$$2] = 0; files[$$2] = "" } } \
    END { for (dir in files) print dir, files[dir] }'
am__base_list = \
  sed '$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;s/\n/ /g' | \
  sed '$$!N;$$!N;$$!N;$$!N;s/\n/ /g'
am__installdirs = "$(DESTDIR)$(libdir)" "$(DESTDIR)$(girdir)" \
	"$(DESTDIR)$(typelibsdir)" "$(DESTDIR)$(library_includedir)"
LTLIBRARIES = $(lib_LTLIBRARIES)
libbabl_0.1_la_DEPENDENCIES = base/libbase.la \
	 $(am__empty)
am__objects_1 =
am__objects_2 = babl.lo babl-component.lo babl-conversion.lo \
	babl-core.lo babl-db.lo babl-extension.lo babl-fish-path.lo \
	babl-fish-reference.lo babl-fish-simple.lo babl-fish-stats.lo \
	babl-fish.lo babl-format.lo babl-hash-table.lo babl-image.lo \
	babl-internal.lo babl-introspect.lo babl-list.lo \
	babl-memory.lo babl-model.lo babl-mutex.lo babl-sampling.lo \
	babl-sanity.lo babl-type.lo babl-util.lo babl-cpuaccel.lo \
	babl-version.lo
am_libbabl_0.1_la_OBJECTS = $(am__objects_1) \
	$(am__objects_2)
libbabl_0.1_la_OBJECTS =  \
	$(am_libbabl_0.1_la_OBJECTS)
AM_V_lt = $(am__v_lt_$(V))
am__v_lt_ = $(am__v_lt_$(AM_DEFAULT_VERBOSITY))
am__v_lt_0 = --silent
libbabl_0.1_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC \
	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CCLD) \
	$(AM_CFLAGS) $(CFLAGS) \
	$(libbabl_0.1_la_LDFLAGS) $(LDFLAGS) -o $@
DEFAULT_INCLUDES = -I. -I$(top_builddir)
depcomp = $(SHELL) $(top_srcdir)/depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS) $(CFLAGS)
AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC    " $@;
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
CCLD = $(CC)
LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD  " $@;
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN   " $@;
SOURCES = $(libbabl_0.1_la_SOURCES)
DIST_SOURCES = $(libbabl_0.1_la_SOURCES)
RECURSIVE_TARGETS = all-recursive check-recursive dvi-recursive \
	html-recursive info-recursive install-data-recursive \
	install-dvi-recursive install-exec-recursive \
	install-html-recursive install-info-recursive \
	install-pdf-recursive install-ps-recursive install-recursive \
	installcheck-recursive installdirs-recursive pdf-recursive \
	ps-recursive uninstall-recursive
DATA = $(gir_DATA) $(typelibs_DATA)
HEADERS = $(library_include_HEADERS)
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
AM_RECURSIVE_TARGETS = $(RECURSIVE_TARGETS:-recursive=) \
	$(RECURSIVE_CLEAN_TARGETS:-recursive=) tags TAGS ctags CTAGS \
	distdir
ETAGS = etags
CTAGS = ctags
DIST_SUBDIRS = $(SUBDIRS)
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
am__relativize = \
  dir0=`pwd`; \
  sed_first='s,^\([^/]*\)/.*$$,\1,'; \
  sed_rest='s,^[^/]*/*,,'; \
  sed_last='s,^.*/\([^/]*\)$$,\1,'; \
  sed_butlast='s,/*[^/]*$$,,'; \
  while test -n "$$dir1"; do \
    first=`echo "$$dir1" | sed -e "$$sed_first"`; \
    if test "$$first" != "."; then \
      if test "$$first" = ".."; then \
        dir2=`echo "$$dir0" | sed -e "$$sed_last"`/"$$dir2"; \
        dir0=`echo "$$dir0" | sed -e "$$sed_butlast"`; \
      else \
        first2=`echo "$$dir2" | sed -e "$$sed_first"`; \
        if test "$$first2" = "$$first"; then \
          dir2=`echo "$$dir2" | sed -e "$$sed_rest"`; \
        else \
          dir2="../$$dir2"; \
        fi; \
        dir0="$$dir0"/"$$first"; \
      fi; \
    fi; \
    dir1=`echo "$$dir1" | sed -e "$$sed_rest"`; \
  done; \
  reldir="$$dir2"
ACLOCAL = ${SHELL} /home/chris/babl-0.1.4/missing --run aclocal-1.11
ALTIVEC_EXTRA_CFLAGS = 
AMTAR = ${SHELL} /home/chris/babl-0.1.4/missing --run tar
AM_DEFAULT_VERBOSITY = 0
AR = ar
AS = as
AUTOCONF = ${SHELL} /home/chris/babl-0.1.4/missing --run autoconf
AUTOHEADER = ${SHELL} /home/chris/babl-0.1.4/missing --run autoheader
AUTOMAKE = ${SHELL} /home/chris/babl-0.1.4/missing --run automake-1.11
AWK = gawk
BABL_API_VERSION = 0.1
BABL_BINARY_AGE = 104
BABL_CURRENT_MINUS_AGE = 0
BABL_INTERFACE_AGE = 1
BABL_LIBRARY_VERSION = 103:1:103
BABL_MAJOR_VERSION = 0
BABL_MICRO_VERSION = 4
BABL_MINOR_VERSION = 1
BABL_REAL_VERSION = 0.1.4
BABL_RELEASE = 0.1
BABL_UNSTABLE = yes
BABL_VERSION = 0.1.4
CC = /usr/bin/gcc
CCDEPMODE = depmode=gcc3
CFLAGS = -g -O2 -Wall -Wdeclaration-after-statement -Wmissing-prototypes -Wmissing-declarations -Winit-self -Wpointer-arith -Wold-style-definition
CPP = /usr/bin/gcc -E
CPPFLAGS = 
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DIR_SEP = /
DLLTOOL = dlltool
DSYMUTIL = :
DUMPBIN = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
EXEEXT = 
FGREP = /bin/grep -F
GREP = /bin/grep
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
INTROSPECTION_CFLAGS = 
INTROSPECTION_COMPILER = 
INTROSPECTION_GENERATE = 
INTROSPECTION_GIRDIR = 
INTROSPECTION_LIBS = 
INTROSPECTION_MAKEFILE = 
INTROSPECTION_SCANNER = 
INTROSPECTION_TYPELIBDIR = 
LD = /usr/bin/ld
LDFLAGS = 
LIBOBJS = 
LIBS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIPO = lipo
LN_S = ln -s
LTLIBOBJS = 
MAINT = #
MAKEINFO = ${SHELL} /home/chris/babl-0.1.4/missing --run makeinfo
MATH_LIB = -lm
MKDIR_P = /bin/mkdir -p
MMX_EXTRA_CFLAGS = 
NM = /usr/bin/nm -p
NMEDIT = nmedit
OBJDUMP = objdump
OBJEXT = o
OTOOL = otool
OTOOL64 = otool64
PACKAGE = babl
PACKAGE_BUGREPORT = 
PACKAGE_NAME = babl
PACKAGE_STRING = babl 0.1.4
PACKAGE_TARNAME = babl
PACKAGE_URL = 
PACKAGE_VERSION = 0.1.4
PATH_SEP = :
PATH_SEPARATOR = :
PKG_CONFIG = /usr/local/bin/pkg-config
PKG_CONFIG_LIBDIR = 
PKG_CONFIG_PATH = /usr/local/lib/pkgconfig:/usr/lib/pkgconfig
RANLIB = ranlib
RSVG = /usr/local/bin/rsvg
SED = /bin/sed
SET_MAKE = 
SHELL = /bin/sh
SHREXT = .dylib
SSE_EXTRA_CFLAGS = 
STRIP = strip
VERSION = 0.1.4
W3M = /usr/local/bin/w3m
WEBSITE_HOST = pippin.gimp.org
WEBSITE_LOCATION = public_html/babl/
abs_builddir = /home/chris/babl-0.1.4/babl
abs_srcdir = /home/chris/babl-0.1.4/babl
abs_top_builddir = /home/chris/babl-0.1.4
abs_top_srcdir = /home/chris/babl-0.1.4
ac_ct_CC = /usr/bin/gcc
ac_ct_DUMPBIN = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = ${AMTAR} chof - "$$tardir"
am__untar = ${AMTAR} xf -
bindir = ${exec_prefix}/bin
build = arm-apple-darwin11.0.0
build_alias = 
build_cpu = arm
build_os = darwin11.0.0
build_vendor = apple
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
host = arm-apple-darwin11.0.0
host_alias = 
host_cpu = arm
host_os = darwin11.0.0
host_vendor = apple
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /home/chris/babl-0.1.4/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mkdir_p = /bin/mkdir -p
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = 
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..
#no_undefined = -no-undefined
SUBDIRS = base
c_sources = \
	babl.c				\
	babl-component.c		\
	babl-conversion.c		\
	babl-core.c			\
	babl-db.c			\
	babl-extension.c		\
	babl-fish-path.c		\
	babl-fish-reference.c		\
	babl-fish-simple.c		\
	babl-fish-stats.c		\
	babl-fish.c			\
	babl-format.c			\
	babl-hash-table.c		\
	babl-image.c			\
	babl-internal.c			\
	babl-introspect.c		\
	babl-list.c			\
	babl-memory.c			\
	babl-model.c			\
	babl-mutex.c			\
	babl-sampling.c			\
	babl-sanity.c			\
	babl-type.c			\
	babl-util.c			\
	babl-cpuaccel.c			\
	babl-version.c

h_sources = \
	babl.h				\
	babl-class.h			\
	babl-classes.h			\
	babl-component.h		\
	babl-conversion.h		\
	babl-cpuaccel.h			\
	babl-db.h			\
	babl-extension.h		\
	babl-fish.h			\
	babl-format.h			\
	babl-hash-table.h		\
	babl-ids.h			\
	babl-image.h			\
	babl-internal.h			\
	babl-introspect.h		\
	babl-list.h			\
	babl-macros.h			\
	babl-memory.h			\
	babl-model.h			\
	babl-mutex.h			\
	babl-sampling.h			\
	babl-type.h			\
	babl-types.h			\
	babl-util.h

library_includedir = $(includedir)/babl-$(BABL_API_VERSION)/babl
library_include_HEADERS = \
	babl-macros.h			\
	babl-types.h                    \
	babl-version.h			\
	babl.h

AM_CPPFLAGS = \
	-DLIBDIR=\""$(libdir)"\"	\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/babl/base

lib_LTLIBRARIES = libbabl-0.1.la
libbabl_0.1_la_SOURCES = $(h_sources) $(c_sources)
libbabl_0.1_la_LIBADD = \
	base/libbase.la \
	

libbabl_0.1_la_LDFLAGS = \
	${no_undefined} $(MATH_LIB)	\
	-version-info $(BABL_LIBRARY_VERSION)

INTROSPECTION_GIRS = 
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)
#girdir = $(datadir)/gir-1.0
#gir_DATA = Babl-$(BABL_API_VERSION).gir
#typelibsdir = $(libdir)/girepository-1.0
#typelibs_DATA = Babl-$(BABL_API_VERSION).typelib
#EXTRA_DIST = $(gir_DATA)
#CLEANFILES = $(gir_DATA) $(typelibs_DATA)
all: all-recursive

.SUFFIXES:
.SUFFIXES: .c .lo .o .obj
$(srcdir)/Makefile.in: # $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu babl/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu babl/Makefile
.PRECIOUS: Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure: # $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4): # $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):
babl-version.h: $(top_builddir)/config.status $(srcdir)/babl-version.h.in
	cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@
install-libLTLIBRARIES: $(lib_LTLIBRARIES)
	@$(NORMAL_INSTALL)
	test -z "$(libdir)" || $(MKDIR_P) "$(DESTDIR)$(libdir)"
	@list='$(lib_LTLIBRARIES)'; test -n "$(libdir)" || list=; \
	list2=; for p in $$list; do \
	  if test -f $$p; then \
	    list2="$$list2 $$p"; \
	  else :; fi; \
	done; \
	test -z "$$list2" || { \
	  echo " $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$list2 '$(DESTDIR)$(libdir)'"; \
	  $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$list2 "$(DESTDIR)$(libdir)"; \
	}

uninstall-libLTLIBRARIES:
	@$(NORMAL_UNINSTALL)
	@list='$(lib_LTLIBRARIES)'; test -n "$(libdir)" || list=; \
	for p in $$list; do \
	  $(am__strip_dir) \
	  echo " $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=uninstall rm -f '$(DESTDIR)$(libdir)/$$f'"; \
	  $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=uninstall rm -f "$(DESTDIR)$(libdir)/$$f"; \
	done

clean-libLTLIBRARIES:
	-test -z "$(lib_LTLIBRARIES)" || rm -f $(lib_LTLIBRARIES)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  dir="`echo $$p | sed -e 's|/[^/]*$$||'`"; \
	  test "$$dir" != "$$p" || dir=.; \
	  echo "rm -f \"$${dir}/so_locations\""; \
	  rm -f "$${dir}/so_locations"; \
	done
libbabl-0.1.la: $(libbabl_0.1_la_OBJECTS) $(libbabl_0.1_la_DEPENDENCIES) 
	$(AM_V_CCLD)$(libbabl_0.1_la_LINK) -rpath $(libdir) $(libbabl_0.1_la_OBJECTS) $(libbabl_0.1_la_LIBADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/gettimeofday.Plo
include ./$(DEPDIR)/babl-component.Plo
include ./$(DEPDIR)/babl-conversion.Plo
include ./$(DEPDIR)/babl-core.Plo
include ./$(DEPDIR)/babl-cpuaccel.Plo
include ./$(DEPDIR)/babl-db.Plo
include ./$(DEPDIR)/babl-extension.Plo
include ./$(DEPDIR)/babl-fish-path.Plo
include ./$(DEPDIR)/babl-fish-reference.Plo
include ./$(DEPDIR)/babl-fish-simple.Plo
include ./$(DEPDIR)/babl-fish-stats.Plo
include ./$(DEPDIR)/babl-fish.Plo
include ./$(DEPDIR)/babl-format.Plo
include ./$(DEPDIR)/babl-hash-table.Plo
include ./$(DEPDIR)/babl-image.Plo
include ./$(DEPDIR)/babl-internal.Plo
include ./$(DEPDIR)/babl-introspect.Plo
include ./$(DEPDIR)/babl-list.Plo
include ./$(DEPDIR)/babl-memory.Plo
include ./$(DEPDIR)/babl-model.Plo
include ./$(DEPDIR)/babl-mutex.Plo
include ./$(DEPDIR)/babl-sampling.Plo
include ./$(DEPDIR)/babl-sanity.Plo
include ./$(DEPDIR)/babl-type.Plo
include ./$(DEPDIR)/babl-util.Plo
include ./$(DEPDIR)/babl-version.Plo
include ./$(DEPDIR)/babl.Plo

.c.o:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC) \
#	source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(COMPILE) -c $<

.c.obj:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC) \
#	source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(COMPILE) -c `$(CYGPATH_W) '$<'`

.c.lo:
	$(AM_V_CC)$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
#	$(AM_V_CC) \
#	source='$<' object='$@' libtool=yes \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(LTCOMPILE) -c -o $@ $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs
install-girDATA: $(gir_DATA)
	@$(NORMAL_INSTALL)
	test -z "$(girdir)" || $(MKDIR_P) "$(DESTDIR)$(girdir)"
	@list='$(gir_DATA)'; test -n "$(girdir)" || list=; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(girdir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(girdir)" || exit $$?; \
	done

uninstall-girDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(gir_DATA)'; test -n "$(girdir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	test -n "$$files" || exit 0; \
	echo " ( cd '$(DESTDIR)$(girdir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(girdir)" && rm -f $$files
install-typelibsDATA: $(typelibs_DATA)
	@$(NORMAL_INSTALL)
	test -z "$(typelibsdir)" || $(MKDIR_P) "$(DESTDIR)$(typelibsdir)"
	@list='$(typelibs_DATA)'; test -n "$(typelibsdir)" || list=; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(typelibsdir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(typelibsdir)" || exit $$?; \
	done

uninstall-typelibsDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(typelibs_DATA)'; test -n "$(typelibsdir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	test -n "$$files" || exit 0; \
	echo " ( cd '$(DESTDIR)$(typelibsdir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(typelibsdir)" && rm -f $$files
install-library_includeHEADERS: $(library_include_HEADERS)
	@$(NORMAL_INSTALL)
	test -z "$(library_includedir)" || $(MKDIR_P) "$(DESTDIR)$(library_includedir)"
	@list='$(library_include_HEADERS)'; test -n "$(library_includedir)" || list=; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_HEADER) $$files '$(DESTDIR)$(library_includedir)'"; \
	  $(INSTALL_HEADER) $$files "$(DESTDIR)$(library_includedir)" || exit $$?; \
	done

uninstall-library_includeHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(library_include_HEADERS)'; test -n "$(library_includedir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	test -n "$$files" || exit 0; \
	echo " ( cd '$(DESTDIR)$(library_includedir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(library_includedir)" && rm -f $$files

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.
$(RECURSIVE_TARGETS):
	@fail= failcom='exit 1'; \
	for f in x $$MAKEFLAGS; do \
	  case $$f in \
	    *=* | --[!k]*);; \
	    *k*) failcom='fail=yes';; \
	  esac; \
	done; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

$(RECURSIVE_CLEAN_TARGETS):
	@fail= failcom='exit 1'; \
	for f in x $$MAKEFLAGS; do \
	  case $$f in \
	    *=* | --[!k]*);; \
	    *k*) failcom='fail=yes';; \
	  esac; \
	done; \
	dot_seen=no; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	rev=''; for subdir in $$list; do \
	  if test "$$subdir" = "."; then :; else \
	    rev="$$subdir $$rev"; \
	  fi; \
	done; \
	rev="$$rev ."; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done
ctags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) ctags); \
	done

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	mkid -fID $$unique
tags: TAGS

TAGS: tags-recursive $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	set x; \
	here=`pwd`; \
	if ($(ETAGS) --etags-include --version) >/dev/null 2>&1; then \
	  include_option=--etags-include; \
	  empty_fix=.; \
	else \
	  include_option=--include; \
	  empty_fix=; \
	fi; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test ! -f $$subdir/TAGS || \
	      set "$$@" "$$include_option=$$here/$$subdir/TAGS"; \
	  fi; \
	done; \
	list='$(SOURCES) $(HEADERS)  $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: CTAGS
CTAGS: ctags-recursive $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	list='$(SOURCES) $(HEADERS)  $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
	@list='$(DIST_SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test -d "$(distdir)/$$subdir" \
	    || $(MKDIR_P) "$(distdir)/$$subdir" \
	    || exit 1; \
	  fi; \
	done
	@list='$(DIST_SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    dir1=$$subdir; dir2="$(distdir)/$$subdir"; \
	    $(am__relativize); \
	    new_distdir=$$reldir; \
	    dir1=$$subdir; dir2="$(top_distdir)"; \
	    $(am__relativize); \
	    new_top_distdir=$$reldir; \
	    echo " (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir="$$new_top_distdir" distdir="$$new_distdir" \\"; \
	    echo "     am__remove_distdir=: am__skip_length_check=: am__skip_mode_fix=: distdir)"; \
	    ($(am__cd) $$subdir && \
	      $(MAKE) $(AM_MAKEFLAGS) \
	        top_distdir="$$new_top_distdir" \
	        distdir="$$new_distdir" \
		am__remove_distdir=: \
		am__skip_length_check=: \
		am__skip_mode_fix=: \
	        distdir) \
	      || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-recursive
all-am: Makefile $(LTLIBRARIES) $(DATA) $(HEADERS)
installdirs: installdirs-recursive
installdirs-am:
	for dir in "$(DESTDIR)$(libdir)" "$(DESTDIR)$(girdir)" "$(DESTDIR)$(typelibsdir)" "$(DESTDIR)$(library_includedir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-recursive

clean-am: clean-generic clean-libLTLIBRARIES clean-libtool \
	mostlyclean-am

distclean: distclean-recursive
	-rm -rf $(DEPDIR) ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-recursive

dvi-am:

html: html-recursive

html-am:

info: info-recursive

info-am:

install-data-am: install-girDATA install-library_includeHEADERS \
	install-typelibsDATA

install-dvi: install-dvi-recursive

install-dvi-am:

install-exec-am: install-libLTLIBRARIES

install-html: install-html-recursive

install-html-am:

install-info: install-info-recursive

install-info-am:

install-man:

install-pdf: install-pdf-recursive

install-pdf-am:

install-ps: install-ps-recursive

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-recursive
	-rm -rf $(DEPDIR) ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

pdf: pdf-recursive

pdf-am:

ps: ps-recursive

ps-am:

uninstall-am: uninstall-girDATA uninstall-libLTLIBRARIES \
	uninstall-library_includeHEADERS uninstall-typelibsDATA

.MAKE: $(RECURSIVE_CLEAN_TARGETS) $(RECURSIVE_TARGETS) ctags-recursive \
	install-am install-strip tags-recursive

.PHONY: $(RECURSIVE_CLEAN_TARGETS) $(RECURSIVE_TARGETS) CTAGS GTAGS \
	all all-am check check-am clean clean-generic \
	clean-libLTLIBRARIES clean-libtool ctags ctags-recursive \
	distclean distclean-compile distclean-generic \
	distclean-libtool distclean-tags distdir dvi dvi-am html \
	html-am info info-am install install-am install-data \
	install-data-am install-dvi install-dvi-am install-exec \
	install-exec-am install-girDATA install-html install-html-am \
	install-info install-info-am install-libLTLIBRARIES \
	install-library_includeHEADERS install-man install-pdf \
	install-pdf-am install-ps install-ps-am install-strip \
	install-typelibsDATA installcheck installcheck-am installdirs \
	installdirs-am maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool pdf pdf-am ps ps-am tags tags-recursive \
	uninstall uninstall-am uninstall-girDATA \
	uninstall-libLTLIBRARIES uninstall-library_includeHEADERS \
	uninstall-typelibsDATA


# GObject Introspection
-include $(INTROSPECTION_MAKEFILE)

#Babl-$(BABL_API_VERSION).gir: $(G_IR_SCANNER) $(library_include_HEADERS) $(c_sources) $(srcdir)/Makefile.am
#	$(INTROSPECTION_SCANNER) -v --namespace Babl --nsversion=$(BABL_API_VERSION) \
#            --add-include-path=$(srcdir) --add-include-path=. \
#            --library=babl-$(BABL_API_VERSION) \
#            --libtool="$(LIBTOOL)" \
#            --output $@ \
#            --pkg babl \
#            -DBABL_IS_BEING_COMPILED \
#	    -I$(top_srcdir) \
#	    -I$(top_builddir) \
#	    $(addprefix $(srcdir)/, $(library_include_HEADERS)) \
#	    $(addprefix $(srcdir)/, $(c_sources))

#%.typelib: %.gir
#	LD_LIBRARY_PATH=$${LD_LIBRARY_PATH:+$$LD_LIBRARY_PATH:}. $(INTROSPECTION_COMPILER) \
#        --includedir=$(srcdir) \
#        --includedir=$(top_builddir)/babl \
#        $(G_IR_COMPILER_OPTS) \
#        $< -o $@

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
