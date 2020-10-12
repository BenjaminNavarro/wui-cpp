CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
set(WORKSPACE_DIR ${CMAKE_SOURCE_DIR}/../.. CACHE PATH "root of the packages workspace directory")
list(APPEND CMAKE_MODULE_PATH ${WORKSPACE_DIR}/share/cmake/system) # using generic scripts/modules of the workspace
include(Package_Definition NO_POLICY_SCOPE)

PROJECT(wui-cpp)

PID_Package(
    AUTHOR          "Benjamin Navarro"
    INSTITUTION     LIRMM
    MAIL            navarro@lirmm.fr
    YEAR            2018-2020
    LICENSE         GNULGPL
    ADDRESS         git@github.com:BenjaminNavarro/wui-cpp.git
    PUBLIC_ADDRESS  https://github.com/BenjaminNavarro/wui-cpp.git
    DESCRIPTION     "WUI-CPP is an easy to use Web-based GUI for your C++ programs."
    README          instructions.md
    CODE_STYLE      pid11
    VERSION         0.5.1
)

PID_Dependency(simple-web-server VERSION 3.0)
PID_Dependency(json FROM VERSION 3.9.1)

build_PID_Package()
