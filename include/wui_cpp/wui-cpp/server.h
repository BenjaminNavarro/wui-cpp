#pragma once

#include <wui-cpp/widgets/widget_container.h>

#include <simple-web-server/server_http.hpp>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

namespace wui {

class Server {
public:
	Server(int port, const std::string& address = "");
	virtual ~Server();

	void startSync();
	void startAsync();
	void stop();
	void update();

	template<typename WidgetT, typename ... Args>
	std::shared_ptr<WidgetT> add(Args&& ... args) {
		auto widget_ptr = std::make_shared<WidgetT>(std::forward<Args>(args) ...);
		if constexpr(std::is_base_of_v<WidgetContainer, WidgetT>) {
			widget_ptr->server_ = this;
		}
		widgets_.push_back(widget_ptr);
		widget_ptr->id = widgets_.size()-1;
		return widget_ptr;
	}

private:
	SimpleWeb::Server<SimpleWeb::HTTP> server_;
	std::thread server_asyn_thread_;

	std::vector<WidgetPtr> widgets_;
	std::mutex update_mutex_;
};

}
