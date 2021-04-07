#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <sstream>

namespace wui {

class ProgressBar : public Widget {
public:
    template <typename ValueT>
    ProgressBar(const std::string& name, ValueT& value, double scale = 1.)
        : Widget(name) {
        getter_ = [this]() {
            std::stringstream ss;
            auto val = std::any_cast<ValueT>(internal_state_);
            ss << val;
            return ss.str();
        };

        update_ = [this, &value, scale]() { internal_state_ = value * scale; };

        update_();
    }

protected:
    void toJson(nlohmann::json& j) const override;

    std::any internal_state_;
};

} // namespace wui
