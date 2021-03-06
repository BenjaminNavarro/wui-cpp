#include <wui-cpp/widgets/tabs.h>
#include <nlohmann/json.hpp>

namespace wui {

void Tabs::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "tabs";
    j["tabs"] = tabs_;
    j["container"] = container_;
}

} // namespace wui
