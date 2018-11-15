#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <utility>

namespace wui {

class Range : public Widget {
public:
	template<typename ValueT>
	Range(const std::string& name, ValueT& lower_bound, ValueT& upper_bound, double min, double max) :
		Widget(name),
		min(min),
		max(max)
	{
		using pair_t = std::pair<ValueT, ValueT>;
		internal_state_ = std::make_pair(lower_bound, upper_bound);
		getter = [this](){
					 auto [lower_bound, upper_bound] = std::any_cast<pair_t>(internal_state_);
					 return std::to_string(lower_bound) + "/" + std::to_string(upper_bound);
				 };
		setter = [this](const wui::property& p){
					 internal_state_ = std::make_pair(p.get<ValueT>("lower_bound"), p.get<ValueT>("upper_bound"));
				 };
		update = [this, &lower_bound, &upper_bound](){
					 std::tie(lower_bound, upper_bound) = std::any_cast<pair_t>(internal_state_);
				 };
	}

protected:
	virtual void toJson(nlohmann::json& j) const override;

	std::any internal_state_;
	double min;
	double max;
};

}
