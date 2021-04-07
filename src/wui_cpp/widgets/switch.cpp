#include <wui-cpp/widgets/switch.h>
#include <nlohmann/json.hpp>

namespace wui {

void Switch::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "switch";
    j["options"] = {{"default", getter_()}};
    j["container"] = container_;
}

} // namespace wui
