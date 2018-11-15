#pragma once

#include <boost/property_tree/ptree.hpp>
#include <nlohmann/json_fwd.hpp>

#include <string>
#include <functional>

namespace wui {

using property = boost::property_tree::ptree;

class Server;
class Widget;

void to_json(nlohmann::json& j, const Widget& widget);

class Widget {
public:
	Widget(std::string name);

	void setContainer(const std::string& container);

protected:
	friend class Server;
	friend void to_json(nlohmann::json& j, const Widget& widget);

	int id;
	std::string name;
	std::string container;

	std::function<std::string(void)> getter;
	std::function<void(const property&)> setter;
	std::function<void(void)> update;

	virtual void toJson(nlohmann::json& j) const = 0;
};

using WidgetPtr = std::shared_ptr<Widget>;

}
