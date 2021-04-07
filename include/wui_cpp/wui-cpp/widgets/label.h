#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <sstream>
#include <utility>

namespace wui {

class Label : public Widget {
public:
    template <typename ValueT>
    Label(const std::string& name, ValueT& value, std::string suffix = "",
          std::string prefix = "")
        : Widget(name), prefix_(std::move(prefix)), suffix_(std::move(suffix)) {
        getter_ = [this]() {
            std::stringstream ss;
            auto val = std::any_cast<ValueT>(internal_state_);
            ss << val;
            return ss.str();
        };

        update_ = [this, &value]() { internal_state_ = value; };

        update_();
    }

protected:
    void toJson(nlohmann::json& j) const override;

    std::any internal_state_;
    std::string prefix_;
    std::string suffix_;
};

} // namespace wui
