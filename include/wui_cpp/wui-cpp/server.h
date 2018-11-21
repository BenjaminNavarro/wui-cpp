#pragma once

#include <wui-cpp/widgets/widget_container.h>

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>

namespace wui {

class Server {
public:
	Server(const std::string& root_path, int port, const std::string& address = "");
	virtual ~Server();

	void start();
	void stop();
	void update();

	template<typename WidgetT, typename ... Args>
	std::shared_ptr<WidgetT> add(Args&& ... args) {
		auto widget_ptr = std::make_shared<WidgetT>(std::forward<Args>(args) ...);
		if constexpr(std::is_base_of_v<WidgetContainer, WidgetT>) {
			widget_ptr->server_ = this;
		}
		addWidget(widget_ptr);
		return widget_ptr;
	}

private:
	struct pImpl;
	std::unique_ptr<pImpl> impl_;

	void addWidget(WidgetPtr widget);
};

}
