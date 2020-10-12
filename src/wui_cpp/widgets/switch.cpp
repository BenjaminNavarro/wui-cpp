#include <wui-cpp/widgets/switch.h>
#include <nlohmann/json.hpp>

namespace wui {

void Switch::toJson(nlohmann::json& j) const {
    j["name"] = name;
    j["type"] = "switch";
    j["container"] = container;
}

} // namespace wui
