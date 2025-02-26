## Makefile.in for ICU - tools/icuinfo
## Copyright (c) 1999-2011, International Business Machines Corporation and
## others. All Rights Reserved.
## Madhu Katragadda

## Source directory information
srcdir = .
top_srcdir = ../..

top_builddir = ../..

include $(top_builddir)/icudefs.mk

## Build directory information
subdir = tools/icuinfo

## Extra files to remove for 'make clean'
CLEANFILES = *~ $(DEPS) $(PLUGIN_OBJECTS) $(PLUGINFILE) $(PLUGIN)

## Target information
TARGET = icuinfo$(EXEEXT)

ifneq ($(top_builddir),$(top_srcdir))
CPPFLAGS += -I$(top_builddir)/common 
endif
CPPFLAGS += -I$(top_srcdir)/common -I$(srcdir)/../toolutil -I$(top_srcdir)/tools/ctestfw 
CPPFLAGS+= -I$(top_srcdir)/i18n
LIBS = $(LIBICUTOOLUTIL) $(LIBICUI18N) $(LIBICUUC) $(DEFAULT_LIBS) $(LIB_M)

OBJECTS = icuinfo.o
PLUGIN_OBJECTS = testplug.o

DEPS = $(OBJECTS:.o=.d)

# pass some information

ICUINFO_OPTS=-i ../../data/out/build/$(ICUDATA_PLATFORM_NAME)

CPPFLAGS+=  "-DU_PLATFORM=\"U_DARWIN\""  "-DU_BUILD=\"arm-apple-darwin10.4.0\"" "-DU_HOST=\"arm-apple-darwin10.4.0\"" "-DU_CC=\"/usr/bin/gcc\"" "-DU_CXX=\"g++\""

## List of phony targets
.PHONY : all all-local install install-local clean clean-local		\
distclean distclean-local dist dist-local check check-local plugin-check

## Clear suffix list
.SUFFIXES :

## List of standard targets
all: all-local
install: install-local
clean: clean-local
distclean : distclean-local
dist: dist-local
check: all check-local

all-local: $(TARGET) 

install-local: all-local
	$(MKINSTALLDIRS) $(DESTDIR)$(bindir)
	$(INSTALL) $(TARGET) $(DESTDIR)$(bindir)

dist-local:

clean-local:
	test -z "$(CLEANFILES)" || $(RMV) $(CLEANFILES)
	$(RMV) $(TARGET) $(OBJECTS)

distclean-local: clean-local
	$(RMV) Makefile

check-local: $(TARGET)
	$(INVOKE) ./$(TARGET) $(ICUINFO_OPTS)

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	 && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status
-include Makefile.local

$(TARGET) : $(OBJECTS)
	$(LINK.cc) $(OUTOPT)$@ $^ $(LIBS) 
	$(POST_BUILD_STEP)

PLUGIN=$(LIBPREFIX)plugin.$(SO)
SO_TARGET=$(PLUGIN)

PLUGINDIR=$(shell pwd)

PLUGINFILE=$(PLUGINDIR)/icuplugins$(SO_TARGET_VERSION_MAJOR).txt

$(PLUGINFILE): Makefile
	echo "$(CURR_FULL_DIR)/$(PLUGIN)	myPlugin	x=4" > $@

CFLAGS+=$(SHAREDLIBCFLAGS)

$(PLUGIN): $(PLUGIN_OBJECTS)
	$(SHLIB.cc) $(SHAREDLIBCFLAGS) $(LD_SONAME) $(OUTOPT)$@ $^ $(LIBS)

plugin: $(PLUGIN)

plugin-check: $(PLUGIN) $(PLUGINFILE)
	$(INVOKE) ICU_PLUGINS="$(CURR_FULL_DIR)" ./$(TARGET) -v -L


ifeq (,$(MAKECMDGOALS))
-include $(DEPS)
else
ifneq ($(patsubst %clean,,$(MAKECMDGOALS)),)
-include $(DEPS)
endif
endif

