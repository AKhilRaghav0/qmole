# -*- mode: rpm-spec -*-

# By default, this spec file will generate RPMs for clisp
# This can be changed by modifying the variables below.
%define enable_clisp 1
%define clisp_flags --enable-clisp
%define enable_cmucl 0
%define cmucl_flags --disable-cmucl
%define enable_sbcl 0
%define sbcl_flags --disable-sbcl
%define enable_gcl 0
%define gcl_flags --disable-gcl

# Options to build language packs
#  Spanish 
%define enable_lang_es 1
%define lang_es_flags --enable-lang-es
#  Spanish UTF-8  
%define enable_lang_es_utf 1
%define lang_es_flags_utf --enable-lang-es-utf8
#  Portuguese
%define enable_lang_pt 1
%define lang_pt_flags --enable-lang-pt
#  Portuguese UTF-8
%define enable_lang_pt_utf 1
%define lang_pt_flags_utf --enable-lang-pt-utf8
#  Brazilian Portuguese
%define enable_lang_pt_br 1
%define lang_pt_br_flags --enable-lang-pt_BR
#  Brazilian Portuguese (UTF-8)
%define enable_lang_pt_br_utf 1
%define lang_pt_br_flags_utf --enable-lang-pt_BR-utf8

# Inhibit automatic compressing of info files. Compressed info
# files break maxima's internal help.
%define __spec_install_post /bin/true

Summary: Symbolic Computation Program
Name: maxima
Version: 5.29.1
Release: 1
License: GPL
Group: Sciences/Mathematics
URL: http://maxima.sourceforge.net
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Requires: maxima-exec = %{version}

%description

Maxima is a system for the manipulation of symbolic
and numerical expressions, including differentiation,
integration, Taylor series, Laplace transforms,
ordinary differential equations, systems of linear
equations, polynomials, sets, lists, vectors, matrices
and tensors.
Maxima yields high precision numeric results by using
exact fractions, arbitrary-precision integers and
arbitrary-precision floating point numbers.
Maxima can plot functions and data in two and three
dimensions, and it comes with hundreds of self tests.
Maxima is based on the original Macsyma developed
at MIT in the 1970's.

%package xmaxima
Summary: Openmath graphics program and interface to Maxima
Group: Sciences/Mathematics
Requires: maxima = %{version}, tk
Provides: xmaxima
%description xmaxima

A graphical interface to the Maxima symbolic computation
program. It also provides Openmath, a graphics program
that can be used from Maxima, and a Web browser that
accepts a custom html tag to execute Maxima commands from
an html page. Xmaxima is written in the Tcl/Tk language.

%if %{enable_clisp}
%package exec-clisp
Summary: Maxima compiled with clisp
Group: Sciences/Mathematics
Requires: maxima = %{version}
Provides: maxima-exec
%description exec-clisp
Maxima compiled with clisp.
%endif

%if %{enable_cmucl}
%package exec-cmucl
Summary: Maxima compiled with CMUCL
Group: Sciences/Mathematics
Requires: maxima = %{version}
Provides: maxima-exec
%description exec-cmucl
Maxima compiled with CMUCL.
%endif

%if %{enable_sbcl}
%package exec-sbcl
Summary: Maxima compiled with SBCL
Group: Sciences/Mathematics
Requires: maxima = %{version}
Provides: maxima-exec
%description exec-sbcl
Maxima compiled with SBCL.
%endif

%if %{enable_gcl}
%package exec-gcl
Summary: Maxima compiled with GCL
Group: Sciences/Mathematics
Requires: maxima = %{version}
Provides: maxima-exec
%description exec-gcl
Maxima compiled with Gnu Common Lisp.
%endif

%if %{enable_lang_es}
%package lang-es
Summary: Maxima Spanish language pack
Group: Sciences/Mathematics
Requires: maxima = %{version}
%description lang-es
Maxima Spanish lagnuage support.
%endif

%if %{enable_lang_es_utf}
%package lang-es-utf8
Summary: Maxima Spanish UTF-8 language pack
Group: Sciences/Mathematics
Requires: maxima = %{version}
%description lang-es-utf8
Maxima Spanish lagnuage support (UTF-8).
%endif

%if %{enable_lang_pt}
%package lang-pt
Summary: Maxima Portuguese language pack
Group: Sciences/Mathematics
Requires: maxima = %{version}
%description lang-pt
Maxima Portuguese lagnuage support.
%endif

%if %{enable_lang_pt_utf}
%package lang-pt-utf8
Summary: Maxima Portuguese UTF-8 language pack
Group: Sciences/Mathematics
Requires: maxima = %{version}
%description lang-pt-utf8
Maxima Portuguese lagnuage support (UTF-8).
%endif

%if %{enable_lang_pt_br}
%package lang-pt_BR
Summary: Maxima Brazilian Portuguese language pack
Group: Sciences/Mathematics
Requires: maxima = %{version}
%description lang-pt_BR
Maxima Brazilian Portuguese lagnuage support.
%endif

%if %{enable_lang_pt_br_utf}
%package lang-pt_BR-utf8
Summary: Maxima Brazilian Portuguese UTF-8 language pack
Group: Sciences/Mathematics
Requires: maxima = %{version}
%description lang-pt_BR-utf8
Maxima Brazilian Portuguese lagnuage support (UTF-8).
%endif

%prep
%setup -q
%configure  %{?sbcl_flags:} %{?cmucl_flags:} %{?gcl_flags:} %{?clisp_flags:} \
	    %{?lang_es_flags:} %{?lang_es_flags_utf:} \
	    %{?lang_pt_flags:} %{?lang_pt_flags_utf:} \
	    %{?lang_pt_br_flags:} %{?lang_pt_br_flags_utf:}

%build
make

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall
  make \
	prefix=%{?buildroot:%{buildroot}}%{_prefix} \
	exec_prefix=%{?buildroot:%{buildroot}}%{_exec_prefix} \
	bindir=%{?buildroot:%{buildroot}}%{_bindir} \
	sbindir=%{?buildroot:%{buildroot}}%{_sbindir} \
	sysconfdir=%{?buildroot:%{buildroot}}%{_sysconfdir} \
	datadir=%{?buildroot:%{buildroot}}%{_datadir} \
	includedir=%{?buildroot:%{buildroot}}%{_includedir} \
	libdir=%{?buildroot:%{buildroot}}%{_libdir} \
	libexecdir=%{?buildroot:%{buildroot}}%{_libexecdir} \
	localstatedir=%{?buildroot:%{buildroot}}%{_localstatedir} \
	sharedstatedir=%{?buildroot:%{buildroot}}%{_sharedstatedir} \
	mandir=%{?buildroot:%{buildroot}}%{_mandir} \
	infodir=%{?buildroot:%{buildroot}}%{_infodir} \
  install-info
touch debugfiles.list
#  Deal with info/dir
rm -f $RPM_BUILD_ROOT%{_datadir}/info/dir

%post
/sbin/install-info %{_infodir}/maxima.info %{_infodir}/dir

%postun
if [ "$1" = 0 ]; then
	/sbin/install-info --delete %{_infodir}/maxima.info %{_infodir}/dir
fi

%post xmaxima
/sbin/install-info %{_infodir}/xmaxima.info %{_infodir}/dir

%postun xmaxima
if [ "$1" = 0 ]; then
	/sbin/install-info --delete %{_infodir}/xmaxima.info %{_infodir}/dir
fi

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%dir %{_datadir}/maxima
%exclude %{_datadir}/maxima/%{version}/xmaxima
%{_datadir}/maxima/%{version}
%if %{enable_lang_es}
%exclude %{_datadir}/maxima/%{version}/doc/html/es
%endif
%if %{enable_lang_pt}
%exclude %{_datadir}/maxima/%{version}/doc/html/pt
%endif
%if %{enable_lang_pt_br}
%exclude %{_datadir}/maxima/%{version}/doc/html/pt_BR
%endif
%if %{enable_lang_es_utf}
%exclude %{_datadir}/maxima/%{version}/doc/html/es.utf8
%endif
%if %{enable_lang_pt_utf}
%exclude %{_datadir}/maxima/%{version}/doc/html/pt.utf8
%endif
%if %{enable_lang_pt_br_utf}
%exclude %{_datadir}/maxima/%{version}/doc/html/pt_BR.utf8
%endif
%dir %{_libexecdir}/maxima
%{_libexecdir}/maxima/%{version}
%{_infodir}/*
%exclude %{_infodir}/xmaxima.info*
%if %{enable_lang_es}
%exclude %{_infodir}/es
%endif
%if %{enable_lang_pt}
%exclude %{_infodir}/pt
%endif
%if %{enable_lang_pt_br}
%exclude %{_infodir}/pt_BR
%endif
%if %{enable_lang_es_utf}
%exclude %{_infodir}/es.utf8
%endif
%if %{enable_lang_pt_utf}
%exclude %{_infodir}/pt.utf8
%endif
%if %{enable_lang_pt_br_utf}
%exclude %{_infodir}/pt_BR.utf8
%endif
%{_mandir}/man1/maxima.1
%{_bindir}/maxima
%{_bindir}/rmaxima
%doc AUTHORS COPYING INSTALL INSTALL.lisp NEWS 
%doc README README.lisps README.rpms README.external README.i18n

%files xmaxima
%{_bindir}/xmaxima
%{_datadir}/maxima/%{version}/xmaxima
%{_infodir}/xmaxima*

%if %{enable_clisp}
%files exec-clisp
%{_libdir}/maxima/%{version}/binary-clisp
%endif

%if %{enable_cmucl}
%files exec-cmucl
%{_libdir}/maxima/%{version}/binary-cmucl
%endif

%if %{enable_sbcl}
%files exec-sbcl
/usr/lib/maxima/%{version}/binary-sbcl
%endif

%if %{enable_gcl}
%files exec-gcl
%{_libdir}/maxima/%{version}/binary-gcl
%endif

%if %{enable_lang_es}
%files lang-es
%{_datadir}/maxima/%{version}/doc/html/es
%{_infodir}/es
%endif

%if %{enable_lang_es_utf}
%files lang-es-utf8
%{_datadir}/maxima/%{version}/doc/html/es.utf8
%{_infodir}/es.utf8
%endif

%if %{enable_lang_pt}
%files lang-pt
%{_datadir}/maxima/%{version}/doc/html/pt
%{_infodir}/pt
%endif

%if %{enable_lang_pt_utf}
%files lang-pt-utf8
%{_datadir}/maxima/%{version}/doc/html/pt.utf8
%{_infodir}/pt.utf8
%endif

%if %{enable_lang_pt_br}
%files lang-pt_BR
%{_datadir}/maxima/%{version}/doc/html/pt_BR
%{_infodir}/pt_BR
%endif

%if %{enable_lang_pt_br_utf}
%files lang-pt_BR-utf8
%{_datadir}/maxima/%{version}/doc/html/pt_BR.utf8
%{_infodir}/pt_BR.utf8
%endif

%doc


%changelog
* Sat Jan 20 2007 Vadim Zhytnikov <vvzhy@netorn.ru>
- Brazilian Portuguese language packs.

* Mon Dec 11 2006 Jaime Villate <villate@fe.up.pt>
- Moved xmaxima files to the maxima-xmaxima package.
- Updated description of Xmaxima.

* Wed Dec 28 2005 Vadim Zhytnikov <vvzhy@netorn.ru>
- Spanish and Portuguese language packs.
- Package maxima subdirectories for clean uninstall.

* Sat Jan  4 2003 James Amundson <amundson@fnal.gov>
- Added doc files
- Added explicit clisp version
- Added conditional to postun
- Renamed exec_* packages to exec-*
- Use rpm macros instead of hard-coded paths
- Thanks to Rex Dieter for helpful suggestions

* Sun Sep  8 2002 James Amundson <amundson@fnal.gov> 
- Initial build.
