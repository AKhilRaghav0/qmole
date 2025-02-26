%define name libgsf
%define version 1.14.22
%define release 1
%define prefix /usr

Summary: GNOME Structured File library

Name: %{name}
Version: %{version}
Release: %{release}
Group: System Environment/Libraries
License: LGPL

Source: http://ftp.gnome.org/pub/GNOME/sources/libgsf/%{name}-%{version}.tar.gz
Buildroot: /var/tmp/%{name}-%{version}-%{release}-root
URL: http://www.gnumeric.org

Requires: glib2 >= 2.16.0
BuildRequires: glib2-devel >= 2.16.0

%description
A library for reading and writing structured files (eg MS OLE and Zip)

%package devel
Summary: Support files necessary to compile applications with libgsf.
Group: Development/Libraries
Requires: libgsf

%description devel
Libraries, headers, and support files necessary to compile applications using libgsf.

%prep

%setup

%build
%ifarch alpha
  MYARCH_FLAGS="--host=alpha-redhat-linux"
%endif

if [ ! -f configure ]; then
CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%{prefix}
else
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{prefix}
fi

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -r $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT%{prefix}
make prefix=$RPM_BUILD_ROOT%{prefix} install

%files
%defattr(644,root,root,755)
%doc AUTHORS COPYING README
%{prefix}/lib/lib*.so*

%files devel
%defattr(644,root,root,755)
%{prefix}/lib/*a
%{prefix}/lib/pkgconfig/libgsf-*1.pc
%{prefix}/include/libgsf-1/*
%{prefix}/share/doc/libgsf/html/*

%clean
rm -r $RPM_BUILD_ROOT

%changelog
* Tue May 13 2003 Rui M. Seabra <rms@407.org>
- fix spec to reflect current stat of the build

* Tue Jun 18 2002 Rui M. Seabra <rms@407.org>
- set permission correctly
- fix common mistake of Copyright flag into License flag.

* Thu May 23 2002 Jody Goldberg <jody@gnome.org>
- Initial version
