# python-libconfig

Python bindings to the C++ library libconfig (1.1.x ~ 1.8.x)

## Installation

First install libconfig and its development files.

**Fedora/RHEL:**

```sh
dnf install libconfig libconfig-devel python3-devel boost-devel gcc-c++
```

**Debian/Ubuntu:**

```sh
apt-get install libconfig++-dev python3-dev libboost-python-dev g++
```

**openSUSE:**

```sh
zypper install libconfig-devel python3-devel boost-devel gcc-c++
```
(available in the Packman repository)

On other platforms, compile libconfig from source:

```sh
wget http://www.hyperrealm.com/libconfig/libconfig-1.8.2.tar.gz
tar -zxf libconfig-1.8.2.tar.gz
cd libconfig-1.8.2
export MYPREFIX=/usr
# or if you lack privileges for making system-wide changes:
# export MYPREFIX=$HOME/local
./configure --prefix=$MYPREFIX
make
make install
```

To install this module, run the following commands:

```sh
python3 setup.py build
python3 setup.py install
```

## Usage

```python
from pylibconfig import Config, TYPE_INT, TYPE_STRING, FORMAT_HEX

cfg = Config()
cfg.readFile("example.cfg")

# Lookup with type-safe defaults
name = cfg.lookupString("name", "unknown")
port = cfg.lookupInt("port", 8080)

# Setting objects
root = cfg.getRoot()
setting = cfg.lookup("server.host")
print(setting.getName(), setting.getType(), setting.getPath())

# Options (libconfig 1.8+)
from pylibconfig import OPTION_FSYNC
cfg.setOption(OPTION_FSYNC, True)

# Format control
from pylibconfig import FORMAT_HEX
cfg.setDefaultFormat(FORMAT_HEX)
```

## Copyright and Licence

Copyright (C) 2010-2026 Cnangel

This program is released under the following license: bsd