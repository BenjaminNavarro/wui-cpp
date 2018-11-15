#include <wui-cpp/widgets/button.h>
#include <nlohmann/json.hpp>

namespace wui {

void Button::toJson(nlohmann::json& j) const {
	j["name"] = name;
	j["type"] = "button";
	j["container"] = container;
}

}
