#include <wui-cpp/widgets/label.h>
#include <nlohmann/json.hpp>

namespace wui {

void Label::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "label";
    j["options"] = {{"prefix", prefix_}, {"suffix", suffix_}};
    j["container"] = container_;
}

} // namespace wui
