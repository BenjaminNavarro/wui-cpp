declare_PID_Component(
    SHARED_LIB
    NAME wui-cpp
    DIRECTORY wui_cpp
    CXX_STANDARD 17
    RUNTIME_RESOURCES wui-cpp
    DEPENDS rpathlib
    EXPORT simple-web-server json
    EXPORTED LINKS ${posix_LINK_OPTIONS}
)

# declare_PID_Component_Dependency(
#     COMPONENT wui-cpp
#     EXPORT EXTERNAL simple-web-server
#     PACKAGE simple-web-server
# )
#
# declare_PID_Component_Dependency(
#     COMPONENT wui-cpp
#     NATIVE rpathlib
#     PACKAGE pid-rpath
# )
#
# declare_PID_Component_Dependency(
# 	COMPONENT wui-cpp
# 	EXPORT LINKS SHARED ${posix_LINK_OPTIONS}
# )
