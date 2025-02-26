#
# spec file for package libproxy (Version 0.3.0)
#
# Copyright (c) 2009 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

%define         build_core_not_modules 1
%if 0%{suse_version} > 1110
%define xulrunner_ver 191
%else
%if 0%{suse_version} >= 1100
%define xulrunner_ver 190
%else
%define xulrunner_ver 181
%endif
%endif
Url:            http://code.google.com/p/libproxy/
%define _name   libproxy

Name:           libproxy
Group:          System/Libraries
Summary:        Libproxy provides consistent proxy configuration to applications
Version:        0.3.1
Release:        1
Source:         http://libproxy.googlecode.com/files/%{_name}-%{version}.tar.bz2
License:        LGPL v2.1 or later
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  pkg-config
BuildRequires:  python-devel
%if !%build_core_not_modules
BuildRequires:  dbus-1-devel
BuildRequires:  gconf2-devel
# For directory ownership, but also because we want to rebuild the modules if
# the library changed
BuildRequires:  libproxy0
BuildRequires:  libwebkit-devel
BuildRequires:  mozilla-xulrunner%{xulrunner_ver}-devel
BuildRequires:  NetworkManager-devel
BuildRequires:  xorg-x11-libXmu-devel
%endif
%if %suse_version <= 1110
%define python_sitelib %{py_sitedir}
%endif

%description
libproxy offers the following features: * extremely small core
   footprint (< 35K)

* no external dependencies within libproxy core (libproxy modules
   may have dependencies)

* only 3 functions in the stable external API

* dynamic adjustment to changing network topology

* a standard way of dealing with proxy settings across all scenarios


%if %build_core_not_modules

%package tools
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        A simple application using libproxy
Group:          System/Libraries
Requires:       libproxy0 = %{version}

%description tools
A simple application that will use libproxy to give the results you can
expect from other applications. It can be used to debug what would
happen in various cases.


%package devel 
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy provides consistent proxy configuration to applications - Development Files
Group:          Development/Libraries/C and C++
Requires:       libproxy0 = %{version}

%description devel
libproxy offers the following features: * extremely small core
   footprint (< 35K)

* no external dependencies within libproxy core (libproxy modules
   may have dependencies)

* only 3 functions in the stable external API

* dynamic adjustment to changing network topology

* a standard way of dealing with proxy settings across all scenarios


%package -n libproxy0          
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy provides consistent proxy configuration to applications
Group:          System/Libraries

%description -n libproxy0
libproxy offers the following features: * extremely small core
   footprint (< 35K)

* no external dependencies within libproxy core (libproxy modules
   may have dependencies)

* only 3 functions in the stable external API

* dynamic adjustment to changing network topology

* a standard way of dealing with proxy settings across all scenarios


%package -n python-libproxy
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Python bindings for libproxy
Group:          System/Libraries
Requires:       libproxy0 = %{version}
%py_requires

%description -n python-libproxy
libproxy offers the following features: * extremely small core
   footprint (< 35K)

* no external dependencies within libproxy core (libproxy modules
   may have dependencies)

* only 3 functions in the stable external API

* dynamic adjustment to changing network topology

* a standard way of dealing with proxy settings across all scenarios


%package -n libproxy-sharp
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        .Net bindings for libproxy
Group:          Development/Languages/Mono
Requires:       libproxy0 = %{version}

%description -n libproxy-sharp
libproxy offers the following features: * extremely small core
   footprint (< 35K)

* no external dependencies within libproxy core (libproxy modules
   may have dependencies)

* only 3 functions in the stable external API

* dynamic adjustment to changing network topology

* a standard way of dealing with proxy settings across all scenarios


%else

%package -n libproxy0-config-gnome
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module for GNOME configuration
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Recommends:     libproxy0-pacrunner = %{version}
Recommends:     libproxy0-wpad-dns = %{version}
Supplements:    packageand(libproxy0:gconf2)
Provides:       libproxy-gnome = %{version}
Obsoletes:      libproxy-gnome < %{version}
Provides:       libproxy0-gnome = %{version}
Obsoletes:      libproxy0-gnome < %{version}


%description -n libproxy0-config-gnome
A module to extend libproxy with capabilities to query GNOME about
proxy settings.


%package -n libproxy0-config-kde
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module for KDE configuration
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Recommends:     libproxy0-pacrunner = %{version}
Recommends:     libproxy0-wpad-dns = %{version}
Supplements:    packageand(libproxy0:libkde4)
Provides:       libproxy-kde = %{version}
Obsoletes:      libproxy-kde < %{version}
Provides:       libproxy0-kde = %{version}
Obsoletes:      libproxy0-kde < %{version}

%description -n libproxy0-config-kde
A module to extend libproxy with capabilities to query KDE about proxy
settings.


%package -n libproxy0-pacrunner-mozjs
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module to support wpad/pac parsing via Mozilla JavaScript Engine
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Supplements:    packageand(libproxy0:mozilla-xulrunner%{xulrunner_ver})
# A virtual symbol to identify that this is a pacrunner.
Provides:       libproxy0-pacrunner = %{version}
Provides:       libproxy-mozjs = %{version}
Obsoletes:      libproxy-mozjs < %{version}
Provides:       libproxy0-mozjs = %{version}
Obsoletes:      libproxy0-mozjs < %{version}

%description -n libproxy0-pacrunner-mozjs
A module to extend libproxy with capabilities to pass addresses to a
WPAD/PAC script and have it find the correct proxy.


%package -n libproxy0-pacrunner-webkit
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module to support WPAD/PAC parsing via WebKit JavaScript Engine
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Supplements:    packageand(libproxy0:libwebkit-1)
Provides:       libproxy0-pacrunner = %{version}
Provides:       libproxy-webkit = %{version}
Obsoletes:      libproxy-webkit < %{version}
Provides:       libproxy0-webkit = %{version}
Obsoletes:      libproxy0-webkit < %{version}

%description -n libproxy0-pacrunner-webkit
A module to extend libproxy with capabilities to pass addresses to a
WPAD/PAC script and have it find the correct proxy.


%package -n libproxy0-networkmanager
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module for NetworkManager configuration
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Supplements:    packageand(libproxy0:NetworkManager)
Provides:       libproxy-networkmanager = %{version}
Obsoletes:      libproxy-networkmanager < %{version}

%description -n libproxy0-networkmanager
A module to extend libproxy with capabilities to query NetworkManager
about network configuration changes.


%package -n libproxy0-config-wpad
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module for WPAD autofallback
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Requires:       libproxy0-wpad-dns = %{version}

%description -n libproxy0-config-wpad
A module to extend libproxy with capabilities to query wpad://
in case no other module returns valid configuration.

This might not be wanted in tight security environments.


%package -n libproxy0-wpad-dns
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module for WPAD Autofallback
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Requires:       libproxy0-pacrunner = %{version}
Recommends:     libproxy-wpad-dns-devolution = %{version}

%description -n libproxy0-wpad-dns
This modules performs the DNS queries needed to find
the wpad:// configuration.

The module is needed to exist whenever automatic proxy
configuration is used.


%package -n libproxy0-wpad-dns-devolution
License:        GPL v2 or later ; LGPL v2.1 or later
Summary:        Libproxy module for WPAD Autofallback
Group:          System/Libraries
Requires:       libproxy0 = %{version}
Requires:       libproxy0-pacrunner = %{version}
Requires:       libproxy0-wpad-dns = %{version}

%description -n libproxy0-wpad-dns-devolution
A module to extend wpad:// capabilities of libproxy with DNS
devolution. The module is written with care and it will never
fall back to the .TLD domains.


%endif

%prep
%setup -q -n %{_name}-%{version}

%build
%configure \
%if %build_core_not_modules
  --without-wpad \
%endif
  --with-python \
  --disable-static
%{__make} %{?jobs:-j%jobs}

%install
%if %build_core_not_modules
%makeinstall
rm %{buildroot}%{_libdir}/*.la
%else
cd src/lib
%makeinstall
cd ../modules
%makeinstall
# remove files that are part of the core
rm %{buildroot}%{_includedir}/*.h
rm %{buildroot}%{_libdir}/libproxy.*
rm %{buildroot}%{_libdir}/libproxy/%{version}/modules/config_envvar.so
rm %{buildroot}%{_libdir}/libproxy/%{version}/modules/config_file.so
rm %{buildroot}%{_libdir}/libproxy/%{version}/modules/config_direct.so
rm %{buildroot}%{_libdir}/libproxy/%{version}/modules/ignore_*.so
%endif
%if %build_core_not_modules

%post -n libproxy0 -p /sbin/ldconfig

%postun -n libproxy0 -p /sbin/ldconfig

%files tools
%defattr(-, root, root)
%{_bindir}/proxy

%files -n libproxy0
%defattr(-, root, root)
%{_libdir}/*.so.*
%dir %{_libdir}/libproxy
%dir %{_libdir}/libproxy/%{version}
%dir %{_libdir}/libproxy/%{version}/modules
%{_libdir}/libproxy/%{version}/modules/config_envvar.so
%{_libdir}/libproxy/%{version}/modules/config_file.so
%{_libdir}/libproxy/%{version}/modules/config_direct.so
%{_libdir}/libproxy/%{version}/modules/ignore_*.so

%files devel
%defattr(-, root, root)
%{_includedir}/*.h
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%files -n python-libproxy
%defattr(-, root, root)
%{python_sitelib}/*.py
%else

%files -n libproxy0-config-kde
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/config_kde.so

%files -n libproxy0-config-gnome
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/config_gnome.so

%files -n libproxy0-config-wpad
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/config_wpad.so

%files -n libproxy0-wpad-dns
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/wpad_dns.so

%files -n libproxy0-wpad-dns-devolution
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/wpad_dnsdevolution.so

%files -n libproxy0-networkmanager
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/network_networkmanager.so

%files -n libproxy0-pacrunner-webkit
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/pacrunner_webkit.so

%files -n libproxy0-pacrunner-mozjs
%defattr(-, root, root)
%{_libdir}/libproxy/%{version}/modules/pacrunner_mozjs.so
%endif

%changelog
