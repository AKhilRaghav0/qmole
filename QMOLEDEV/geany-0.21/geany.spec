# Note: This spec file is very basic to provide functionality
# on all known RPM based distributions.
# It's stronly recommended to use packages of your own distributor

Name:          geany
Version:       0.21
Release:       1
Summary:       A fast and lightweight IDE using GTK2

Group:         Development/Tools
License:       GPLv2+
URL:           http://www.geany.org/
Vendor:        The Geany developer team <info@geany.org>
Packager:      Dominic Hopf <dmaphy@googlemail.com>
Source:        http://download.geany.org/%{name}-%{version}.tar.bz2

# The following tags files were retrieved 2nd Jan 2010
# You will have to download these and put them into your rpmbuild/SOURCES directory,
# otherwise comment these lines out
Source1:   http://download.geany.org/contrib/tags/sqlite3.c.tags
Source2:   http://download.geany.org/contrib/tags/std.glsl.tags
Source3:   http://download.geany.org/contrib/tags/gtk218.c.tags
Source4:   http://download.geany.org/contrib/tags/xfce46.c.tags
Source5:   http://download.geany.org/contrib/tags/dbus-glib-0.76.c.tags
Source6:   http://download.geany.org/contrib/tags/standard.css.tags
Source7:   http://download.geany.org/contrib/tags/geany-api-0.18.c.tags
Source8:   http://download.geany.org/contrib/tags/std.vala.tags
Source9:   http://download.geany.org/contrib/tags/drupal.php.tags
Source10:  http://download.geany.org/contrib/tags/std.latex.tags
Source11:  http://download.geany.org/contrib/tags/libxml-2.0.c.tags

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires:      glib2, gtk2, pango
BuildRequires: glib2-devel, gtk2-devel, pango-devel, gettext, intltool

%description
Geany is a small and fast integrated development enviroment with basic
features and few dependencies to other packages or Desktop Environments.

Some features:
- Syntax highlighting
- Code completion
- Code folding
- Construct completion/snippets
- Auto-closing of XML and HTML tags
- Call tips
- Support for Many languages like C, Java, PHP, HTML, Python, Perl, Pascal
- symbol lists and symbol name auto-completion
- Code navigation
- Simple project management
- Plugin interface

%package devel
Summary:   Header files for building Geany plug-ins
Group:     Development/Tools
Requires:  geany = %{version}-%{release}
Requires:  pkgconfig gtk2-devel

%description devel
This package contains the header files and pkg-config file needed for building
Geany plug-ins. You do not need to install this package to use Geany.

%prep
%setup -q
# remove waf since this isn't needed for the build, we're building the package
# with autotools
rm -f waf
rm -f wscript

%build
%configure
%__make

%install
%__rm -Rf $RPM_BUILD_ROOT
%makeinstall
# If you experience build problems like
# "Found '/home/user/rpmbuild/BUILDROOT/geany-0.16svn-3328.i386' in installed files; aborting"
# try uncommenting the following line to workaround the problem (and add sed to BuildRequires)
#sed -i "s@libdir='.*'@libdir='%{_libdir}/%{name}'@g" $RPM_BUILD_ROOT%{_libdir}/%{name}/*.la

%__rm -f $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/icon-theme.cache

# Install tags files
# comment the following two lines out if you didn't download the tags files mentioned
# above
mkdir -p $RPM_BUILD_ROOT%{_datadir}/%{name}/tags/
install -p %{SOURCE1} %{SOURCE2} %{SOURCE3} %{SOURCE4} %{SOURCE5} %{SOURCE6} %{SOURCE7} %{SOURCE8} %{SOURCE9} %{SOURCE10} %{SOURCE11} $RPM_BUILD_ROOT%{_datadir}/%{name}/tags/

%clean
%__rm -Rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root, -)
%doc %{_datadir}/doc/%{name}/*
%doc %{_mandir}/man1/%{name}.1.gz

%{_bindir}/%{name}
%{_datadir}/%{name}
%{_libdir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/locale/*/LC_MESSAGES/%{name}.mo
%{_datadir}/icons/hicolor/16x16/apps/*.png
%{_datadir}/icons/hicolor/48x48/apps/*.png
%{_datadir}/icons/hicolor/scalable/apps/*.svg


%files devel
%defattr(-, root, root, -)
%{_includedir}/geany
%{_libdir}/pkgconfig/geany.pc
