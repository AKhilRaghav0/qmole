%define __libtoolize :
%define __spec_install_post /usr/lib/rpm/brp-compress
Name:             librsvg2
Summary:          An SVG library based on cairo.
Version:          2.32.1
Release:          2
License:          LGPL
Group:            System Environment/Libraries
Source:           librsvg-%{version}.tar.gz
URL:		  http://librsvg.sourceforge.net/
BuildRoot:        %{_tmppath}/%{name}-%{version}-root
BuildRequires:    pkgconfig >= 0.8
Requires:         glib2 >= @GLIB_REQUIRED@
Requires:         cairo >= @CAIRO_REQUIRED@
Requires:         libxml2 >= @LIBXML_REQUIRED@
Requires:         pango >= @PANGOFT2_REQUIRED@
Requires:	  libgsf >= 1.6.0
BuildRequires:    glib2-devel >= @GLIB_REQUIRED@
BuildRequires:    cairo-devel >= @CAIRO_REQUIRED@
BuildRequires:    libxml2-devel >= @LIBXML_REQUIRED@
BuildRequires:    pango-devel >= @PANGOFT2_REQUIRED@
BuildRequires:	  libgsf >= 1.6.0

%description
An SVG library based on cairo.


%package devel
Summary:          Libraries and include files for developing with librsvg.
Group:            Development/Libraries
Requires:         %{name} = %{version}
Requires:         pkgconfig >= 0.8
Requires:         glib2 >= @GLIB_REQUIRED@
Requires:         glib2-devel >= @GLIB_REQUIRED@
Requires:         cairo >= @CAIRO_REQUIRED@
Requires:         cairo-devel >= @CAIRO_REQUIRED@
Requires:         libxml2 >= @LIBXML_REQUIRED@
Requires:         libxml2-devel >= @LIBXML_REQUIRED@
Requires:         pango >= @PANGOFT2_REQUIRED@
Requires:         pango-devel >= @PANGOFT2_REQUIRED@


%description devel
This package provides the necessary development libraries and include
files to allow you to develop with librsvg.

%package -n librsvg2-gtk
Summary:        Gtk+ 2.0 theme engine for SVG based themes
Group:          System Environment/Libraries
Requires:         gtk2 >= @GDK_PIXBUF_REQUIRED@
BuildRequires:         gtk2-devel >= @GDK_PIXBUF_REQUIRED@


%description -n librsvg2-gtk
This package installs a GTK+ 2.0 theme engine that uses SVG images. It is based
on the gdkpixbuf engine.

%prep
%setup -q -n librsvg-%{version}

%build
%configure --enable-svgz
make

%install
rm -rf $RPM_BUILD_ROOT

%makeinstall
# Clean out files that should not be part of the rpm.
# This is the recommended way of dealing with it for RH8
rm -f $RPM_BUILD_ROOT%{_libdir}/gtk-2.0/2.4.0/engines/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/gtk-2.0/2.4.0/loaders/*.la
rm -f $RPM_BUILD_ROOT%{_libdir}/*.la
rm -f $RPM_BUILD_ROOT%{_sysconfdir}/gtk-2.0/gdk-pixbuf.loaders
mkdir -p $RPM_BUILD_DIR/librsvg-%{version}
touch $RPM_BUILD_DIR/librsvg-%{version}/debugfiles.list
%clean
rm -rf $RPM_BUILD_ROOT

%post -n librsvg2-gtk
/sbin/ldconfig
mkdir -p %{_sysconfdir}/gtk-2.0
gdk-pixbuf-query-loaders-32 > %{_sysconfdir}/gtk-2.0/gdk-pixbuf.loaders

%postun -n librsvg2-gtk
/sbin/ldconfig
gdk-pixbuf-query-loaders-32 > %{_sysconfdir}/gtk-2.0/gdk-pixbuf.loaders

%files
%defattr(-, root, root)
%doc AUTHORS COPYING COPYING.LIB ChangeLog NEWS README TODO
%{_libdir}/*.so.*
%{_bindir}/rsvg
%{_mandir}/*/*

%files devel
%defattr(-, root, root)
%{_libdir}/*.*
%{_includedir}/librsvg-2/librsvg/*.h
%{_libdir}/pkgconfig/librsvg-2.0.pc
%{_datadir}/gtk-doc/html/rsvg/*

%files -n librsvg2-gtk
%defattr(-, root, root)
%{_bindir}/rsvg*
%{_libdir}/gtk-2.0/2.4.0/engines/*
%{_libdir}/gtk-2.0/2.4.0/loaders/*
%{_datadir}/pixmaps/*
# %{_datadir}/themes/bubble/gtk-2.0/*
# %{_datadir}/themes/bubble/README

%changelog
* Sat Mar 20 2004 Christian Schaller <Uraeus@gnome.org>
- Update the path for the engines and loaders
* Thu Mar 18 2004 Christian Schaller <Uraeus@gnome.org>
- Add mozilla plugins (to gtk for now)
- Add .a files since they should probably be installed

* Sun Feb 02 2003 Christian Schaller <Uraeus@linuxrising.org>
- Update to handle latest changes
- Add some fixes from the RH spec file
- renamed rsvg-gtk package to librsvg-gtk
- Improve depency listing somewhat

* Thu Oct 22 2002 Christian Schaller <Uraeus@linuxrising.org>
- Disabled building of example theme (as done in gtk-engines)

* Mon Oct 21 2002 Christian Schaller <Uraeus@linuxrising.org>
- Fixes for RH 8 
- Adding gtk theme engine
- adding gdk-loader

* Tue Mar 05 2002 Chris Chabot <chabotc@reviewboard.com>
- Deps
- Formatting
- converted to .spec.in

* Sat Jan 19 2002 Chris Chabot <chabotc@reviewboard.com>
- Imported into gnome 2.0 alpha, set Requirements accordingly
- Bumped version to 1.1.1
- Minor cleanups

* Wed Jan  2 2002 Havoc Pennington <hp@redhat.com>
- new CVS snap 1.1.0.91
- remove automake/autoconf calls

* Mon Nov 26 2001 Havoc Pennington <hp@redhat.com>
- convert to librsvg2 RPM

* Tue Oct 23 2001 Havoc Pennington <hp@redhat.com>
- 1.0.2

* Fri Jul 27 2001 Alexander Larsson <alexl@redhat.com>
- Add a patch that moves the includes to librsvg-1/librsvg
- in preparation for a later librsvg 2 library.

* Tue Jul 24 2001 Havoc Pennington <hp@redhat.com>
- build requires gnome-libs-devel, #49509

* Thu Jul 19 2001 Havoc Pennington <hp@redhat.com>
- own /usr/include/librsvg

* Wed Jul 18 2001 Akira TAGOH <tagoh@redhat.com> 1.0.0-4
- fixed the linefeed problem in multibyte environment. (Bug#49310)

* Mon Jul 09 2001 Havoc Pennington <hp@redhat.com>
- put .la file back in package

* Fri Jul  6 2001 Trond Eivind Glomsr�d <teg@redhat.com>
- Put changelog at the end
- Move .so files to devel subpackage
- Don't mess with ld.so.conf
- Don't use %%{prefix}, this isn't a relocatable package
- Don't define a bad docdir
- Add BuildRequires
- Use %%{_tmppath}
- Don't define name, version etc. on top of the file (why do so many do that?)
- s/Copyright/License/

* Wed May  9 2001 Jonathan Blandford <jrb@redhat.com>
- Put into Red Hat Build system

* Tue Oct 10 2000 Robin Slomkowski <rslomkow@eazel.com>
- removed obsoletes from sub packages and added mozilla and trilobite
subpackages

* Wed Apr 26 2000 Ramiro Estrugo <ramiro@eazel.com>
- created this thing

