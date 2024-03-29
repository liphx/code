#ifndef LIPH_LOGGING_H_
#define LIPH_LOGGING_H_

#include <atomic>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "liph/format.h"
#include "liph/lang/noncopyable.h"
#include "liph/lang/singleton.h"
#include "liph/time.h"

namespace liph {

class logger : noncopyable {
private:
    static constexpr char logfilename[] = "std.log";

public:
    logger() {
        producer_ = &buffer_[0];
        consumer_ = &buffer_[1];
        file_.open(logfilename, std::ios_base::out | std::ios_base::app);
        if (!file_.is_open()) {
            logtostderr_ = true;
            std::cerr << "open log file fail, use stderr\n";
        }
        started_ = true;
        tid_ = std::thread(&logger::run, this);
    }

    ~logger() {
        started_ = false;
        if (tid_.joinable()) tid_.join();
        if (file_.is_open()) file_.close();
    }

    void log(std::ostringstream& ss) {
        std::lock_guard<std::mutex> lock(lock_);
        producer_->emplace_back(std::move(ss).str());
    }

    void set_logtostderr(bool b) { logtostderr_ = b; }

private:
    void run() {
        while (started_) {
            consume();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        consume();
    }

    void consume() {
        std::ostream& os = (logtostderr_ || !file_.is_open()) ? std::cerr : file_;
        {
            std::lock_guard<std::mutex> lock(lock_);
            std::swap(producer_, consumer_);
        }
        if (!consumer_->empty()) {
            for (const std::string& str : *consumer_) {
                os << str;
            }
            os.flush();
            consumer_->clear();
        }
    }

    std::ofstream file_;
    std::atomic<bool> started_;
    std::thread tid_;
    std::vector<std::string> *producer_, *consumer_;
    std::vector<std::string> buffer_[2];
    mutable std::mutex lock_;
    std::atomic<bool> logtostderr_{false};
};

class log_message {
public:
    log_message(const char *filename, int line) {
        ss_.str("");
        ss_ << format("[{} {} {}:{}] ", time_format(), std::this_thread::get_id(), filename, line);
    }

    std::ostringstream& stream() { return ss_; }

    ~log_message() {
        ss_ << '\n';
        singleton<logger>::instance().log(ss_);
    }

private:
    inline thread_local static std::ostringstream ss_;
};

}  // namespace liph

#ifndef __FILENAME__
#define __FILENAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#endif
#define LOG liph::log_message(__FILENAME__, __LINE__).stream()

#endif  // LIPH_LOGGING_H_
