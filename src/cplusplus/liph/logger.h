#ifndef LIPH_LOGGER_H_
#define LIPH_LOGGER_H_

#include <atomic>
#include <ctime>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "liph/singleton.h"
#include "liph/time.h"

namespace liph {

class logger {
public:
    logger(const char *pathname);
    ~logger();

    template <class... Args>
    void log(const std::string& filename, int line, const Args&...args) {
        std::stringstream ss;
        ss << std::boolalpha;
        ss << "[" << time_format() << " " << filename << ":" << line << "] ";
        (void)std::initializer_list<int>{([&ss](auto i) { ss << i << ' '; }(args), 0)...};
        ss << std::endl;
        std::lock_guard<std::mutex> lock(lock_);
        producer_->emplace_back(ss.str());
    }

    void start();
    void stop();

private:
    void run();
    void consume(std::ostream& os);

    std::ofstream write_file_;
    std::atomic<bool> started_;
    std::thread tid_;
    std::vector<std::string> *producer_, *consumer_;
    std::vector<std::string> buffer_[2];
    mutable std::mutex lock_;
};

#ifndef __FILENAME__
#define __FILENAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#endif

#define logging(...) singleton<logger>::instance("std.log").log(__FILENAME__, __LINE__, __VA_ARGS__)

}  // namespace liph

#endif  // LIPH_LOGGER_H_
