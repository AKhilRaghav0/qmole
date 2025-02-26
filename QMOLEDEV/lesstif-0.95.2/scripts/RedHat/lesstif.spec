Summary:	lesstif
Name:		lesstif
Version:	0.95.2
Release:	1
License:	LGPL
Packager:	LessTif Releasemeister <lesstif-discuss@lists.sourceforge.net>
Group:		System Environment/Libraries
Prefixes:	/usr/X11R6 /usr/doc
Source:		ftp://ftp.hungry.com/pub/hungry/lesstif/lesstif-0.95.2.tar.gz
BuildRoot:	/tmp/lesstif-0.95.2-root

%description
Lesstif is an API compatible clone of the Motif toolkit.
Most of the Motif API is in place,
we're working on improving it.
Many Motif applications compile and run out-of-the-box with LessTif,
and we want to hear about those that don't.

%package mwm
Summary: Lesstif Motif window manager clone based on fvwm
Group: User Interface/Desktops
Prefixes:  /usr/X11R6 /usr/doc /etc
%description mwm
MWM is a window manager that adheres largely to the Motif mwm specification.

%package clients
Summary: lesstif clients
Group: Development/Tools
Prefixes:  /usr/X11R6 /usr/doc
%description clients
Uil and xmbind.

%package devel
Group: Development/Libraries
Prefixes:  /usr/X11R6 /usr/doc
Summary: header files for Lesstif/Motif development
Requires: lesstif
%description devel
This package contains the lesstif header files
required to develop motif-based applications.
Package also contains development documentation in html (Lessdox), and
mxmkmf for Lesstif.

# %package static
# Group: Development/Libraries
# Prefixes:  /usr/X11R6
# Summary: static libraries for Lesstif/Motif development
# Requires: lesstif
# %description static
# This package contains the lesstif static libraries
# required to develop motif-based applications.

%prep
%setup -n lesstif-0.95.2

LESSTIFTOP=$PWD

CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr/X11R6 --enable-production

%build
make -j4

%install
# chmod a-x [A-Z]*
make install DESTDIR=$RPM_BUILD_ROOT

strip $RPM_BUILD_ROOT/usr/X11R6/bin/mwm
strip $RPM_BUILD_ROOT/usr/X11R6/bin/uil
strip $RPM_BUILD_ROOT/usr/X11R6/bin/xmbind
strip $RPM_BUILD_ROOT/usr/X11R6/lib/libXm.so*
strip $RPM_BUILD_ROOT/usr/X11R6/lib/libMrm.so*
strip $RPM_BUILD_ROOT/usr/X11R6/lib/libDtPrint.so*

install -d $RPM_BUILD_ROOT/etc/X11
ln -sf ../../usr/X11R6/lib/X11/mwm $RPM_BUILD_ROOT/etc/X11/mwm

install -d $RPM_BUILD_ROOT/usr/X11R6/man
install -d $RPM_BUILD_ROOT/usr/X11R6/man/man1
install -d $RPM_BUILD_ROOT/usr/X11R6/man/man3
install -d $RPM_BUILD_ROOT/usr/X11R6/man/man5
# install -c -m 644 doc/lessdox/clients/mwmrc.5   $RPM_BUILD_ROOT/usr/X11R6/man/man5
# install -c -m 644 doc/lessdox/clients/mwm.1     $RPM_BUILD_ROOT/usr/X11R6/man/man1
# install -c -m 644 doc/lessdox/clients/lesstif.1 $RPM_BUILD_ROOT/usr/X11R6/man/man1
# install -c -m 644 doc/lessdox/clients/xmbind.1  $RPM_BUILD_ROOT/usr/X11R6/man/man1
install -c -m 644 doc/lessdox/*/*.html $RPM_BUILD_ROOT/usr/X11R6/LessTif/doc/Lessdox || :

# generate config files 
# cd $RPM_BUILD_ROOT/usr/X11R6/lib/X11/config;
# 
# mv Motif.rules Motif-lesstif.rules
# mv Motif.tmpl  Motif-lesstif.tmpl

# cleanup in a preparation for an installation - unify layout

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%clean
#do this manually; it's a better idea
#rm -rf $RPM_BUILD_ROOT

%files
#%attr(755, root, root) %doc
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/AUTHORS
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/BUG-REPORTING
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/COPYING
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/COPYING.LIB
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/CREDITS
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/FAQ
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/INSTALL.txt
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/README
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/ReleaseNotes.txt
%multiarch %attr(644, root, root) /usr/X11R6/LessTif/ReleaseNotes.html
%attr(755, root, root) %dir /usr/X11R6/lib
%attr(755, root, root) /usr/X11R6/lib/libXm.so*
%attr(755, root, root) /usr/X11R6/lib/libXm.la
%attr(755, root, root) /usr/X11R6/lib/libMrm.so*
%attr(755, root, root) /usr/X11R6/lib/libMrm.la
%attr(755, root, root) /usr/X11R6/lib/libUil.so*
%attr(755, root, root) /usr/X11R6/lib/libUil.la
%attr(755, root, root) /usr/X11R6/lib/libDtPrint.so*
%attr(755, root, root) /usr/X11R6/lib/libDtPrint.la
%multiarch %attr(755, root, root) %dir /usr/X11R6/LessTif/doc
%multiarch %attr(-, root, root) /usr/X11R6/LessTif/doc/*
%multiarch %attr(644, root, root) /usr/X11R6/man/man1/lesstif.1*
%multiarch %attr(644, root, root) /usr/X11R6/man/man1/ltversion.1*
%multiarch %attr(644, root, root) /usr/X11R6/man/man5/VirtualBindings.5*

%files mwm
%multiarch %attr(644, root, root) %doc clients/Motif-2.1/mwm/COPYING
%multiarch %attr(644, root, root) %doc clients/Motif-2.1/mwm/README
%multiarch %attr(755, root, root) %dir /etc/X11/mwm
%multiarch %attr(755, root, root) %dir /usr/X11R6/lib/X11/mwm
%multiarch %attr(755, root, root) /usr/X11R6/bin/mwm
%multiarch %attr(644, root, root) %config /usr/X11R6/lib/X11/mwm/*
%multiarch %attr(644, root, root) %config /usr/X11R6/lib/X11/app-defaults/Mwm
%multiarch %attr(644, root, root) /usr/X11R6/man/man1/mwm.1*
%multiarch %attr(644, root, root) /usr/X11R6/man/man5/mwmrc.5*

%files clients
%multiarch %attr(644, root, root) %doc doc/UIL.txt
%attr(755, root, root) /usr/X11R6/bin/uil
%attr(755, root, root) /usr/X11R6/bin/xmbind
%multiarch %attr(644, root, root) /usr/X11R6/man/man1/xmbind.1*
%multiarch %attr(644, root, root) /usr/X11R6/man/man1/uil.1*

%files devel
# %doc doc/lessdox/*
%attr(755, root, root) %dir /usr/X11R6/include/Mrm
%attr(755, root, root) %dir /usr/X11R6/include/Xm
%attr(755, root, root) %dir /usr/X11R6/include/Dt
%attr(755, root, root) %dir /usr/X11R6/include/uil
%attr(644, root, root) /usr/X11R6/include/Mrm/*
%attr(644, root, root) /usr/X11R6/include/Xm/*
%attr(644, root, root) /usr/X11R6/include/Dt/*
%attr(644, root, root) /usr/X11R6/include/uil/*
%multiarch %attr(755, root, root) /usr/X11R6/bin/mxmkmf
%multiarch %attr(755, root, root) /usr/X11R6/bin/motif-config
%multiarch %attr(644, root, root) /usr/X11R6/man/man3/*.3*
%multiarch %attr(644, root, root) /usr/local/share/aclocal/ac_find_motif.m4
%multiarch %attr(644, root, root) /usr/X11R6/lib/LessTif/config/LessTif.rules
%multiarch %attr(644, root, root) /usr/X11R6/lib/LessTif/config/LessTif.tmpl
%multiarch %attr(644, root, root) /usr/X11R6/lib/LessTif/config/host.def

%changelog
* Fri May  6 2005 Danny Backx <dannybackx@users.sourceforge.net>
- Flag the motif-config script as %multiarch.

* Sun Jan 16 2005 Danny Backx <dannybackx@users.sourceforge.net>
- Add the motif-config script.

* Sun Oct 17 2004 Danny Backx <dannybackx@users.sourceforge.net>
- Get installation directory for AC macro file from the build.

* Tue Sep 28 2004 Danny Backx <dannybackx@users.sourceforge.net>
- Configure should be launched with fewer parameters to approximate
  its default behaviour.

* Sat Aug 28 2004 Danny Backx <dannybackx@users.sourceforge.net>
- Make it all 2.1.

* Fri Aug 6 2004 Danny Backx <dannybackx@users.sourceforge.net>
- Remove the references to 1.2 in this file.
- Fixes for changes with the auto tools.
- Packager is the new name of the mailing list.
- Add lots of files that were not previously correctly packages.

* Wed Sep 19 2001 Danny Backx <danny.backx@skynet.be>
- Adapt to new directory structure without the MotifX.Y in it.

* Tue Aug 28 2001 Danny Backx <danny.backx@skynet.be>
- Changes w.r.t. renamed and removed documentation files.

* Wed Jul 18 2001 Danny Backx <danny.backx@skynet.be>
- Remove Xbae, Xlt

* Fri Mar 30 2001 Danny Backx <danny.backx@skynet.be>
- Rename Dt library to DtPrint
- Fix the toplevel documentation ownership.
  This is a solution for bug 410578.
- Comment out the static RPM definition.
  Static libs are not something we should promote, they're also *big*.
- Use "make -j2" to speed up compilation a bit if you have enough RAM.
- Add --disable-debug flag to avoid compile with -g, we strip the
  libraries anyway.
- Remove the %doc doc/lessdox/* line because it causes unnecessary
  file inclusion, and a problem during the rpm build process.
- Add %attr to all %doc lines.

* Thu Dec 21 2000 John Gotts <jgotts@linuxsavvy.com>
- Wildcard on manual pages because some versions of RPM compress them.
- Include the 2.* clients (uil, mwm, xmbind).

* Sat Dec  9 2000 Danny Backx <danny.backx@skynet.be>
- Add Dt and uil, both include files and libraries.

* Tue Nov 14 2000 Danny Backx <danny.backx@skynet.be>
- Replace 2.0 version by 2.1.

* Mon Oct 23 2000 John Gotts <jgotts@linuxsavvy.com>
- Removed %attr for %doc because the documentation directories were getting
  created without the execute bit set.
- Synched the current state of the documentation.
- Fixed the categories to be consistent with Red Hat Linux.

* Sat Oct  7 2000 Danny Backx <danny.backx@skynet.be>
- Remove Imake.tmpl handling, it was removed from lib/config a while ago.

* Sun Sep 10 2000 Danny Backx <danny.backx@skynet.be>
- Change the default so 2.0 library and include files get used, but
  1.2 is also still built and installed.

* Tue Jul 11 2000 Danny Backx <danny.backx@skynet.be>
- BuildRoot was wrong, changed it so it now works with what autoconf
  provides (PACKAGE and VERSION macros).

* Wed Jun 14 2000 Danny Backx <danny.backx@skynet.be>
- Make sure that links for libXm, libMrm are in /usr/X11R6/lib too,
  not only in /usr/X11R6/LessTif/Motif2.0/lib .

* Tue Apr 25 2000 Danny Backx <danny.backx@skynet.be>
- add CFLAGS="$RPM_OPT_FLAGS" in front of ./configure to be able to
  pass parameters when building.
  Fix for linuxppc by howarth@bromo.med.uc.edu (Jack Howarth).

* Sat Apr 8 2000 Danny Backx <danny.backx@skynet.be>
- Add the "static" file

* Wed Apr 5 2000 Danny Backx <danny.backx@skynet.be>
- Make sure to include both directories and files for Xlt, Xbae in "devel"

* Wed Feb 23 2000 Jon Christopher <jon@hungry.com>
- Don't use brace expansion because not all distributions have a full
- featured bash as /bin/sh

* Wed Jan 19 2000 Danny Backx <danny@hungry.com>
- Remove static libs from the regular file, they should be in "devel" only.
- Change path for DESTDIR as indicated by Pavel Roskin.

* Fri Dec 10 1999 Danny Backx <danny@hungry.com>
- Changed "make install prefix=..." to use DESTDIR. Prefix is not properly
  forwarded in the directory tree, DESTDIR is.

* Wed Dec 8 1999 Danny Backx <danny@hungry.com>
- be sure to include Xbae and Xlt in packaging (%files sections)
- include 2.0 libraries and includes in the standard rpm

* Sat Oct 23 1999 Danny Backx <danny@hungry.com>
- try to integrate Michal's stuff in lesstif.spec.in
- don't treat Xbae, Xlt separately
- compile for 2.0 as well as for 1.2
- compile Xbae, Xlt by default

* Tue Oct 19 1999 Michal Jaegermann <michal@harddata.com>              (0.89)
- Updated to version 0.89
- Reorganized spec file to follow closer its own descriptions
- Xlt does not require special make anymore.

* Thu Apr 30 1998 C. Scott Ananian <cananian@alumni.princeton.edu>      (0.83+)
- Updated to lessdoc-current.
- Removes Lessdox package (integrated into lesstif)

* Tue Mar 31 1998 C. Scott Ananian <cananian@alumni.princeton.edu>      (0.83+)
- Removed pedantic.patch
- Removed lesstif-M12 (Motif 1.2 wrapper)
- Reviewed installation and fixed %files sections.
- Added patch to fix a bug which causes mozilla to crash.
- Added patch to fix the include prefix on install.

* Sun Jul 20 1997 Tomasz K�oczko <kloczek@rudy.mif.pg.gda.pl>        (0.80-2)
- added to all %doc %attr macros (this allow build package from normal user
  account),
- some simplification in %files (%doc).

* Wed Jul 9 1997 Tomasz K�oczko <kloczek@rudy.mif.pg.gda.pl>
- added using %%{PACKAGE_VERSION} macro in "Source:" and %files,
- added additional parameter "--enable-build-12" to runing configure,
- added %posun and %clear,
- in %post and %postun ldconfig is called as parameter with "-p"
  (this feature is avalable in rpm >= 2.4.3 and you must have this
  version and if you want recompile package from src.rpm you must have new
  version rpm),
- added package lesstif-M12 simpe Motif 1.2 wrapper,
- simplified %_install section,
- added %attr macros in %files sections,
- added striping shared libraries,
- added URL field,
- added Lessdox - a html development documentation to lesstif-devel,
- added lesstif-0.80public-nopedantic.patch, this allow compile lesstif on
  sparc by removing "-pedantic" from CFLAGS.




