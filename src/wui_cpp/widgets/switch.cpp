#include <wui-cpp/widgets/switch.h>
#include <nlohmann/json.hpp>

namespace wui {

void Switch::toJson(nlohmann::json& j) const {
    j["name"] = name;
    j["type"] = "switch";
    j["options"] = {{"default", getter()}};
    j["container"] = container;
}

} // namespace wui
