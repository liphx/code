#include <iostream>
#include <vector>

#if 1
#include "spdlog/sinks/basic_file_sink.h"
auto logger = spdlog::basic_logger_mt("basic_logger", "std.log");
int main() {
    logger->info("info log");
    logger->error("info error");
    sleep(10);
}
#endif

#if 0
#include "glog/logging.h"

int main(int, char **argv) {
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "info log";
    LOG(WARNING) << "warning log";
    LOG(ERROR) << "error log";
    /* LOG(FATAL) << "fatal log"; */
    sleep(10);
}
#endif

#if 0
struct A {
    int x;
    void f() { delete this; }

    ~A() { std::cout << "~A()" << std::endl; }

    void *operator new(std::size_t size) {
        std::cout << "A::new" << std::endl;
        return malloc(size);
    }
    void operator delete(void *ptr) {
        std::cout << "A::delete" << std::endl;
        free(ptr);
    }
};

int main() {
    A *a = new A;
    a->f();
    std::cout << "end main" << std::endl;
}
#endif

#if 0
#include "absl/strings/string_view.h"

int main() {
    D(__GNUC__);
    D(0x1a);
    D(std::numeric_limits<uint64_t>::max());
    absl::string_view sv("hello");
    D(sv);
}
#endif
