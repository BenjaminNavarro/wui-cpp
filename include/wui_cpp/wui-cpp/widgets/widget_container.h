#pragma once

#include <wui-cpp/widgets/widget.h>
#include <list>

namespace wui {

class WidgetContainer : public Widget {
public:
	WidgetContainer(std::string name) :
		Widget(name)
	{
	}


protected:
	friend class Server;
	Server* server_;
};

}
