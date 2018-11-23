function(create_wui_component type name)
    PID_Component(
        ${type}
        NAME ${name}
        DIRECTORY wui_cpp
        CXX_STANDARD 17
        RUNTIME_RESOURCES wui-cpp
        EXPORT simple-web-server json
        EXPORTED LINKS -lstdc++fs
    )
endfunction()

create_wui_component(SHARED_LIB wui-cpp)
create_wui_component(STATIC_LIB wui-cpp-st)
