#include <wui-cpp/widgets/button.h>
#include <nlohmann/json.hpp>

namespace wui {

void Button::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "button";
    j["container"] = container_;
}

} // namespace wui
