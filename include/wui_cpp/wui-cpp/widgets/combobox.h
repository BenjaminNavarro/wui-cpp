#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <cassert>

namespace wui {

class ComboBox : public Widget {
public:
    ComboBox(const std::string& name, std::string& selected_value,
             std::vector<std::string> entries,
             const std::string& default_value = "")
        : Widget(name), entries_{std::move(entries)} {

        setup(default_value);

        update_ = [this, &selected_value]() {
            selected_value = entries_.at(current_index_);
        };
    }

    ComboBox(const std::string& name, size_t& selected_index,
             std::vector<std::string> entries,
             const std::string& default_value = "")
        : Widget(name), entries_{std::move(entries)} {

        setup(default_value);

        update_ = [this, &selected_index]() {
            assert(selected_index < entries_.size());
            selected_index = current_index_;
        };
    }

protected:
    void toJson(nlohmann::json& j) const override;

    void setup(const std::string& default_value) {
        if (auto idx = indexOf(default_value); idx >= 0) {
            current_index_ = static_cast<size_t>(idx);
        } else {
            current_index_ = 0;
        }

        getter_ = [this]() { return std::to_string(current_index_); };
        setter_ = [this](const wui::property& p) {
            if (auto idx = internal::readPropertyAsInt(p, "index"); idx >= 0) {
                current_index_ = static_cast<size_t>(idx);
            }
        };
    }

    ssize_t indexOf(std::string_view entry) {
        if (auto it =
                std::find_if(begin(entries_), end(entries_),
                             [entry](const auto& str) { return str == entry; });
            it != end(entries_)) {
            return std::distance(begin(entries_), it);
        } else {
            return -1;
        }
    }

    std::vector<std::string> entries_;
    size_t current_index_;
};

} // namespace wui
