#pragma once

#include <wui-cpp/widgets/widget_container.h>
#include <wui-cpp/server.h>
#include <list>

namespace wui {

class Tabs : public WidgetContainer {
public:
	Tabs(std::string name) :
		WidgetContainer(name)
	{
	}

	template<typename WidgetT, typename ... Args>
	std::shared_ptr<WidgetT> add(const std::string& tab_name, Args&& ... args) {
		auto widget_ptr = server_->add<WidgetT>(std::forward<Args>(args) ...);
		widget_ptr->setContainer(container + "/" + std::to_string(id) + "/" + tab_name);
		if(std::find(tabs_.begin(), tabs_.end(), tab_name) == tabs_.end()) {
			tabs_.push_back(tab_name);
		}
		return widget_ptr;
	}

protected:
	virtual void toJson(nlohmann::json& j) const override;

	std::list<std::string> tabs_;
};

}
