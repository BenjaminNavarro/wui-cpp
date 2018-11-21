declare_PID_Component(
    SHARED_LIB
    NAME wui-cpp
    DIRECTORY wui_cpp
    CXX_STANDARD 17
    RUNTIME_RESOURCES wui-cpp
    EXPORT simple-web-server json
    EXPORTED LINKS -lstdc++fs
)
