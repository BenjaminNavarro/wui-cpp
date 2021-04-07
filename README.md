
wui-cpp
==============

WUI-CPP is an easy to use Web-based GUI for your C++ programs



With a few lines of code, you can start a web server and describe the content of the user interface using the available widgets:
```cpp
#include <wui-cpp/wui.h>
#include "my_robot.h"

int main() {
	// Create a wui server on port 8080
	std::string web_root_path = "../share/resources/wui-cpp"
	wui::Server wui(web_root_path, 8080)

	double robot_max_velocity = 1.
	double robot_current_velocity = 0.
	std::string state = "Initialization"

	// Add some widgets
	wui.add<wui::Slider>("Robot maximum velocity", robot_max_velocity, 0., 10.)        // name, variable to pilot, min & max values
	wui.add<wui::Label> ("Robot maximum velocity", robot_max_velocity, "m/s")          // name, variable to display, suffix
	wui.add<wui::Label> ("Robot current velocity", robot_current_velocity, "m/s", "~") // name, variable to display, suffix, prefix
	wui.add<wui::Label> ("Current state", state)                                       // name, variable to display

	// Start the wui server asynchronously so that it serves requests in the background
	wui.start()

	while(isRobotOk()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10))

		robot_current_velocity = getRobotCurrentVelocity()
		state = getRobotState()

		// Update the GUI and program state.
		// For output widgets (e.g. Label): update their displayed value according to the local variables' state
		// For control widgets (e.g. Slider): update the local variables with the values set in the GUI
		wui.update()

		// Do some stuff
		setRobotMaxVelocity(robot_max_velocity)
	}
}
```

As you can see, `WUI-CPP` was designed to not bloat your code and to ask only the bare minimum from you:
 - create the server
 - add your widgets
 - start the server
 - call `update()` when you need to update your data

By running this code and accessing http://localhost:8080 from a web browser, you get the following interface:

![Example1 GUI](https://raw.githubusercontent.com/BenjaminNavarro/wui-cpp/integration/share/readme_content/wui-cpp_readme_1.gif)

Details
=======

The `Start` button will load the interface description from the server, add the corresponding widgets and start a timer at the period indicated in the spinbox (here 1000ms) to update the output widgets (here the `wui::Label`s in the table). The `Stop` button will cancel the timer and no more data will be retrieved from the server.

Input widgets, such as `wui::Slider`, will send an update every time their value changes and the server will store this information into its internal state.
When a call to `update()` is issued, variables associated with input widgets (here `robot_max_velocity`) will be updated with the server's internal state and variables associated with output widgets (here `robot_max_velocity`, `robot_current_velocity` and `state`) will be read and stored internally so that the interface can access these new values.

The graphical interface is based on [jQWidgets](https://www.jqwidgets.com/) and is free to use for non-commercial applications.

Available widgets
=================

For now, the following widgets are available:

| Name | Description | Type  |
|---|---|---|
| `wui::Label`  | Display anything having the output stream operator defined. All labels within the same container will be grouped inside a table  | Output  |
| `wui::ProgressBar`  | A progress bar working with anything having the output stream operator defined to output a numerical value | Output  |
|  `wui::Slider` | A slider to pilot a numerical value |  Input |
|  `wui::Range` | A "pair of sliders" to define a min/max interval |  Input |
|  `wui::Button` | A momentary push button |  Input |
|  `wui::Switch` | An on/off switch |  Input |
|  `wui::Tabs` | Add Tabs to better organize your interface |  Container |

Here is the interface created by the `wui-example` demo application:

![Example2 GUI](https://raw.githubusercontent.com/BenjaminNavarro/wui-cpp/integration/share/readme_content/wui-cpp_readme_2.gif)

What is and is not WUI-CPP
==========================

`WUI-CPP` is not aimed to replace `Qt`, `wxWidgets`, `gtk` and alike where you can create complex and good looking applications. It will never be able to compete with such solutions.

`WUI-CPP` is for those who want a simple solution to add graphical capabilities to their C++ applications and/or those who need a remote access to their GUI. Being web-based, the user interface can be accessed from computers, tablets and phones, with no additional cost!

The future plans for WUI-CPP
============================
 - Write the documentation
 - Add more widgets
 - Make real time plotting of `wui::Label`s possible


Package Overview
================

The **wui-cpp** package contains the following:

 * Libraries:

   * wui-cpp (shared)

   * wui-cpp-st (static)

 * Examples:

   * wui-example


Installation and Usage
======================

The **wui-cpp** project is packaged using [PID](http://pid.lirmm.net), a build and deployment system based on CMake.

If you wish to adopt PID for your develoment please first follow the installation procedure [here](http://pid.lirmm.net/pid-framework/pages/install.html).

If you already are a PID user or wish to integrate **wui-cpp** in your current build system, please read the appropriate section below.


## Using an existing PID workspace

This method is for developers who want to install and access **wui-cpp** from their PID workspace.

You can use the `deploy` command to manually install **wui-cpp** in the workspace:
```
cd <path to pid workspace>
pid deploy package=wui-cpp # latest version
# OR
pid deploy package=wui-cpp version=x.y.z # specific version
```
Alternatively you can simply declare a dependency to **wui-cpp** in your package's `CMakeLists.txt` and let PID handle everything:
```
PID_Dependency(wui-cpp) # any version
# OR
PID_Dependency(wui-cpp VERSION x.y.z) # any version compatible with x.y.z
```

If you need more control over your dependency declaration, please look at [PID_Dependency](https://pid.lirmm.net/pid-framework/assets/apidoc/html/pages/Package_API.html#pid-dependency) documentation.

Once the package dependency has been added, you can use the following components as component dependencies:
 * `wui-cpp/wui-cpp`
 * `wui-cpp/wui-cpp-st`

You can read [PID_Component](https://pid.lirmm.net/pid-framework/assets/apidoc/html/pages/Package_API.html#pid-component) and [PID_Component_Dependency](https://pid.lirmm.net/pid-framework/assets/apidoc/html/pages/Package_API.html#pid-component-dependency) documentations for more details.
## Standalone installation

This method allows to build the package without having to create a PID workspace manually. This method is UNIX only.

All you need to do is to first clone the package locally and then run the installation script:
 ```
git clone https://gite.lirmm.fr/pid/gui/wui-cpp.git
cd wui-cpp
./share/install/standalone_install.sh
```
The package as well as its dependencies will be deployed under `binaries/pid-workspace`.

You can pass `--help` to the script to list the available options.

### Using **wui-cpp** in a CMake project
There are two ways to integrate **wui-cpp** in CMake project: the external API or a system install.

The first one doesn't require the installation of files outside of the package itself and so is well suited when used as a Git submodule for example.
Please read [this page](https://pid.lirmm.net/pid-framework/pages/external_API_tutorial.html#using-cmake) for more information.

The second option is more traditional as it installs the package and its dependencies in a given system folder which can then be retrived using `find_package(wui-cpp)`.
You can pass the `--install <path>` option to the installation script to perform the installation and then follow [these steps](https://pid.lirmm.net/pid-framework/pages/external_API_tutorial.html#third-step--extra-system-configuration-required) to configure your environment, find PID packages and link with their components.
### Using **wui-cpp** with pkg-config
You can pass `--pkg-config on` to the installation script to generate the necessary pkg-config files.
Upon completion, the script will tell you how to set the `PKG_CONFIG_PATH` environment variable for **wui-cpp** to be discoverable.

Then, to get the necessary compilation flags run:

```
pkg-config --static --cflags wui-cpp_<component>
```

```
pkg-config --variable=c_standard wui-cpp_<component>
```

```
pkg-config --variable=cxx_standard wui-cpp_<component>
```

To get the linker flags run:

```
pkg-config --static --libs wui-cpp_<component>
```

Where `<component>` is one of:
 * `wui-cpp`
 * `wui-cpp-st`


# Online Documentaion
**wui-cpp** documentation is available [online](https://pid.lirmm.net/pid-framework/packages/wui-cpp).
You can find:


Offline API Documentation
=========================

With [Doxygen](https://www.doxygen.nl) installed, the API documentation can be built locally by turning the `BUILD_API_DOC` CMake option `ON` and running the `doc` target, e.g
```
pid cd wui-cpp
pid -DBUILD_API_DOC=ON doc
```
The resulting documentation can be accessed by opening `<path to wui-cpp>/build/release/share/doc/html/index.html` in a web browser.

License
=======

The license that applies to the whole package content is **GNULGPL**. Please look at the [license.txt](./license.txt) file at the root of this repository for more details.

Authors
=======

**wui-cpp** has been developed by the following authors: 
+ Benjamin Navarro (LIRMM / CNRS)

Please contact Benjamin Navarro (navarro@lirmm.fr) - LIRMM / CNRS for more information or questions.
