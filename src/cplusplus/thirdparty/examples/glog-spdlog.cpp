#include <sys/time.h>

#include <fstream>
#include <iostream>
#include <thread>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

DEFINE_int32(total_line, 1000 * 1000, "");
DEFINE_int32(thread_num, 1, "");
DEFINE_int32(message_length, 1000, "");
std::mutex ofstream_mutex;

inline int64_t gettimeofday_us() {
    struct timeval now;
    if (gettimeofday(&now, NULL) != 0) {
        perror("gettimeofday");
    }
    return now.tv_sec * 1000000L + now.tv_usec;
}

void glog_cost(int loop, const std::string& log_message) {
    for (int i = 0; i < loop; i++) {
        LOG(INFO) << log_message;
    }
}

void spdlog_cost(std::shared_ptr<spdlog::logger> logger, int loop, const std::string& log_message) {
    for (int i = 0; i < loop; i++) {
        SPDLOG_LOGGER_INFO(logger, log_message);
    }
}

void ofstream_cost(std::ofstream& out, int loop, const std::string& log_message) {
    for (int i = 0; i < loop; i++) {
        std::time_t t = std::time(nullptr);
        struct tm p;
        localtime_r(&t, &p);
        char time_format[32];
        strftime(time_format, sizeof(time_format), "%Y-%m-%d %H:%M:%S", &p);

        std::lock_guard<std::mutex> lock(ofstream_mutex);
        out << time_format << ": " << log_message << "\n";
    }
}

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    const std::string log_message(FLAGS_message_length, '0');

    {
        google::InitGoogleLogging(argv[0]);
        std::vector<std::thread> threads(FLAGS_thread_num);
        int64_t t_begin = gettimeofday_us();
        for (int i = 0; i < FLAGS_thread_num; i++)
            threads[i] = std::thread(glog_cost, FLAGS_total_line / FLAGS_thread_num, log_message);
        for (int i = 0; i < FLAGS_thread_num; i++) threads[i].join();
        int64_t t_end = gettimeofday_us();
        std::cout << "        glog cost " << t_end - t_begin << " us" << std::endl;
    }
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "spd.log");
        logger->set_pattern("I%Y%m%d %H:%M:%S.%f %t %s:%#] %v");
        std::vector<std::thread> threads(FLAGS_thread_num);
        int64_t t_begin = gettimeofday_us();
        for (int i = 0; i < FLAGS_thread_num; i++)
            threads[i] = std::thread(spdlog_cost, logger, FLAGS_total_line / FLAGS_thread_num, log_message);
        for (int i = 0; i < FLAGS_thread_num; i++) threads[i].join();
        int64_t t_end = gettimeofday_us();
        std::cout << "      spdlog cost " << t_end - t_begin << " us" << std::endl;
    }
    {
        auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "spd_async.log");
        async_file->set_pattern("I%Y%m%d %H:%M:%S.%f %t %s:%#] %v");
        std::vector<std::thread> threads(FLAGS_thread_num);
        int64_t t_begin = gettimeofday_us();
        for (int i = 0; i < FLAGS_thread_num; i++)
            threads[i] = std::thread(spdlog_cost, async_file, FLAGS_total_line / FLAGS_thread_num, log_message);
        for (int i = 0; i < FLAGS_thread_num; i++) threads[i].join();
        int64_t t_end = gettimeofday_us();
        std::cout << "spdlog async cost " << t_end - t_begin << " us" << std::endl;
    }
    {
        std::ofstream out("ofstream.log");
        std::vector<std::thread> threads(FLAGS_thread_num);
        int64_t t_begin = gettimeofday_us();
        for (int i = 0; i < FLAGS_thread_num; i++)
            threads[i] = std::thread(ofstream_cost, std::ref(out), FLAGS_total_line / FLAGS_thread_num, log_message);
        for (int i = 0; i < FLAGS_thread_num; i++) threads[i].join();
        out.flush();
        int64_t t_end = gettimeofday_us();
        std::cout << "    ofstream cost " << t_end - t_begin << " us" << std::endl;
    }
}
