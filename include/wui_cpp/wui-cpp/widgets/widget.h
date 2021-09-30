#pragma once

#include <nlohmann/json_fwd.hpp>

#include <string>
#include <functional>

namespace wui {

using property = nlohmann::json;

class Server;
class Widget;

// NOLINTNEXTLINE(readability-identifier-naming)
void to_json(nlohmann::json& j, const Widget& widget);

class Widget {
public:
    Widget(std::string name);
    virtual ~Widget() = default;

    void setContainer(const std::string& container);

protected:
    friend class Server;
    friend void to_json(nlohmann::json& j, const Widget& widget);

    int id_;
    std::string name_;
    std::string container_;

    std::function<std::string(void)> getter_;
    std::function<void(const property&)> setter_;
    std::function<void(void)> update_;

    virtual void toJson(nlohmann::json& j) const = 0;
};

using WidgetPtr = std::shared_ptr<Widget>;

namespace internal {
double readPropertyAsDouble(const property& p, const std::string& key);
bool readPropertyAsBool(const property& p, const std::string& key);
int readPropertyAsInt(const property& p, const std::string& key);
} // namespace internal

} // namespace wui
