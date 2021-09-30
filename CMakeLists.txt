CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
set(WORKSPACE_DIR ${CMAKE_SOURCE_DIR}/../.. CACHE PATH "root of the packages workspace directory")
list(APPEND CMAKE_MODULE_PATH ${WORKSPACE_DIR}/share/cmake/system) # using generic scripts/modules of the workspace
include(Package_Definition NO_POLICY_SCOPE)

PROJECT(wui-cpp)

PID_Package(
    AUTHOR             Benjamin Navarro
    INSTITUTION        LIRMM / CNRS
    MAIL               navarro@lirmm.fr
    YEAR               2018-2021
    LICENSE            GNULGPL
    ADDRESS            git@gite.lirmm.fr:pid/gui/wui-cpp.git
    PUBLIC_ADDRESS     https://gite.lirmm.fr/pid/gui/wui-cpp.git
    CONTRIBUTION_SPACE pid
    DESCRIPTION        "WUI-CPP is an easy to use Web-based GUI for your C++ programs"
    README             instructions.md
    CODE_STYLE         pid11
    VERSION            1.2.0
)

check_PID_Environment(TOOL conventional_commits)
check_PID_Environment(LANGUAGE CXX[std=17])

PID_Dependency(simple-web-server VERSION 3.0)
PID_Dependency(json FROM VERSION 3.9.1)

PID_Publishing(
    PROJECT https://gite.lirmm.fr/pid/gui/wui-cpp
    FRAMEWORK pid
    CATEGORIES programming/gui
    DESCRIPTION WUI-CPP is an easy to use Web-based GUI for your C++ programs
    PUBLISH_BINARIES 
    PUBLISH_DEVELOPMENT_INFO
    ALLOWED_PLATFORMS 
        x86_64_linux_stdc++11__ub18_gcc9__ 
        x86_64_linux_stdc++11__ub20_gcc9__
        x86_64_linux_stdc++11__arch_gcc__
)

build_PID_Package()
