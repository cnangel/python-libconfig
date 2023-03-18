Summary: A python interface to libconfig
Name: python-libconfig
Version: 0.0.4
Release: 1%{?dist}
License: bsd
Group: Development/Libraries
Source0: %{name}-%{version}.tar.gz
URL: https://github.com/cnangel/python-libconfig

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: boost-devel python-devel libconfig libconfig-devel
Requires: libconfig

%description
Python interface to libconfig

python-libconfig is an interface to the popular config for Python.

%prep
%setup -q -n %{name}-%{version}

%build
rm -f doc/*~
export libdirname=%{_lib}
CFLAGS="$RPM_OPT_FLAGS" python setup.py build

%install
rm -rf $RPM_BUILD_ROOT

export libdirname=%{_lib}
python setup.py install --root=$RPM_BUILD_ROOT --record=INSTALLED_FILES

%clean
rm -rf $RPM_BUILD_ROOT

%files -f INSTALLED_FILES
%defattr(-,root,root)
%doc README 

%changelog
* Sat Mar 18 2023 Cnangel <cnangel@gmail.com> 0.0.4-1
- fix root path issue.
* Mon Nov 04 2019 Cnangel <cnangel@gmail.com> 0.0.3-1
- update for support python3
* Mon Apr 14 2014 Cnangel <cnangel@gmail.com> 0.0.2-1
- modify some deps method
* Fri Apr 16 2010 Cnangel <cnangel@gmail.com> 0.0.1-1
- build the first spec file
