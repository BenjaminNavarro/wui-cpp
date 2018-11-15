
Overview
=========

An easy to use Web-based GUI for your C++ programs

The license that applies to the whole package content is **GNULGPL**. Please look at the license.txt file at the root of this repository.

Installation and Usage
=======================

The detailed procedures for installing the wui-cpp package and for using its components is based on the [PID](http://pid.lirmm.net/pid-framework/pages/install.html) build and deployment system called PID. Just follow and read the links to understand how to install, use and call its API and/or applications.

For a quick installation:

## Installing the project into an existing PID workspace

To get last version :
 ```
cd <path to pid workspace>/pid
make deploy package=wui-cpp
```

To get a specific version of the package :
 ```
cd <path to pid workspace>/pid
make deploy package=wui-cpp version=<version number>
```

## Standalone install
 ```
git clone https://github.com/BenjaminNavarro/wui-cpp.git
cd wui-cpp
```

Then run the adequate install script depending on your system. For instance on linux:
```
sh share/install/standalone_install.sh
```

The pkg-config tool can be used to get all links and compilation flags for the libraries defined inthe project. To let pkg-config know these libraries, read the last output of the install_script and apply the given command. It consists in setting the PKG_CONFIG_PATH, for instance on linux do:
```
export PKG_CONFIG_PATH=<path to wui-cpp>/binaries/pid-workspace/share/pkgconfig:$PKG_CONFIG_PATH
```

Then, to get compilation flags run:

```
pkg-config --static --cflags wui-cpp_<name of library>
```

To get linker flags run:

```
pkg-config --static --libs wui-cpp_<name of library>
```




About authors
=====================

wui-cpp has been developped by following authors: 
+ Benjamin Navarro (LIRMM)

Please contact Benjamin Navarro (navarro@lirmm.fr) - LIRMM for more information or questions.



