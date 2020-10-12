#include <nlohmann/json.hpp>
#include <wui-cpp/widgets/progress_bar.h>

namespace wui {

void ProgressBar::toJson(nlohmann::json& j) const {
    j["name"] = name;
    j["type"] = "progress_bar";
    j["container"] = container;
}

} // namespace wui
