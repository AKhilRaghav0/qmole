%define __libtoolize /bin/true
%define __spec_install_post /usr/lib/rpm/brp-compress
Summary:          Library for writing network programs
Name:             linc
Version:          0.7.1
Release:          1
Source:           ftp://ftp.gnome.org/pub/GNOME/pre-gnome2/sources/linc/%{name}-%{version}.tar.gz
Group:            System Environment/Libraries
License:          LGPL
BuildRoot:        %{_tmppath}/%{name}-%{version}-root
BuildRequires:    pkgconfig >= 0.8
Requires:         glib2 >= 1.3.11
BuildRequires:    glib2-devel >= 1.3.11


%description
linc is a library that eases the task of writing networked servers and
clients. It takes care of connection initiation and maintainance, and
the details of varioustransports. It is used by the new ORBit to
handle message transmission/receipt.

%package devel
Summary:          Development libraries and header files for linc.
Group:            Development/Libraries
Requires:         %name = %{version}
Requires:         pkgconfig >= 0.8
Requires:         glib2 >= 1.3.11
Requires:         glib2-devel >= 1.3.11


%description devel
linc is a library that eases the task of writing networked servers and
clients. It takes care of connection initiation and maintainance, and
the details of varioustransports. It is used by the new ORBit to
handle message transmission/receipt.

This package contains the header files and libraries needed to write
or compile programs that use linc.

%prep
%setup -q -n %{name}-%{version}

%build
%configure
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%makeinstall

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog README NEWS
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/*a
%{_libdir}/pkgconfig/*
%{_bindir}/*
%{_datadir}/aclocal/*


%changelog
* Tue Mar 05 2002 Chris Chabot <chabotc@reviewboard.com>
- Converted to configure.in
- final spit and polish

* Tue Jan 22 2002 Gregory Leblanc <gleblanc@linuxweasel.com>
- removed tabs from the header
- turn off libtoolize
- cleanup and add missing files to files section

* Sat Jan 19 2002 Chris Chabot <chabotc@reviewboard.com>
- Imported into gnome 2.0 alpha
- Bumped version to 0.13
- Cleanups, moved versions to defines

* Wed Jan  2 2002 Havoc Pennington <hp@redhat.com>
- 0.1.12.90 snap

* Sun Nov 25 2001 Havoc Pennington <hp@redhat.com>
- new cvs snap 0.1.9.90, rebuild for glib 1.3.11

* Fri Oct 26 2001 Havoc Pennington <hp@redhat.com>
- new cvs snap, rebuild on new glib

* Tue Oct  9 2001 Havoc Pennington <hp@redhat.com>
- new cvs snap with headers moved
- hrm, put headers in the devel package, doh

* Thu Oct  4 2001 Havoc Pennington <hp@redhat.com>
- cvs snap action

* Thu Sep 27 2001 Havoc Pennington <hp@redhat.com>
- move to released 0.1.5 tarball

* Fri Sep 21 2001 Havoc Pennington <hp@redhat.com>
- -devel package requires specific glib, update to new cvs snap

* Mon Sep 10 2001 Havoc Pennington <hp@redhat.com>
- update to CVS snapshot

* Tue Sep  4 2001 Havoc Pennington <hp@pobox.com>
- create package


