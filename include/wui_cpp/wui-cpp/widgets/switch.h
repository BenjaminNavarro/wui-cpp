#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>

namespace wui {

class Switch : public Widget {
public:
    Switch(const std::string& name, bool& state) : Widget(name) {
        internal_state_ = state;
        getter_ = [this]() {
            return std::to_string(static_cast<int>(internal_state_));
        };
        setter_ = [this](const wui::property& p) {
            internal_state_ = internal::readPropertyAsBool(p, "state");
        };
        update_ = [this, &state]() { state = internal_state_; };
    }

protected:
    void toJson(nlohmann::json& j) const override;

    bool internal_state_;
};

} // namespace wui
