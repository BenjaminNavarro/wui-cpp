#include <wui-cpp/wui.h>
#include <pid/signal_manager.h>

#include <ostream>
#include <chrono>

// Custom data type with an output stream operator
struct Point2D {
	double x;
	double y;
};

std::ostream& operator<< (std::ostream &os, const Point2D& pt) {
	os << "(" << pt.x << "," << pt.y << ")";
	return os;
}

int main(int argc, char const *argv[]) {
	// Create a server on port 8080
	wui::Server server(8080);

	// Program variables that need to be read/write from the UI
	double velocity = 0.;
	double acceleration = 0.;
	std::string state = "Initialization";
	Point2D point{12,42};
	double p_gain = 20.;
	double i_gain = 0.1;
	double d_gain = 2.5;
	int size = 12;
	auto bounds = std::make_pair<int,int>(-10., 10.);
	double progress = 0.;
	bool reset = false;
	bool stop_progress = false;

	// Add some widgets in the root container
	server.add<wui::Slider>     ("Size",            size,           10,             20);            // Name, reference, min, max
	server.add<wui::Range>      ("Bounds",          bounds.first,   bounds.second,  -20.,   20.f);  // Name, lower bound reference, upper bound reference, min, max
	server.add<wui::ProgressBar>("Progress",        progress,       100.);                          // Name, reference, scale (progress bar = reference * scale). Here 0 <= progress <= 1
	server.add<wui::Button>     ("Reset",           reset);
	server.add<wui::Switch>     ("Stop",            stop_progress);
	server.add<wui::Label>      ("current size",    size,           "mm");                          // Name, reference, suffix
	server.add<wui::Label>      ("Point",           point);

	// Create a Tab container inside the root container
	auto tabs = server.add<wui::Tabs>("tabs");
	// Add widgets inside the tabs
	tabs->add<wui::Slider>  ("Gains", "P gain",       p_gain,         0.,     100.);    // Tab name, Name, reference, min, max
	tabs->add<wui::Slider>  ("Gains", "I gain",       i_gain,         0.,     1.);
	tabs->add<wui::Slider>  ("Gains", "D gain",       d_gain,         0.,     10.);
	tabs->add<wui::Label>   ("State", "Velocity",     velocity,       "m/s",  "~");     // Tab name, Name, reference, suffix, prefix
	tabs->add<wui::Label>   ("State", "Acceleration", acceleration,   "m/s²");
	tabs->add<wui::Label>   ("State", "State",        state);

	// Start the server asynchronously so that it serves requests in the background
	server.startAsync();

	// Register a callback to exit the main loop en CTRL-C
	bool stop = false;
	pid::SignalManager::registerCallback(pid::SignalManager::Interrupt, "stop", [&stop](int){stop = true;});

	while(not stop) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		// Update the GUI and program state.
		// For control widgets (e.g. Slider): update the local variables with the values set in the GUI
		// For Label widgets: update their displayed value according to the local variables' state
		server.update();

		// Do some stuff
		velocity = static_cast<double>(std::rand())/RAND_MAX;
		acceleration = static_cast<double>(std::rand())/RAND_MAX;

		if(not stop_progress) {
			progress += 0.0001;
		}
		if(progress > 1. or reset) {
			progress = 0.;
			reset = false;
		}

		std::cout << "\rp_gain: " << p_gain << "\ti_gain: " << i_gain << "\td_gain: " << d_gain;
		std::cout << "\tlow: " << bounds.first << "\thigh: " << bounds.second << "         " << std::flush;
	}
	std::cout << std::endl;

	// Unregister CTRL-C
	pid::SignalManager::unregisterCallback(pid::SignalManager::Interrupt, "stop");

	server.stop();

	return 0;
}
