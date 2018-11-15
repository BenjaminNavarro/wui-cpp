declare_PID_Component(
    EXAMPLE_APPLICATION
    NAME wui-tests
    DIRECTORY wui-tests
)


declare_PID_Component_Dependency(
    COMPONENT wui-tests
    NATIVE wui-cpp
)

declare_PID_Component_Dependency(
    COMPONENT wui-tests
    NATIVE pid-signal-manager
    PACKAGE pid-os-utilities
)
