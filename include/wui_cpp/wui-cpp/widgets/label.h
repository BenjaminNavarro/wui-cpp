#pragma once

#include <wui-cpp/widgets/widget.h>
#include <any>
#include <sstream>
#include <utility>
#include <type_traits>

namespace wui {

namespace detail {
template <typename S, typename T, typename = void>
struct IsToStreamWritable : std::false_type {};

template <typename S, typename T>
struct IsToStreamWritable<
    S, T, std::void_t<decltype(std::declval<S&>() << std::declval<T>())>>
    : std::true_type {};

template <typename S, typename T>
inline constexpr bool is_to_stream_writable = IsToStreamWritable<S, T>::value;
} // namespace detail

class Label : public Widget {
public:
    template <typename ValueT>
    Label(const std::string& name, ValueT& value, std::string suffix = "",
          std::string prefix = "")
        : Widget(name), prefix_(std::move(prefix)), suffix_(std::move(suffix)) {
        getter_ = [this]() {
            std::stringstream ss;
            constexpr bool streamable =
                detail::is_to_stream_writable<decltype(ss), ValueT>;
            auto val = std::any_cast<ValueT>(internal_state_);
            if constexpr (std::is_enum_v<ValueT> and not streamable) {
                ss << static_cast<std::underlying_type_t<ValueT>>(val);
            } else if constexpr (streamable) {
                ss << val;
            } else {
                static_assert(streamable, "The provided type cannot be "
                                          "streamed to an std::stringstream");
            }
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
