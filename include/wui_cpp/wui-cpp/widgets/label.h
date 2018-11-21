#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <sstream>

namespace wui {

class Label : public Widget {
public:
	template<typename ValueT>
	Label(const std::string& name, ValueT& value, const std::string& suffix = "", const std::string& prefix = "") :
		Widget(name),
		prefix(prefix),
		suffix(suffix)
	{
		getter = [this](){
					 std::stringstream ss;
					 auto val = std::any_cast<ValueT>(internal_state_);
					 ss << val;
					 return ss.str();
				 };

		update = [this, &value](){
					 internal_state_ = value;
				 };

		update();
	}

protected:
	virtual void toJson(nlohmann::json& j) const override;

	std::any internal_state_;
	std::string prefix;
	std::string suffix;
};

}
