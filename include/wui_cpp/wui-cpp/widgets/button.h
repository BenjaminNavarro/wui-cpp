#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>

namespace wui {

class Button : public Widget {
public:
    Button(const std::string& name, bool& state) : Widget(name) {
        internal_state_ = state;
        getter = [this]() { return std::to_string(internal_state_); };
        setter = [this](const wui::property& p) {
            internal_state_ = internal::readPropertyAsBool(p, "state");
        };
        update = [this, &state]() {
            state = internal_state_;
            internal_state_ = false;
        };
    }

protected:
    virtual void toJson(nlohmann::json& j) const override;

    bool internal_state_;
};

} // namespace wui
