## Makefile.am for gtk+/gdk
include $(top_srcdir)/Makefile.decl
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = \
	--add-include-path=../gdk \
	--warn-all
INTROSPECTION_COMPILER_ARGS = \
   --includedir=$(srcdir) \
   --includedir=.

SUBDIRS = $(GDK_BACKENDS) . tests

DIST_SUBDIRS = win32 x11 quartz broadway wayland tests

CLEANFILES =

EXTRA_DIST += 			\
	keynames.txt		\
	keyname-table.h		\
	gdkkeynames.c		\
	gen-keyname-table.pl	\
	gdkconfig.h.win32	\
	gdkkeysyms-update.pl	\
	gdk.def			\
	gdkmarshalers.list	\
	gdkwindowimpl.h		\
	makefile.msc		\
	gdk.symbols		\
	gdkenumtypes.c.template	\
	gdkenumtypes.h.template	\
	abicheck.sh

AM_CPPFLAGS = 				\
	-DG_LOG_DOMAIN=\"Gdk\"		\
	-DGDK_COMPILATION		\
	-I$(top_srcdir)			\
	-I$(top_builddir)		\
	-I$(top_builddir)/gdk		\
	$(GTK_DEBUG_FLAGS)		\
	$(GDK_DEP_CFLAGS)

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

# libtool stuff: set version and export symbols for resolving
# since automake doesn't support conditionalized libsomething_la_LDFLAGS
# we use the general approach here
LDADD = 					\
	$(GTK_LINK_FLAGS)			\
	-version-info $(LT_VERSION_INFO)	\
	-export-dynamic 			\
	-rpath $(libdir) 			\
	$(no_undefined)				\
	$(LIBTOOL_EXPORT_OPTIONS)

#
# setup source file variables
#
#
# GDK header files for public installation (non-generated)
#
gdk_public_h_sources = 				\
	gdk.h					\
	gdkapplaunchcontext.h			\
	gdkcairo.h				\
	gdkcolor.h				\
	gdkcursor.h				\
	gdkdevice.h				\
	gdkdevicemanager.h			\
	gdkdisplay.h				\
	gdkdisplaymanager.h			\
	gdkdnd.h				\
	gdkevents.h				\
	gdkkeys.h				\
	gdkkeysyms.h				\
	gdkkeysyms-compat.h			\
	gdkmain.h				\
	gdkpango.h				\
	gdkpixbuf.h				\
	gdkprivate.h				\
	gdkproperty.h				\
	gdkrectangle.h				\
	gdkrgba.h				\
	gdkscreen.h				\
	gdkselection.h				\
	gdktestutils.h				\
	gdkthreads.h				\
	gdktypes.h				\
	gdkvisual.h				\
	gdkwindow.h

gdk_private_headers = 				\
	gdkapplaunchcontextprivate.h		\
	gdkcursorprivate.h			\
	gdkdevicemanagerprivate.h		\
	gdkdeviceprivate.h			\
	gdkdisplaymanagerprivate.h		\
	gdkdisplayprivate.h			\
	gdkdndprivate.h				\
	gdkscreenprivate.h			\
	gdkinternals.h				\
	gdkintl.h				\
	gdkkeysprivate.h			\
	gdkvisualprivate.h			\
	gdkx.h

gdk_c_sources = 				\
	gdk.c					\
	gdkapplaunchcontext.c			\
	gdkcairo.c				\
	gdkcolor.c				\
	gdkcursor.c				\
	gdkdevice.c				\
	gdkdevicemanager.c			\
	gdkdisplay.c				\
	gdkdisplaymanager.c			\
	gdkdnd.c				\
	gdkevents.c     			\
	gdkglobals.c				\
	gdkkeys.c				\
	gdkkeyuni.c				\
	gdkoffscreenwindow.c			\
	gdkpango.c				\
	gdkpixbuf-drawable.c			\
	gdkrectangle.c				\
	gdkrgba.c				\
	gdkscreen.c				\
	gdkselection.c				\
	gdkvisual.c				\
	gdkwindow.c				\
	gdkwindowimpl.c

gdk_built_sources = 				\
	gdkenumtypes.c				\
	gdkmarshalers.h 			\
	gdkmarshalers.c				\
	gdkenumtypes.h

#
# setup GDK sources and their dependencies
#

gdkincludedir = $(includedir)/gtk-3.0/gdk
gdkinclude_HEADERS = $(gdk_public_h_sources) gdkenumtypes.h
nodist_gdkinclude_HEADERS = gdkconfig.h

common_sources = 		\
	$(gdk_private_headers)	\
	$(gdk_c_sources)	\
	gdkenumtypes.c		\
	gdkmarshalers.c		\
	gdkmarshalers.h

libgdk_3_la_SOURCES = $(common_sources)
libgdk_3_la_LIBADD = $(GDK_DEP_LIBS)
libgdk_3_la_LDFLAGS = $(LDADD)

if USE_X11
libgdk_3_la_LIBADD += x11/libgdk-x11.la
endif # USE_X11

if USE_QUARTZ
libgdk_3_la_LIBADD += quartz/libgdk-quartz.la
endif # USE_QUARTZ

if USE_WIN32
libgdk_3_la_SOURCES += gdkkeynames.c
libgdk_3_la_LIBADD += win32/libgdk-win32.la
libgdk_3_la_DEPENDENCIES = win32/libgdk-win32.la win32/rc/gdk-win32-res.o gdk.def
libgdk_3_la_LDFLAGS += -Wl,win32/rc/gdk-win32-res.o -export-symbols $(srcdir)/gdk.def
endif # USE_WIN32

if USE_BROADWAY
libgdk_3_la_LIBADD += broadway/libgdk-broadway.la
endif # USE_BROADWAY

if USE_WAYLAND
libgdk_3_la_LIBADD += wayland/libgdk-wayland.la
endif

if HAVE_INTROSPECTION

introspection_files = 		\
	$(filter-out gdkkeysyms-compat.h, $(gdk_public_h_sources))	\
	$(gdk_c_sources)	\
        gdkenumtypes.c		\
        gdkenumtypes.h

Gdk-3.0.gir: libgdk-3.la Makefile
Gdk_3_0_gir_SCANNERFLAGS = 	\
	--c-include="gdk/gdk.h"
Gdk_3_0_gir_INCLUDES = Gio-2.0 GdkPixbuf-2.0 Pango-1.0 cairo-1.0
Gdk_3_0_gir_LIBS = libgdk-3.la
Gdk_3_0_gir_FILES = $(introspection_files)
Gdk_3_0_gir_CFLAGS = $(AM_CPPFLAGS)
Gdk_3_0_gir_EXPORT_PACKAGES = gdk-3.0
INTROSPECTION_GIRS += Gdk-3.0.gir

if USE_X11
x11_introspection_files = 		\
	x11/checksettings.c		\
	x11/gdkapplaunchcontext-x11.c	\
	x11/gdkasync.c			\
	x11/gdkcursor-x11.c		\
	x11/gdkdevice-core-x11.c	\
	x11/gdkdevicemanager-core-x11.c	\
	x11/gdkdevicemanager-x11.c	\
	x11/gdkdevicemanager-xi.c	\
	x11/gdkdevicemanager-xi2.c	\
	x11/gdkdevice-xi2.c		\
	x11/gdkdevice-xi.c		\
	x11/gdkdisplay-x11.c		\
	x11/gdkdnd-x11.c		\
	x11/gdkeventsource.c		\
	x11/gdkeventtranslator.c	\
	x11/gdkgeometry-x11.c		\
	x11/gdkkeys-x11.c		\
	x11/gdkmain-x11.c		\
	x11/gdkproperty-x11.c		\
	x11/gdkscreen-x11.c		\
	x11/gdkselection-x11.c		\
	x11/gdksettings.c		\
	x11/gdktestutils-x11.c		\
	x11/gdkvisual-x11.c		\
	x11/gdkwindow-x11.c		\
	x11/gdkxftdefaults.c		\
	x11/gdkxid.c			\
	x11/xsettings-client.c		\
	x11/xsettings-common.c		\
	x11/gdkx.h			\
	x11/gdkx11cursor.h		\
	x11/gdkx11display.h		\
	x11/gdkx11property.h		\
	x11/gdkx11screen.h		\
	x11/gdkx11selection.h		\
	x11/gdkx11utils.h		\
	x11/gdkx11visual.h		\
	x11/gdkx11window.h

GdkX11-3.0.gir: libgdk-3.la Gdk-3.0.gir Makefile
GdkX11_3_0_gir_SCANNERFLAGS = 		\
	--identifier-prefix=Gdk		\
	--c-include="gdk/gdkx.h"	\
	--include-uninstalled=$(top_builddir)/gdk/Gdk-3.0.gir
GdkX11_3_0_gir_INCLUDES = Gio-2.0 GdkPixbuf-2.0 Pango-1.0 xlib-2.0
GdkX11_3_0_gir_LIBS = libgdk-3.la
GdkX11_3_0_gir_FILES = $(x11_introspection_files)
GdkX11_3_0_gir_CFLAGS = $(AM_CPPFLAGS) -L$(top_builddir)/gdk
GdkX11_3_0_gir_EXPORT_PACKAGES = gdk-x11-3.0
INTROSPECTION_GIRS += GdkX11-3.0.gir

endif # USE_X11

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)

endif # HAVE_INTROSPECTION

if OS_WIN32
install-def-file: gdk.def
	mkdir -p $(DESTDIR)$(libdir)
	$(INSTALL) $(srcdir)/gdk.def $(DESTDIR)$(libdir)/gdk-win32-3.0.def
uninstall-def-file:
	-rm $(DESTDIR)$(libdir)/gdk-win32-3.0.def
else
install-def-file:
uninstall-def-file:
endif

if MS_LIB_AVAILABLE
noinst_DATA = gdk-win32-$(GTK_API_VERSION).lib

gdk-win32-$(GTK_API_VERSION).lib: libgdk-win32-$(GTK_API_VERSION).la gdk.def
	lib -machine:@LIB_EXE_MACHINE_FLAG@ -name:libgdk-win32-$(GTK_API_VERSION)-@LT_CURRENT_MINUS_AGE@.dll -def:gdk.def -out:$@

install-ms-lib:
	mkdir -p $(DESTDIR)$(libdir)
	$(INSTALL) gdk-win32-$(GTK_API_VERSION).lib $(DESTDIR)$(libdir)

uninstall-ms-lib:
	-rm $(DESTDIR)$(libdir)/gdk-win32-$(GTK_API_VERSION).lib
else
install-ms-lib:
uninstall-ms-lib:
endif

# This places the generated .def file in srcdir, since it is expected to be there.
# (The one from a tarball is)
gdk.def: gdk.symbols
	$(AM_V_GEN) (echo -e EXPORTS; $(CPP) -P -DGDK_WINDOWING_WIN32 - <$(srcdir)/gdk.symbols | sed -e '/^$$/d' -e 's/^/	/' -e 's/G_GNUC_[^ ]*//g') > $(srcdir)/gdk.def

TESTS_ENVIRONMENT = srcdir="$(srcdir)"
if OS_LINUX
TESTS = abicheck.sh
endif

lib_LTLIBRARIES = libgdk-3.la

MAINTAINERCLEANFILES = $(gdk_built_sources) stamp-gdkenumtypes.h
EXTRA_DIST += $(gdk_built_sources)

install-exec-hook:
if DISABLE_EXPLICIT_DEPS
	$(SHELL) $(top_srcdir)/sanitize-la.sh $(DESTDIR)$(libdir)/libgdk-3.la
endif

BUILT_SOURCES = \
	$(gdk_built_sources)			\
	gdkconfig.h

gdkenumtypes.h: stamp-gdkenumtypes.h
	@true
stamp-gdkenumtypes.h: @REBUILD@ $(gdk_public_h_sources) gdkenumtypes.h.template
	$(AM_V_GEN) ( cd $(srcdir) && $(GLIB_MKENUMS) --template gdkenumtypes.h.template \
		$(gdk_public_h_sources) ) >> xgen-geth \
	&& (cmp -s xgen-geth gdkenumtypes.h || cp xgen-geth gdkenumtypes.h ) \
	&& rm -f xgen-geth \
	&& echo timestamp > $(@F)
gdkenumtypes.c: @REBUILD@ $(gdk_public_h_sources) gdkenumtypes.c.template
	$(AM_V_GEN) ( cd $(srcdir) && $(GLIB_MKENUMS) --template gdkenumtypes.c.template \
		$(gdk_public_h_sources) ) > xgen-getc \
	&& cp xgen-getc gdkenumtypes.c  \
	&& rm -f xgen-getc

#
# Marshaller generation
#
gdkmarshalers.h: @REBUILD@ gdkmarshalers.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=_gdk_marshal $(srcdir)/gdkmarshalers.list --header > gdkmarshalers-h.tmp \
	&& mv gdkmarshalers-h.tmp gdkmarshalers.h \
	|| ( rm -f gdkmarshalers-h.tmp && exit 1)
gdkmarshalers.c: @REBUILD@ gdkmarshalers.list
	$(AM_V_GEN) $(GLIB_GENMARSHAL) --prefix=_gdk_marshal $(srcdir)/gdkmarshalers.list --body > gdkmarshalers-c.tmp	\
	&& mv gdkmarshalers-c.tmp gdkmarshalers.c \
	|| ( rm -f gdkmarshalers-c.tmp && exit 1 )

gdkconfig.h: stamp-gc-h
	$(AM_V_GEN) if test -f gdkconfig.h; then :; \
	else rm -f stamp-gc-h; $(MAKE) stamp-gc-h; fi
stamp-gc-h: $(top_builddir)/config.status
	$(AM_V_at) cd $(top_builddir) && $(SHELL) ./config.status gdk/gdkconfig.h
	echo timestamp > stamp-gc-h

dist-hook: ../build/win32/vs9/gdk.vcproj ../build/win32/vs10/gdk.vcxproj ../build/win32/vs10/gdk.vcxproj.filters

../build/win32/vs9/gdk.vcproj: ../build/win32/vs9/gdk.vcprojin
	for F in $(libgdk_3_la_SOURCES); do \
		case $$F in \
		*.c) echo '   <File RelativePath="..\..\..\gdk\'$$F'" />' \
		     ;; \
		esac; \
	done >libgdk.sourcefiles
	$(CPP) -P - <$(top_srcdir)/build/win32/vs9/gdk.vcprojin >$@
	rm libgdk.sourcefiles
	
../build/win32/vs10/gdk.vcxproj: ../build/win32/vs10/gdk.vcxprojin
	for F in $(libgdk_3_la_SOURCES); do \
		case $$F in \
		*.c) echo '   <ClCompile Include="..\..\..\gdk\'$$F'" />' \
		     ;; \
		esac; \
	done >libgdk.vs10.sourcefiles
	$(CPP) -P - <$(top_srcdir)/build/win32/vs10/gdk.vcxprojin >$@
	rm libgdk.vs10.sourcefiles

../build/win32/vs10/gdk.vcxproj.filters: ../build/win32/vs10/gdk.vcxproj.filtersin
	for F in $(libgdk_3_la_SOURCES); do \
		case $$F in \
		*.c) echo '   <ClCompile Include="..\..\..\gdk\'$$F'"><Filter>Source Files</Filter></ClCompile>' \
		     ;; \
		esac; \
	done >libgdk.vs10.sourcefiles.filters
	$(CPP) -P - <$(top_srcdir)/build/win32/vs10/gdk.vcxproj.filtersin >$@
	rm libgdk.vs10.sourcefiles.filters

DISTCLEANFILES = gdkconfig.h stamp-gc-h

install-data-local: install-ms-lib install-def-file

uninstall-local: uninstall-ms-lib uninstall-def-file
	rm -f $(DESTDIR)$(configexecincludedir)/gdkconfig.h

# if srcdir!=builddir, clean out maintainer-clean files from builddir
# this allows dist to pass.
distclean-local:
	if test $(srcdir) != .; then \
	  rm -f $(MAINTAINERCLEANFILES); \
	fi

.PHONY: files

files:
	@files=`ls $(DISTFILES) 2> /dev/null `; for p in $$files; do \
	  echo $$p; \
	done

-include $(top_srcdir)/git.mk
