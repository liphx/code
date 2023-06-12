#include "liph/logger.h"

#include <unistd.h>

#include <iostream>

namespace liph {

logger::logger(const char *pathname) {
    write_file_.open(pathname, std::ios_base::out | std::ios_base::app);
    producer_ = &buffer_[0];
    consumer_ = &buffer_[1];
}

void logger::start() {
    if (started_) return;
    started_ = true;
    tid_ = std::thread(&logger::run, this);
}

void logger::consume(std::ostream& os) {
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

void logger::run() {
    std::ostream& os = write_file_.is_open() ? write_file_ : std::cerr;
    while (started_) {
        consume(os);
        usleep(100000);
    }
    consume(os);
}

void logger::stop() {
    if (started_) {
        started_ = false;
        tid_.join();
    }
}

logger::~logger() { stop(); }

}  // namespace liph
