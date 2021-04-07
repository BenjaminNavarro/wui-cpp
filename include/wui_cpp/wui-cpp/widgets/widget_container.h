#pragma once

#include <wui-cpp/widgets/widget.h>
#include <list>
#include <utility>

namespace wui {

class WidgetContainer : public Widget {
public:
    WidgetContainer(std::string name) : Widget(std::move(name)) {
    }

protected:
    friend class Server;
    Server* server_;
};

} // namespace wui
