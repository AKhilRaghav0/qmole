Name: 		xfe
Version:	1.34
Summary: X File Explorer (Xfe) is a file manager for X. 
Release: 1
License: GPL
Group: File tools
Requires: fox >= 1.6 libpng >= 1.2
BuildRequires: fox-devel >= 1.6 libpng-devel >= 1.2
Source: %{name}-%{version}.tar.gz
Packager: Roland Baudin <roland65@free.fr>
BuildRoot: %{_tmppath}/%{name}-buildroot

%description
X File Explorer (Xfe) is a filemanager for X. It is based on the popular X Win Commander, which is
discontinued. Xfe is desktop independent and is written with the C++ Fox Toolkit. It has Windows Commander
or MS-Explorer look and is very fast and simple. The main features are: file associations,
mount/umount devices, directory tree for quick cd, change file attributes, auto
save registry, compressed archives view/creation/extraction and much more.

%prep
%setup -q

%build
%configure --with-included-gettext --enable-release
make


%install
rm -rf %{buildroot}
%makeinstall
%find_lang %{name}
if [ -f %{buildroot}%{_datadir}/locale/locale.alias ]; then
	rm %{buildroot}%{_datadir}/locale/locale.alias
fi


%clean
rm -rf %{buildroot}

%files -f %{name}.lang
%defattr(644,root,root,755)
%doc AUTHORS COPYING README TODO BUGS
%attr(755,root,root) %{_bindir}/*
%{_datadir}/xfe/icons/*
%{_datadir}/xfe/xferc
%{_datadir}/applications/xf*.desktop
%{_datadir}/pixmaps/*
%{_mandir}/man1/*

%changelog
* Tue Sep  8 2009 Roland Baudin <roland65@free.fr>
- Added desktop files to the files section

* Tue Feb  13 2007 Roland Baudin <roland65@free.fr>
- Fixed again the location of the config file xferc

* Tue Feb  6 2007 Roland Baudin <roland65@free.fr>
- Rebuild for Fedora Core 6
- Fixed the location of the config file xferc

* Thu Nov 23 2006 Roland Baudin <roland65@free.fr>
- Added configure --enable-release option

* Wed Oct 11 2006 Roland Baudin <roland65@free.fr>
- FOX 1.6.x support
- Removed the static build option

* Tue Jun 21 2005 Roland Baudin <roland65@free.fr>
- FOX 1.4.x support.

* Tue Aug 3 2004 Andrzej Stypula <andrzej@altair.krakow.pl>
- locale adjustment

* Thu Jul 29 2004 Andrzej Stypula <andrzej@altair.krakow.pl>
- file permissions adjustment

* Thu Jul 29 2004 Roland Baudin <roland65@free.fr>
- FOX 1.2.x support.

* Fri Dec 19 2003 Roland Baudin <roland65@free.fr>
- Rebuild for Fedora Core 1.

* Mon Oct 8 2003 Roland Baudin <roland65@free.fr>
- Add of libPNG requirements.

* Mon Sep 8 2003 Roland Baudin <roland65@free.fr>
- Spec file for RedHat 9.

* Fri Jul 18 2003 Roland Baudin <roland65@free.fr>
- Add of the man pages and fix of the locale.alias problem.

* Mon Apr 14 2003 Roland Baudin <roland65@free.fr>
- Fixed the Xfe icon destination.
 
* Fri Apr 11 2003 Roland Baudin <roland65@free.fr>
- Add of i18n.

* Tue Jan 28 2003 Roland Baudin <roland65@free.fr>
- Add of the '--with-static' build option.

* Thu Oct 15 2002 Roland Baudin <roland65@free.fr>
- First release of the spec file for RedHat 7.3.
