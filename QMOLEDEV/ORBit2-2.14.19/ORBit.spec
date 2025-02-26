%define __libtoolize /bin/true
%define __spec_install_post /usr/lib/rpm/brp-compress
Summary:          A high-performance CORBA Object Request Broker.
Name:             ORBit2
Version:          2.14.19
Release:          1
Source:           %{name}-%{version}.tar.gz
Group:            System Environment/Daemons
License:          LGPL/GPL
BuildRoot:        %{_tmppath}/%{name}-%{version}-root
Prereq:           /sbin/install-info
URL:              http://www.labs.redhat.com/orbit/
BuildRequires:    pkgconfig >= 0.8
Requires:         glib2 >= 2.8.0
Requires:         libIDL >= 0.8.2
BuildRequires:    glib2-devel >= 2.8.0
BuildRequires:    libIDL-devel >= 0.8.2


%description
ORBit is a high-performance CORBA (Common Object Request Broker
Architecture) ORB (object request broker). It allows programs to
send requests and receive replies from other programs, regardless
of the locations of the two programs. CORBA is an architecture that
enables communication between program objects, regardless of the
programming language they're written in or the operating system they
run on.

You will need to install this package and ORBit-devel if you want to
write programs that use CORBA technology.

%package devel
Summary:          Development libraries, header files and utilities for ORBit.
Group:            Development/Libraries
Requires:         %name = %{version}
Requires:         indent
Requires:         pkgconfig >= 0.8
Requires:         glib2 >= 2.8.0
Requires:         glib2-devel >= 2.8.0
Requires:         libIDL >= 0.8.2
Requires:         libIDL-devel >= 0.8.2


%description devel
ORBit is a high-performance CORBA (Common Object Request Broker
Architecture) ORB (object request broker) with support for the
C language.

This package contains the header files, libraries and utilities
necessary to write programs that use CORBA technology. If you want to
write such programs, you'll also need to install the ORBIT package.

%prep
%setup -q
mv ltmain.sh ltmain.sh.orig
sed s/relink=yes/relink=no/ ltmain.sh.orig > ltmain.sh
%build
%configure

make 
#%{?_smp_mflags}

%install
rm -rf %{buildroot}
%makeinstall

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%post devel

%preun devel

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog NEWS README TODO
%{_libdir}/*.so.*
%dir %{_libdir}/orbit-2.0/
%{_bindir}/linc-cleanup-sockets

%files devel
%defattr(-,root,root)
%doc /usr/share/gtk-doc/html/ORBit2/
%{_bindir}/orbit-idl-2
%{_bindir}/typelib-dump
%{_bindir}/orbit2-config
%{_bindir}/ior-decode-2
%{_libdir}/*a
%{_libdir}/*.so
%{_libdir}/orbit-2.0/*.so
%{_libdir}/orbit-2.0/*a
%{_libdir}/pkgconfig/*
%{_includedir}/*
%{_datadir}/aclocal/*
%{_datadir}/idl/orbit-2.0/*

%changelog
* Tue Mar 05 2002 Chris Chabot <chabotc@reviewboard.com>
- Cleaned up formatting
- made into .spec.in
- expanded deps

* Fri Feb 01 2002 Roy-Magne Mo <rmo@sunnmore.net>
- Added deps

* Tue Jan 22 2002 Gregory Leblanc <gleblanc@linuxweasel.com>
- removed Conflicts line
- cleaned up %files section
- removed duplicate glib-devel requires line

* Sat Jan 19 2002 Chris Chabot <chabotc@reviewboard.com>
- Imported spec into gnome 2.0 alpha
- Bumped version to 2.3.102
- Changed requires accordingly
- Minor cleanups

* Wed Jan 09 2002 Tim Powers <timp@redhat.com>
- automated rebuild

* Wed Jan  2 2002 Havoc Pennington <hp@redhat.com>
- build system somehow built against libglib-1.3.so.11
even though pkg-config found 1.3.12? wtf?
trying again

* Wed Jan  2 2002 Havoc Pennington <hp@redhat.com>
- 2.3.100.90 snap

* Mon Nov 26 2001 Havoc Pennington <hp@redhat.com>
- 2.3.99

* Sun Nov 25 2001 Havoc Pennington <hp@redhat.com>
- new snap 2.3.97.90, rebuild for glib 1.3.11

* Fri Oct 26 2001 Havoc Pennington <hp@redhat.com>
- new snap, glib 1.3.10 rebuild

* Tue Oct  9 2001 Havoc Pennington <hp@redhat.com>
- check rebuild against new linc with headers moved
- remove epoch, that was a screwup

* Thu Oct  4 2001 Havoc Pennington <hp@redhat.com>
- cvs snap
- require specific glib2

* Thu Sep 27 2001 Havoc Pennington <hp@redhat.com>
- 2.3.95 tarball
- depend on new standalone libIDL, remove all libIDL stuff from file list

* Fri Sep 21 2001 Havoc Pennington <hp@redhat.com>
- require specific linc version, unrequire specific glib version since
we get that via linc

* Mon Sep 17 2001 Havoc Pennington <hp@redhat.com>
- newer orbit2 from CVS

* Thu Sep 13 2001 Havoc Pennington <hp@redhat.com>
- conflict with old orbit with headers not moved

* Wed Sep 12 2001 Havoc Pennington <hp@redhat.com>
- renaming more things
- remove smp flags, doesn't work atm
- fix .pc file, trying to get bonobo-activation to build

* Tue Sep 11 2001 Havoc Pennington <hp@redhat.com>
- kill all file conflicts with ORBit1

* Mon Sep 10 2001 Havoc Pennington <hp@redhat.com>
- convert to ORBit2 spec file (from ORBit original)

* Tue Jul 24 2001 Alexander Larsson <alexl@redhat.com>
- Added glib-devel BuildRequires

* Thu Jun 21 2001 Elliot Lee <sopwith@redhat.com> 0.5.8-2
- Use _smp_mflags if possible.

* Mon May 14 2001 Jonathan Blandford <jrb@redhat.com>
- bumped version

* Thu Mar 01 2001 Owen Taylor <otaylor@redhat.com>
- Rebuild for GTK+-1.2.9

* Tue Jan 30 2001 Elliot Lee <sopwith@redhat.com>
- 0.5.7 for real

* Thu Jan 18 2001 Elliot Lee <sopwith@redhat.com>
- 0.5.7

* Tue Dec 19 2000 Elliot Lee <sopwith@redhat.com>
- 0.5.6

* Thu Nov 30 2000 Elliot Lee <sopwith@redhat.com>
- 0.5.5

* Fri Aug 11 2000 Jonathan Blandford <jrb@redhat.com>
- Up Epoch and release

* Tue Jul 25 2000 Elliot Lee <sopwith@redhat.com>
- 0.5.3

* Wed Jul 19 2000 Jonathan Blandford <jrb@redhat.com>
- fixed to work with new cpp.

* Wed Jul 12 2000 Prospector <bugzilla@redhat.com>
- automatic rebuild

* Tue Jul 11 2000 Jonathan Blandford <jrb@redhat.com>
- Upgraded to 0.5.2 - a bug fix release.

* Tue Jun 27 2000 Havoc Pennington <hp@redhat.com>
- Trying to build the package for Preston and the build
system wants a new changelog entry

* Sat Jun 24 2000 Preston Brown <pbrown@redhat.com>
- defattr the devel pkg
- FHS paths

* Fri May 19 2000 Jonathan Blandford <jrb@redhat.com>
- Upgraded to 0.5.1

* Tue Feb  3 2000 Elliot Lee <sopwith@redhat.com> 0.5.0-3
- Strip shared libraries

* Mon Aug 30 1999 Elliot Lee <sopwith@redhat.com> 0.4.94-1
- Spec file fixes from RHL 6.0.

* Wed Jun 2 1999  Jose Mercado <jmercado@mit.edu>
- Fixed configure.in so spec.in could be used.

* Mon Nov 23 1998 Pablo Saratxaga <srtxg@chanae.alphanet.ch>

- improved %files section, and added use of %{prefix} and install-info
(well,... no. The info file has not dir info inside, commented out)
