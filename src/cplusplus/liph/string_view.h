// use std::string_view since c++17

#ifndef LIPH_STRING_VIEW_H_
#define LIPH_STRING_VIEW_H_

#include <ostream>
#include <stdexcept>
#include <string>

namespace liph {

class string_view {
public:
    constexpr string_view() noexcept {}
    constexpr string_view(const string_view& other) noexcept = default;
    constexpr string_view(const char *s, size_t count) : ptr_(s), size_(count) {}
    constexpr string_view(const char *s) : ptr_(s), size_(s == nullptr ? 0 : std::char_traits<char>::length(s)) {}
    string_view(const std::string& str) : ptr_(str.data()), size_(str.size()) {}

    constexpr const char *data() const noexcept { return ptr_; }
    constexpr size_t size() const noexcept { return size_; }
    constexpr size_t length() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr const char& operator[](size_t pos) const { return ptr_[pos]; }
    constexpr const char& at(size_t pos) const {
        if (pos >= size_) throw std::out_of_range("pos >= size()");
        return ptr_[pos];
    }

    constexpr string_view substr(size_t pos = 0, size_t count = npos) const {
        if (pos > size_) throw std::out_of_range("pos > size()");
        return string_view(ptr_ + pos, std::min(count, size_ - pos));
    }

public:
    static constexpr size_t npos = size_t(-1);

private:
    const char *ptr_{nullptr};
    size_t size_{0};
};

std::ostream& operator<<(std::ostream& os, string_view sv) {
    os.write(sv.data(), sv.size());
    return os;
}

}  // namespace liph

#endif  // LIPH_STRING_VIEW_H_
