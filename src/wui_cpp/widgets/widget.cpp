#include <wui-cpp/widgets/widget.h>
#include <nlohmann/json.hpp>

namespace wui {
void to_json(nlohmann::json& j, const Widget& widget) {
	widget.toJson(j);
}

Widget::Widget(std::string name) :
	name(name)
{
	update = [](){};
}

void Widget::setContainer(const std::string& container) {
	this->container = container;
}

}
