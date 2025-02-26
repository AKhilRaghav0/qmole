%define version 0.8.14
%define release SNAP

Summary:   IDL parsing library
Name:      libIDL
Version:   %{version}
Release:   %{release}
Source:    %{name}-%PACKAGE_VERSION.tar.gz
Vendor:    Andrew T. Veliath <andrewtv@usa.net>
License:   LGPL
Group:     Libraries
Prereq:    /sbin/install-info
Prefix:    /usr
Docdir:    %prefix/doc
BuildRoot: /var/tmp/%{name}-%{version}-root

%changelog
* Fri Oct 12 2001 Ross Golder <ross@golder.org>
- Updated filenames and paths for libIDL2
* Mon Sep 24 2001 Mark McLoughlin <mark@skynet.ie>
- use LIBIDL_VERSION instead of LIBIDL_LIBRARY_VERSION
* Fri Nov 28 1998 Andrew T. Veliath <andrewtv@usa.net>
- Initial version

%description
libIDL is a small library for creating parse trees of CORBA v2.2
compliant Interface Definition Language (IDL) files, which is a
specification for defining interfaces which can be used between
different CORBA implementations.

%package devel
Summary:  Header files and libraries needed for libIDL development
Group:    Development/Libraries
Requires: %{name} = %{version}

%description devel
This package includes the header files and libraries needed for
developing programs using libIDL.

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr
make

%install
rm -rf $RPM_BUILD_ROOT
make install prefix=$RPM_BUILD_ROOT/%prefix
gzip -9 $RPM_BUILD_ROOT/%prefix/info/*.info

%post   -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%post devel
/sbin/install-info %prefix/info/libIDL2.info.gz %prefix/info/dir

%preun devel
if [ $1 = 0 ]; then
	/sbin/install-info --delete %prefix/info/libIDL2.info.gz %prefix/info/dir
fi

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc COPYING ChangeLog AUTHORS
%doc README* NEWS BUGS tstidl.c
%prefix/lib/lib*.so.*.*
%{_libdir}/libIDL-2.la
%{_libdir}/libIDL-2.so.0

%files devel
%defattr(-,root,root)
%prefix/bin/libIDL-config-2
#%prefix/lib/*.sh
%prefix/lib/pkgconfig/*.pc
%prefix/lib/lib*.a
%prefix/lib/lib*.so
%prefix/info/libIDL2.info.gz
%prefix/include/libIDL-2.0/libIDL/*.h
#%prefix/share/aclocal/*
