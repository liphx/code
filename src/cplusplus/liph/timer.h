#ifndef LIPH_TIMER_H_
#define LIPH_TIMER_H_

#include <pthread.h>
#include <unistd.h>

#include <algorithm>
#include <tuple>
#include <vector>

namespace liph {

class Timer {
public:
    static const int interval_without_jobs = 1;
    typedef void (*timer_func)(void *);
    Timer();
    ~Timer();
    pthread_t start();
    void AddJob(timer_func func, int interval, void *args);

private:
    std::vector<std::tuple<int, int, timer_func, void *>> jobs;
};

void Timer::AddJob(timer_func func, int interval, void *args) {
    jobs.emplace_back(std::make_tuple(0, interval, func, args));
    push_heap(jobs.begin(), jobs.end(), std::greater<>());
}

pthread_t Timer::start() {
    pthread_t th;
    pthread_create(
            &th, NULL,
            [](void *t) {
                Timer *timer = (Timer *)t;
                int current_time = 0;
                for (;;) {
                    if (timer->jobs.empty()) {
                        sleep(Timer::interval_without_jobs);
                        continue;
                    }

                    int next_time = std::get<0>(timer->jobs.front());
                    int sleep_time = next_time - current_time;
                    if (sleep_time > 0)  // 为0时不调用sleep
                        sleep(sleep_time);
                    current_time = next_time;

                    auto current_job = timer->jobs.front();
                    timer_func func = std::get<2>(current_job);
                    void *args = std::get<3>(current_job);
                    func(args);

                    pop_heap(timer->jobs.begin(), timer->jobs.end(), std::greater<>());
                    timer->jobs.pop_back();

                    std::get<0>(current_job) += std::get<1>(current_job);
                    timer->jobs.emplace_back(current_job);
                    push_heap(timer->jobs.begin(), timer->jobs.end(), std::greater<>());
                }

                return (void *)nullptr;
            },
            this);

    return th;
}

Timer::Timer() {}

Timer::~Timer() { pthread_exit(NULL); }

}  // namespace liph

#endif  // LIPH_TIMER_H_
