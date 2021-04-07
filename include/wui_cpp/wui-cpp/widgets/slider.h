#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>

namespace wui {

class Slider : public Widget {
public:
    template <typename ValueT>
    Slider(const std::string& name, ValueT& value, double min, double max)
        : Widget(name), min_(min), max_(max) {
        internal_state_ = value;
        getter_ = [this]() {
            return std::to_string(std::any_cast<ValueT>(internal_state_));
        };
        setter_ = [this](const wui::property& p) {
            internal_state_ =
                static_cast<ValueT>(internal::readPropertyAsDouble(p, "value"));
        };
        update_ = [this, &value]() {
            value = std::any_cast<ValueT>(this->internal_state_);
        };
    }

protected:
    void toJson(nlohmann::json& j) const override;

    std::any internal_state_;
    double min_;
    double max_;
};

} // namespace wui
