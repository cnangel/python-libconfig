Summary: A Python interface to libconfig
Name: python-libconfig
Version: 0.2.1
Release: 1%{?dist}
License: bsd
Group: Development/Libraries
Source0: %{name}-%{version}.tar.gz
URL: https://github.com/cnangel/python-libconfig

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: boost-devel python3-devel libconfig libconfig-devel gcc-c++
Requires: libconfig

%description
Python bindings to the C++ library libconfig (1.1.x ~ 1.8.x).
Supports reading, writing, and manipulating configuration files
via the libconfig C++ API through Boost.Python.

%prep
%setup -q -n %{name}-%{version}

%build
export libdirname=%{_lib}
CFLAGS="$RPM_OPT_FLAGS" python3 setup.py build

%install
rm -rf $RPM_BUILD_ROOT

export libdirname=%{_lib}
python3 setup.py install --root=$RPM_BUILD_ROOT --record=INSTALLED_FILES

%clean
rm -rf $RPM_BUILD_ROOT

%files -f INSTALLED_FILES
%defattr(-,root,root)
%doc README.md

%changelog
* Sat May 16 2026 Cnangel <cnangel@gmail.com> 0.2.1-1
- Fix version guards for multi-version libconfig compatibility (1.1.x ~ 1.8.x)
- Fix isString() implementation to use getType() instead of non-existent API

* Fri May 15 2026 Cnangel <cnangel@gmail.com> 0.2.0-1
- Upgrade to support libconfig 1.1.x ~ 1.8.x
- Export Setting class with full API
- Add type-safe lookup methods (lookupInt, lookupString, etc.)
- Add options, format, precision, tab_width, clear
- Export type/format/option constants
- Fix bugs: children() shadowed declaration, setValue type dispatch, add* empty path
- Use setValue with Python type introspection dispatching

* Fri Apr 16 2010 Cnangel <cnangel@gmail.com> 0.0.1-1
- Initial build
