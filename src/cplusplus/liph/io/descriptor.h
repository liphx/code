#ifndef LIPH_IO_DESCRIPTOR_H_
#define LIPH_IO_DESCRIPTOR_H_

#include <unistd.h>

namespace liph::io {

enum class descriptor_type { none, regular, socket };

class descriptor {
public:
    static constexpr int invalid = -1;

public:
    descriptor(int fd = invalid, descriptor_type type = descriptor_type::none) : fd_(fd), type_(type) {}

    virtual ~descriptor() {
        if (operator bool()) ::close(fd_);
    }

    int fd() const { return fd_; }

    descriptor_type type() const { return type_; }

    operator bool() const { return fd_ != invalid; };

    ssize_t read(void *buf, size_t nbytes) { return ::read(fd_, buf, nbytes); }

    ssize_t write(const void *buf, size_t nbytes) { return ::write(fd_, buf, nbytes); }

protected:
    int fd_;
    descriptor_type type_;
};

}  // namespace liph::io

#endif  // LIPH_IO_DESCRIPTOR_H_
