#include <wui-cpp/widgets/slider.h>
#include <nlohmann/json.hpp>

namespace wui {

void Slider::toJson(nlohmann::json& j) const {
    j["name"] = name;
    j["type"] = "slider";
    j["options"] = {{"min", min}, {"max", max}, {"default", getter()}};
    j["container"] = container;
}

} // namespace wui
