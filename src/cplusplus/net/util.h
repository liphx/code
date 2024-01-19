#pragma once

#include <functional>

namespace liph::net {

inline std::string addr2ip(uint32_t addr) {
    auto p = reinterpret_cast<uint8_t *>(&addr);
    std::string ret = std::to_string(static_cast<uint32_t>(*p++)) + '.';
    ret += std::to_string(static_cast<uint32_t>(*p++)) + '.';
    ret += std::to_string(static_cast<uint32_t>(*p++)) + '.';
    ret += std::to_string(static_cast<uint32_t>(*p));
    return ret;
}

template <typename signature>
struct make_copyable_function_helper;
template <typename R, typename... Args>
struct make_copyable_function_helper<R(Args...)> {
    template <typename input>
    std::function<R(Args...)> operator()(input&& i) const {
        auto ptr = std::make_shared<typename std::decay<input>::type>(std::forward<input>(i));
        return [ptr](Args... args) -> R { return (*ptr)(std::forward<Args>(args)...); };
    }
};

template <typename signature, typename input>
std::function<signature> make_copyable_function(input&& i) {
    return make_copyable_function_helper<signature>()(std::forward<input>(i));
}

}  // namespace liph::net
