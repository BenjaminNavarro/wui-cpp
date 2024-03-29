With a few lines of code, you can start a web server and describe the content of the user interface using the available widgets:
```cpp
#include <wui-cpp/wui.h>
#include "my_robot.h"

int main() {
	// Create a wui server on port 8080
	std::string web_root_path = "../share/resources/wui-cpp";
	wui::Server wui(web_root_path, 8080);

	double robot_max_velocity = 1.;
	double robot_current_velocity = 0.;
	std::string state = "Initialization";

	// Add some widgets
	wui.add<wui::Slider>("Robot maximum velocity", robot_max_velocity, 0., 10.);        // name, variable to pilot, min & max values
	wui.add<wui::Label> ("Robot maximum velocity", robot_max_velocity, "m/s");          // name, variable to display, suffix
	wui.add<wui::Label> ("Robot current velocity", robot_current_velocity, "m/s", "~"); // name, variable to display, suffix, prefix
	wui.add<wui::Label> ("Current state", state);                                       // name, variable to display

	// Start the wui server asynchronously so that it serves requests in the background
	wui.start();

	while(isRobotOk()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		robot_current_velocity = getRobotCurrentVelocity();
		state = getRobotState();

		// Update the GUI and program state.
		// For output widgets (e.g. Label): update their displayed value according to the local variables' state
		// For control widgets (e.g. Slider): update the local variables with the values set in the GUI
		wui.update();

		// Do some stuff
		setRobotMaxVelocity(robot_max_velocity);
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
|  `wui::ComboBox` | A drop down list of values |  Input |
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
