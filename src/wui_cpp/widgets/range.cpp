#include <wui-cpp/widgets/range.h>
#include <nlohmann/json.hpp>

namespace wui {

void Range::toJson(nlohmann::json& j) const {
	j["name"] = name;
	j["type"] = "range";
	j["options"] = { {"min", min}, {"max", max}, {"default", getter()} };
	j["container"] = container;
}

}
