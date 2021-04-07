#include <wui-cpp/widgets/range.h>
#include <nlohmann/json.hpp>

namespace wui {

void Range::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "range";
    j["options"] = {{"min", min_}, {"max", max_}, {"default", getter_()}};
    j["container"] = container_;
}

} // namespace wui
