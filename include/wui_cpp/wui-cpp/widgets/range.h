#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <utility>

namespace wui {

class Range : public Widget {
public:
    template <typename ValueT>
    Range(const std::string& name, ValueT& lower_bound, ValueT& upper_bound,
          double min, double max)
        : Widget(name), min_(min), max_(max) {
        using pair_t = std::pair<ValueT, ValueT>;
        internal_state_ = std::make_pair(lower_bound, upper_bound);
        getter_ = [this]() {
            auto [internal_lower_bound, internal_upper_bound] =
                std::any_cast<pair_t>(internal_state_);
            return std::to_string(internal_lower_bound) + "/" +
                   std::to_string(internal_upper_bound);
        };
        setter_ = [this](const wui::property& p) {
            internal_state_ = std::make_pair(
                static_cast<ValueT>(
                    internal::readPropertyAsDouble(p, "lower_bound")),
                static_cast<ValueT>(
                    internal::readPropertyAsDouble(p, "upper_bound")));
        };
        update_ = [this, &lower_bound, &upper_bound]() {
            std::tie(lower_bound, upper_bound) =
                std::any_cast<pair_t>(internal_state_);
        };
    }

protected:
    void toJson(nlohmann::json& j) const override;

    std::any internal_state_;
    double min_;
    double max_;
};

} // namespace wui
