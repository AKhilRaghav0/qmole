# Makefile for lib-src subdirectory in GNU Emacs.
# Copyright (C) 1985, 1987, 1988, 1993, 1994, 2001, 2002, 2003, 2004,
#               2005, 2006, 2007, 2008, 2009, 2010  Free Software Foundation, Inc.

# This file is part of GNU Emacs.

# GNU Emacs is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# GNU Emacs is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.


# Avoid trouble on systems where the `SHELL' variable might be
# inherited from the environment.
SHELL = /bin/sh

# Following ../lisp/Makefile.in.
EMACS = ../src/emacs
EMACSOPT = -batch --no-site-file --multibyte

# ==================== Things `configure' will edit ====================

CC=/usr/bin/gcc
CFLAGS=-g -O2 -Wdeclaration-after-statement -Wno-pointer-sign  
version=23.2
configuration=arm-apple-darwin10.4.0
EXEEXT=

# Program name transformation.
TRANSFORM = s,x,x,

# ==================== Where To Install Things ====================

# The default location for installation.  Everything is placed in
# subdirectories of this directory.  The default values for many of
# the variables below are expressed in terms of this one, so you may
# not need to change them.  This is set with the --prefix option to
# `../configure'.
prefix=/usr/local

# Like `prefix', but used for architecture-specific files.  This is
# set with the --exec-prefix option to `../configure'.
exec_prefix=${prefix}

# Where to install Emacs and other binaries that people will want to
# run directly (like etags).  This is set with the --bindir option
# to `../configure'.
bindir=${exec_prefix}/bin

# Where to install and expect executable files to be run by Emacs
# rather than directly by users, and other architecture-dependent
# data.  ${archlibdir} is usually below this.  This is set with the
# --libexecdir option to `../configure'.
libexecdir=${exec_prefix}/libexec

# Directory for local state files for all programs.
localstatedir=${prefix}/var

# Where to find the source code.  This is set by the configure
# script's `--srcdir' option.  However, the value of ${srcdir} in
# this makefile is not identical to what was specified with --srcdir,
# since the variable here has `/lib-src' added at the end.

# We use $(srcdir) explicitly in dependencies so as not to depend on VPATH.
srcdir=/home/chris/emacs-23.2.x11/lib-src
VPATH=/home/chris/emacs-23.2.x11/lib-src

# The top-level source directory, also set by configure.
top_srcdir=/home/chris/emacs-23.2.x11

# ==================== Emacs-specific directories ====================

# These variables hold the values Emacs will actually use.  They are
# based on the values of the standard Make variables above.

# Where to put executables to be run by Emacs rather than the user.
# This path usually includes the Emacs version and configuration name,
# so that multiple configurations for multiple versions of Emacs may
# be installed at once.  This can be set with the --archlibdir option
# to `../configure'.
archlibdir=${libexecdir}/emacs/${version}/${configuration}

gamedir=${localstatedir}/games/emacs
gameuser=games

# ==================== Utility Programs for the Build =================

# ../configure figures out the correct values for these.
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
# By default, we uphold the dignity of our programs.
INSTALL_STRIP =

# ========================== Lists of Files ===========================

# Things that a user might actually run,
# which should be installed in bindir.
INSTALLABLES = etags${EXEEXT} ctags${EXEEXT} emacsclient${EXEEXT} b2m${EXEEXT} ebrowse${EXEEXT} 
INSTALLABLE_SCRIPTS = rcs-checkin grep-changelog

# Things that Emacs runs internally, or during the build process,
#  which should not be installed in bindir.
UTILITIES=  profile${EXEEXT} digest-doc${EXEEXT} sorted-doc${EXEEXT} \
            movemail${EXEEXT} fakemail${EXEEXT} \
            hexl${EXEEXT} update-game-score${EXEEXT}

DONT_INSTALL= test-distrib${EXEEXT} make-docfile${EXEEXT}

# Like UTILITIES, but they're not system-dependent, and should not be
#  deleted by the distclean target.
SCRIPTS= rcs2log vcdiff

EXECUTABLES= ${UTILITIES} ${INSTALLABLES} ${SCRIPTS} ${INSTALLABLE_SCRIPTS}

# Additional -D flags for movemail (add to MOVE_FLAGS if desired):
# MAIL_USE_POP		Support mail retrieval from a POP mailbox.
# MAIL_USE_MMDF		Support MMDF mailboxes.
# MAIL_USE_FLOCK	Use flock for file locking (see the comments
#			about locking in movemail.c)
# MAIL_UNLINK_SPOOL	Unlink the user's spool mailbox after reading
#			it (instead of just emptying it).
# KERBEROS		Support Kerberized POP.
# KRB5			Support Kerberos Version 5 pop instead of
#			Version 4 (define this in addition to
#			KERBEROS).
# HESIOD		Support Hesiod lookups of user mailboxes.
# MAILHOST		A string, the host name of the default POP
#			mail host for the site.
MOVE_FLAGS=

# ========================== start of cpp stuff =======================
/* From here on, comments must be done in C syntax.  */

#define THIS_IS_MAKEFILE
#define NOT_C_CODE
#include "../src/config.h"

/* Some s/SYSTEM.h files define this to request special libraries.  */
#ifndef LIBS_SYSTEM
#define LIBS_SYSTEM
#endif

/* Some m/MACHINE.h files define this to request special libraries.  */
#ifndef LIBS_MACHINE
#define LIBS_MACHINE
#endif

#ifndef C_SWITCH_SYSTEM
#define C_SWITCH_SYSTEM
#endif

#ifndef C_SWITCH_MACHINE
#define C_SWITCH_MACHINE
#endif

#undef MOVEMAIL_NEEDS_BLESSING
#ifndef MAIL_USE_FLOCK
#ifndef MAIL_USE_LOCKF
#define MOVEMAIL_NEEDS_BLESSING
#endif
#endif

#ifdef MOVEMAIL_NEEDS_BLESSING
#define BLESSMAIL blessmail
#else
#define BLESSMAIL
#endif

#ifdef KERBEROS
# ifdef HAVE_LIBKRB
    KRB4LIB = -lkrb
# else
#  ifdef HAVE_LIBKRB4
     KRB4LIB = -lkrb4
#  endif
# endif
# ifdef HAVE_LIBDES
    DESLIB = -ldes
# else
#  ifdef HAVE_LIBDES425
    DESLIB = -ldes425
#  endif
# endif
# ifdef HAVE_LIBKRB5
    KRB5LIB = -lkrb5
# endif
# ifdef HAVE_LIBK5CRYPTO
    CRYPTOLIB = -lk5crypto
# else
#  ifdef HAVE_LIBCRYPTO
    CRYPTOLIB = -lcrypto
#  endif
# endif
# ifdef HAVE_LIBCOM_ERR
    COM_ERRLIB = -lcom_err
# endif
#endif /* KERBEROS */

/* If HESIOD is defined, set this to "-lhesiod". */
#ifdef HAVE_LIBHESIOD
# ifdef HAVE_LIBRESOLV
    HESIODLIB= -lhesiod -lresolv
# else
    HESIODLIB= -lhesiod
# endif
#endif

LIBS_MOVE=$(KRB4LIB) $(DESLIB) $(KRB5LIB) $(CRYPTOLIB) $(COM_ERRLIB) $(HESIODLIB)

#ifdef HAVE_LIBLOCKFILE
LIBS_MAIL=-llockfile
#else
#ifdef HAVE_LIBMAIL
LIBS_MAIL=-lmail
#endif
#endif

LOADLIBES=LIBS_SYSTEM LIBS_MACHINE

/* We need to #define emacs to get the right versions of some files.
   Some other files - those shared with other GNU utilities - need
   HAVE_CONFIG_H #defined before they know they can take advantage of
   the information in ../src/config.h.  */
ALL_CFLAGS = C_SWITCH_SYSTEM C_SWITCH_MACHINE -DHAVE_CONFIG_H \
   -I. -I../src -I${srcdir} -I${srcdir}/../src ${LDFLAGS} ${CPPFLAGS} ${CFLAGS}
LINK_CFLAGS = C_SWITCH_SYSTEM C_SWITCH_MACHINE -DHAVE_CONFIG_H \
   -I. -I../src -I${srcdir} -I${srcdir}/../src ${LDFLAGS} ${CFLAGS}
CPP_CFLAGS = C_SWITCH_SYSTEM C_SWITCH_MACHINE -DHAVE_CONFIG_H \
   -I. -I../src -I${srcdir} -I${srcdir}/../src ${CPPFLAGS} ${CFLAGS}
/* This was all of CPP_CFLAGS except -Demacs.
   Now that -Demacs has been deleted from CPP_CFLAGS,
   this is actually the same as CPP_CFLAGS, but let\'s not delete it yet.  */
BASE_CFLAGS = C_SWITCH_SYSTEM C_SWITCH_MACHINE -DHAVE_CONFIG_H \
   -I. -I../src -I${srcdir} -I${srcdir}/../src ${CPPFLAGS} ${CFLAGS}

.SUFFIXES: .m

/* This is the default compilation command.
   But we should never rely on it, because some make version
   failed to find it for getopt.o.
   Using an explicit command made it work.  */
.c.o:
	${CC} -c ${CPP_CFLAGS} $<

.m.o:
	$(CC) -c $(CPPFLAGS) $(ALL_CFLAGS) $<

all: ${DONT_INSTALL} ${UTILITIES} ${INSTALLABLES} ${SCRIPTS} ${INSTALLABLE_SCRIPTS}

/* These targets copy the scripts into the build directory
so that they can be run from there in an uninstalled Emacs.
The "-" is prepended because some versions of cp barf when
srcdir is the current directory, and thus the file will be
copied into itself.  */
rcs2log: $(srcdir)/rcs2log
	-cp -p $(srcdir)/rcs2log rcs2log

rcs-checkin: $(srcdir)/rcs-checkin
	-cp -p $(srcdir)/rcs-checkin rcs-checkin

grep-changelog: $(srcdir)/grep-changelog
	-cp -p $(srcdir)/grep-changelog grep-changelog

vcdiff: $(srcdir)/vcdiff
	-cp -p $(srcdir)/vcdiff vcdiff

#ifdef MOVEMAIL_NEEDS_BLESSING
blessmail:
	$(EMACS) $(EMACSOPT) -l $(srcdir)/../lisp/mail/blessmail.el
	chmod +x blessmail
#endif

maybe-blessmail: BLESSMAIL
#ifdef MOVEMAIL_NEEDS_BLESSING
/* Don\'t charge ahead and do it!  Let the installer decide.
	  ./blessmail $(DESTDIR)${archlibdir}/movemail${EXEEXT}  */
	@if [ `wc -l <blessmail` != 2 ] ; then \
	  dir=`sed -n -e 's/echo mail directory = \(.*\)/\1/p' blessmail`; \
	  echo Assuming $$dir is really the mail spool directory, you should; \
	  echo run  lib-src/blessmail $(DESTDIR)${archlibdir}/movemail${EXEEXT}; \
	  echo as root, to give  movemail${EXEEXT}  appropriate permissions.; \
	  echo Do that after running  make install.; \
	fi
#endif

/* Install the internal utilities.  Until they are installed, we can
   just run them directly from lib-src.  */
$(DESTDIR)${archlibdir}: all
	@echo
	@echo "Installing utilities run internally by Emacs."
	umask 022; $(top_srcdir)/mkinstalldirs $(DESTDIR)${archlibdir}
	if [ `(cd $(DESTDIR)${archlibdir} && /bin/pwd)` != `/bin/pwd` ]; then \
	  for file in ${UTILITIES}; do \
	    $(INSTALL_PROGRAM) $(INSTALL_STRIP) $$file $(DESTDIR)${archlibdir}/$$file ; \
	  done ; \
        fi
	umask 022; $(top_srcdir)/mkinstalldirs $(DESTDIR)${gamedir}; \
	touch $(DESTDIR)${gamedir}/snake-scores; \
	touch $(DESTDIR)${gamedir}/tetris-scores
/* If the following commands fail, that is not a big deal.
   update-game-score will detect at runtime that it is not setuid,
   and handle things accordingly. */
	-if chown ${gameuser} $(DESTDIR)${archlibdir}/update-game-score && chmod u+s $(DESTDIR)${archlibdir}/update-game-score; then \
	  chown ${gameuser} $(DESTDIR)${gamedir}; \
	  chmod u=rwx,g=rwx,o=rx $(DESTDIR)${gamedir}; \
	fi
        if [ `(cd $(DESTDIR)${archlibdir} && /bin/pwd)` \
             != `(cd ${srcdir} && /bin/pwd)` ]; then \
	  for file in ${SCRIPTS}; do \
	    $(INSTALL_SCRIPT) ${srcdir}/$$file $(DESTDIR)${archlibdir}/$$file; \
	  done ; \
	fi

install: $(DESTDIR)${archlibdir}
	@echo
	@echo "Installing utilities for users to run."
	for file in ${INSTALLABLES} ; do \
	  $(INSTALL_PROGRAM) $(INSTALL_STRIP) $${file} $(DESTDIR)${bindir}/`echo $${file} | sed '$(TRANSFORM)'` ; \
	  chmod a+rx $(DESTDIR)${bindir}/`echo $${file} | sed '$(TRANSFORM)'`; \
	done
	for file in ${INSTALLABLE_SCRIPTS} ; do \
	  $(INSTALL_SCRIPT) ${srcdir}/$${file} $(DESTDIR)${bindir}/`echo $${file} | sed '$(TRANSFORM)'`  ; \
	  chmod a+rx $(DESTDIR)${bindir}/`echo $${file} | sed '$(TRANSFORM)'`; \
	done

uninstall:
	(cd $(DESTDIR)${bindir}; \
	for file in ${INSTALLABLES} ${INSTALLABLE_SCRIPTS}; do \
	  rm -f $(DESTDIR)${bindir}/`echo $${file} | sed '$(TRANSFORM)'` ; \
	done)
	(cd $(DESTDIR)${archlibdir} && \
	 rm -f ${UTILITIES} ${INSTALLABLES} ${SCRIPTS} ${INSTALLABLE_SCRIPTS})

mostlyclean:
	-rm -f core *.o getopt.h getopt.h-t

clean: mostlyclean
	-rm -f ${INSTALLABLES} ${UTILITIES} ${DONT_INSTALL}
	-rm -f fns*.el *.tab.c *.tab.h

distclean: clean
	-rm -f TAGS
	-rm -f Makefile Makefile.c blessmail

maintainer-clean: distclean
	true

extraclean: maintainer-clean
	-rm -f *~ \#*

/* Test the contents of the directory.  */
check:
	@echo "We don't have any tests for GNU Emacs yet."

tags: TAGS
TAGS: etags${EXEEXT}
	etags *.[ch]

/* This verifies that the non-ASCII characters in the file \`testfile\'
   have not been clobbered by whatever means were used to copy and
   distribute Emacs.  If they were clobbered, all the .elc files were
   clobbered too.  */
test-distrib${EXEEXT}: ${srcdir}/test-distrib.c
	$(CC) ${ALL_CFLAGS} -o test-distrib ${srcdir}/test-distrib.c
	./test-distrib ${srcdir}/testfile

/* We need the following in order to create a <getopt.h> when the system
   does not have one that works with the given compiler.  */
GETOPT_H = getopt.h
getopt.h: getopt_.h
	cp $(srcdir)/getopt_.h $@-t
	mv $@-t $@

GETOPTOBJS = getopt.o getopt1.o
GETOPTDEPS = $(GETOPTOBJS) $(GETOPT_H)
getopt.o: ${srcdir}/getopt.c $(GETOPT_H) ${srcdir}/gettext.h
	${CC} -c ${CPP_CFLAGS} ${srcdir}/getopt.c
getopt1.o: ${srcdir}/getopt1.c $(GETOPT_H)
	${CC} -c ${CPP_CFLAGS} ${srcdir}/getopt1.c

REGEXPOBJ = regex.o
REGEXPDEPS = $(REGEXPOBJ) $(srcdir)/../src/regex.h

regex.o: $(srcdir)/../src/regex.c $(srcdir)/../src/regex.h ../src/config.h
	${CC} -c ${BASE_CFLAGS} -DCONFIG_BROKETS -DINHIBIT_STRING_HEADER ${srcdir}/../src/regex.c

etags${EXEEXT}: ${srcdir}/etags.c $(GETOPTDEPS) $(REGEXPDEPS) ../src/config.h
	$(CC) ${ALL_CFLAGS} -DEMACS_NAME="\"GNU Emacs\"" -DVERSION="\"${version}\"" ${srcdir}/etags.c $(GETOPTOBJS) $(REGEXPOBJ) $(LOADLIBES) -o etags

ebrowse${EXEEXT}: ${srcdir}/ebrowse.c $(GETOPTDEPS) ../src/config.h
	$(CC) ${ALL_CFLAGS} -DVERSION="\"${version}\"" ${srcdir}/ebrowse.c $(GETOPTOBJS) $(LOADLIBES) -o ebrowse

/* We depend on etags to assure that parallel makes don\'t write two
   etags.o files on top of each other.  */
ctags${EXEEXT}: etags${EXEEXT}
	$(CC) ${ALL_CFLAGS} -DCTAGS -DEMACS_NAME="\"GNU Emacs\"" -DVERSION="\"${version}\"" ${srcdir}/etags.c $(GETOPTOBJS) $(REGEXPOBJ) $(LOADLIBES) -o ctags

profile${EXEEXT}: ${srcdir}/profile.c ../src/config.h
	$(CC) ${ALL_CFLAGS} ${srcdir}/profile.c $(LOADLIBES) -o profile

make-docfile${EXEEXT}: ${srcdir}/make-docfile.c ../src/config.h
	$(CC) ${ALL_CFLAGS} ${srcdir}/make-docfile.c $(LOADLIBES) -o make-docfile

digest-doc${EXEEXT}: ${srcdir}/digest-doc.c
	$(CC) ${ALL_CFLAGS} ${srcdir}/digest-doc.c $(LOADLIBES) -o digest-doc

sorted-doc${EXEEXT}: ${srcdir}/sorted-doc.c
	$(CC) ${ALL_CFLAGS} ${srcdir}/sorted-doc.c $(LOADLIBES) -o sorted-doc

b2m${EXEEXT}: ${srcdir}/b2m.c ../src/config.h $(GETOPTDEPS)
	$(CC) ${ALL_CFLAGS} ${srcdir}/b2m.c  -DVERSION="\"${version}\"" \
	   $(GETOPTOBJS) $(LOADLIBES) -o b2m

movemail${EXEEXT}: movemail.o pop.o $(GETOPTDEPS)
	$(CC) ${LINK_CFLAGS} ${MOVE_FLAGS} movemail.o pop.o $(GETOPTOBJS) $(LOADLIBES) $(LIBS_MAIL) $(LIBS_MOVE) -o movemail

movemail.o: ${srcdir}/movemail.c ../src/config.h $(GETOPT_H)
	$(CC) -c ${CPP_CFLAGS} ${MOVE_FLAGS} ${srcdir}/movemail.c

pop.o: ${srcdir}/pop.c  ../src/config.h
	$(CC) -c ${CPP_CFLAGS} ${MOVE_FLAGS} ${srcdir}/pop.c

fakemail${EXEEXT}: ${srcdir}/fakemail.c ../src/config.h
	$(CC) ${ALL_CFLAGS} ${srcdir}/fakemail.c $(LOADLIBES) -o fakemail

emacsclient${EXEEXT}: ${srcdir}/emacsclient.c ../src/config.h $(GETOPTDEPS)
	$(CC) ${ALL_CFLAGS} ${srcdir}/emacsclient.c $(GETOPTOBJS)  \
	   -DVERSION="\"${version}\"" \
	   $(LOADLIBES) -o emacsclient

hexl${EXEEXT}: ${srcdir}/hexl.c ../src/config.h
	$(CC) ${ALL_CFLAGS} ${srcdir}/hexl.c $(LOADLIBES) -o hexl

update-game-score${EXEEXT}: update-game-score.o $(GETOPTDEPS)
	$(CC) ${LINK_CFLAGS} ${MOVE_FLAGS} update-game-score.o $(GETOPTOBJS) $(LOADLIBES) -o update-game-score

update-game-score.o: ${srcdir}/update-game-score.c ../src/config.h $(GETOPT_H)
	$(CC) -c ${CPP_CFLAGS} ${srcdir}/update-game-score.c \
	  -DHAVE_SHARED_GAME_DIR="\"$(gamedir)\""
