CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
set(WORKSPACE_DIR ${CMAKE_SOURCE_DIR}/../.. CACHE PATH "root of the packages workspace directory")
list(APPEND CMAKE_MODULE_PATH ${WORKSPACE_DIR}/share/cmake/system) # using generic scripts/modules of the workspace
include(Package_Definition NO_POLICY_SCOPE)

PROJECT(wui-cpp)

declare_PID_Package(
			AUTHOR 			"Benjamin Navarro"
			INSTITUTION		LIRMM
			MAIL			navarro@lirmm.fr
			YEAR			2018
			LICENSE			GNULGPL
			ADDRESS			git@github.com:BenjaminNavarro/wui-cpp.git
            PUBLIC_ADDRESS	https://github.com/BenjaminNavarro/wui-cpp.git
			DESCRIPTION		"WUI-CPP is an easy to use Web-based GUI for your C++ programs."
			README			instructions.md
			VERSION			0.5.0
		)

declare_PID_Package_Dependency(PACKAGE simple-web-server EXTERNAL VERSION 3.0)
declare_PID_Package_Dependency(PACKAGE json EXTERNAL VERSION 3.4)

build_PID_Package()
