#include <wui-cpp/widgets/widget.h>
#include <nlohmann/json.hpp>
#include <utility>

namespace wui {
void to_json(nlohmann::json& j, const Widget& widget) {
    widget.toJson(j);
}

Widget::Widget(std::string name) : name_(std::move(name)) {
    update_ = []() {};
}

void Widget::setContainer(const std::string& container) {
    this->container_ = container;
}

namespace internal {
double readPropertyAsDouble(const property& p, const std::string& key) {
    return p[key].get<double>();
}
bool readPropertyAsBool(const property& p, const std::string& key) {
    return p[key].get<bool>();
}
} // namespace internal

} // namespace wui
