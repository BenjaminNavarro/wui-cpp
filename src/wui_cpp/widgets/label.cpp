#include <wui-cpp/widgets/label.h>
#include <nlohmann/json.hpp>

namespace wui {

void Label::toJson(nlohmann::json& j) const {
    j["name"] = name;
    j["type"] = "label";
    j["options"] = {{"prefix", prefix}, {"suffix", suffix}};
    j["container"] = container;
}

} // namespace wui
