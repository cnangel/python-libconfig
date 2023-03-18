Summary: A python interface to libconfig
Name: python-libconfig
Version: 0.0.2
Release: 1
License: bsd
Group: Development/Libraries
Source0: %{name}-%{version}.tar.gz
URL: http://code.google.com/r/cnangel-python-libconfig/

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: boost-devel python-devel libconfig libconfig-devel
Requires: libconfig

%description
Python interface to libconfig

python-libconfig is an interface to the popular config for Python.

%prep
%setup -q -n %{name}-%{version}c1

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
%{_libdir}/python?.?/site-packages/*.so

%changelog
* Sat Mar 18 2023 Cnangel <cnangel@gmail.com> 0.0.2-1
- fix root path issue.
* Fri Apr 16 2010 Cnangel <cnangel@gmail.com> 0.0.1-1
- build the first spec file
