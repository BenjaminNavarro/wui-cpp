#include <wui-cpp/widgets/slider.h>
#include <nlohmann/json.hpp>

namespace wui {

void Slider::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "slider";
    j["options"] = {{"min", min_}, {"max", max_}, {"default", getter_()}};
    j["container"] = container_;
}

} // namespace wui
