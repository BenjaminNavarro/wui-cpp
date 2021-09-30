#include <wui-cpp/widgets/combobox.h>
#include <nlohmann/json.hpp>

namespace wui {

void ComboBox::toJson(nlohmann::json& j) const {
    j["name"] = name_;
    j["type"] = "combobox";
    j["container"] = container_;
    j["options"]["entries"] = entries_;
    j["options"]["default"] = current_index_;
}

} // namespace wui
