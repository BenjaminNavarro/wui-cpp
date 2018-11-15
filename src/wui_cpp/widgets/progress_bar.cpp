#include <wui-cpp/widgets/progress_bar.h>
#include <nlohmann/json.hpp>

namespace wui {

void ProgressBar::toJson(nlohmann::json& j) const {
	j["name"] = name;
	j["type"] = "progress_bar";
	j["container"] = container;
}


}
