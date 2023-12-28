#ifndef LIPH_LANG_FINALLY_H_
#define LIPH_LANG_FINALLY_H_

#include <utility>

namespace liph {

template <class F>
class final_action {
public:
    explicit final_action(const F& ff) noexcept : f{ff} {}
    explicit final_action(F&& ff) noexcept : f{std::move(ff)} {}
    ~final_action() noexcept {
        if (invoke) f();
    }
    final_action(final_action&& other) noexcept : f(std::move(other.f)), invoke(std::exchange(other.invoke, false)) {}
    final_action(const final_action&) = delete;
    void operator=(const final_action&) = delete;
    void operator=(final_action&&) = delete;

private:
    F f;
    bool invoke = true;
};

template <class F>
[[nodiscard]] auto finally(F&& f) noexcept {
    return final_action<std::decay_t<F>>{std::forward<F>(f)};
}

}  // namespace liph

#endif  // LIPH_LANG_FINALLY_H_
