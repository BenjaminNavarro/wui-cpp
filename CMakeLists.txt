CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
set(WORKSPACE_DIR ${CMAKE_SOURCE_DIR}/../.. CACHE PATH "root of the packages workspace directory")
list(APPEND CMAKE_MODULE_PATH ${WORKSPACE_DIR}/share/cmake/system) # using generic scripts/modules of the workspace
include(Package_Definition NO_POLICY_SCOPE)

PROJECT(wui-cpp)

declare_Package(
			"Benjamin Navarro"
			LIRMM
			navarro@lirmm.fr
			2018
			GNULGPL
			git@github.com:BenjaminNavarro/wui-cpp.git
            https://github.com/BenjaminNavarro/wui-cpp.git
			"An easy to use Web-based GUI for your C++ programs"
			instructions.md
		)

set_PID_Package_Version(0 1 0)

check_PID_Platform(CONFIGURATION posix)

declare_PID_Package_Dependency(PACKAGE pid-rpath NATIVE VERSION 2.0)
declare_PID_Package_Dependency(PACKAGE pid-os-utilities NATIVE VERSION 2.0)
declare_PID_Package_Dependency(PACKAGE simple-web-server EXTERNAL VERSION 3.0)
declare_PID_Package_Dependency(PACKAGE json EXTERNAL VERSION 3.4)

build_PID_Package()
