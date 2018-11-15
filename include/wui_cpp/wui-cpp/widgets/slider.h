#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>

namespace wui {

class Slider : public Widget {
public:
	template<typename ValueT>
	Slider(const std::string& name, ValueT& value, double min, double max) :
		Widget(name),
		min(min),
		max(max)
	{
		internal_state_ = value;
		getter = [this](){
					 return std::to_string(std::any_cast<ValueT>(internal_state_));
				 };
		setter = [this](const wui::property& p){
					 internal_state_ = p.get<ValueT>("value");
				 };
		update = [this, &value](){
					 value = std::any_cast<ValueT>(this->internal_state_);
				 };
	}

protected:
	virtual void toJson(nlohmann::json& j) const override;

	std::any internal_state_;
	double min;
	double max;
};

}
