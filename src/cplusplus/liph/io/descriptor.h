#ifndef LIPH_IO_DESCRIPTOR_H_
#define LIPH_IO_DESCRIPTOR_H_

#include <unistd.h>

namespace liph::io {

enum class descriptor_type { none, regular, socket };

class descriptor {
public:
    descriptor(int fd = -1, descriptor_type type = descriptor_type::none) : fd_(fd), type_(type) {}
    virtual ~descriptor() {}

    int fd() const { return fd_; }
    descriptor_type type() const { return type_; }
    operator bool() const { return fd_ != -1; };

    ssize_t read(void *buf, size_t nbytes) { return ::read(fd_, buf, nbytes); }
    ssize_t write(const void *buf, size_t nbytes) { return ::write(fd_, buf, nbytes); }

protected:
    int fd_;
    descriptor_type type_;
};

}  // namespace liph::io

#endif  // LIPH_IO_DESCRIPTOR_H_
