Summary:    Image viewer using Imlib 2
Name:       feh
Version:    1.3.4
Release:    1
Copyright:  BSD
Group:      Amusements/Graphics
Source:     %{name}-%{version}.tar.gz
Url:        http://www.linuxbrit.co.uk
BuildRoot:  /var/tmp/%{name}-%{version}-root

%description
feh is a versatile and fast image viewer using imlib2, the
premier image file handling library. feh has many features,
from simple single file viewing, to multiple file modes using
a slideshow or multiple windows. feh supports the creation of
montages as index prints with many user-configurable options.

%prep
%setup

%build
export CFLAGS="$RPM_OPT_FLAGS"
%{configure} --prefix=%{_prefix} --bindir=%{_bindir} \
             --mandir=%{_mandir} --datadir=%{_datadir}

make CFLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%files
%defattr(-, root, root)
%{_bindir}/*
%{_mandir}/*
%{_datadir}/%{name}/*

%doc AUTHORS ChangeLog README TODO

%changelog
* Wed Aug 23 2000 Calum Selkirk <cselkirk@sophix.uklinux.net>
- removed the double entry for doc

