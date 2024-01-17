#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;
// using namespace std::literals;
using namespace std::chrono_literals;

int main() {
    time_point t0 = system_clock::now();
    cout << t0 << '\n';
    this_thread::sleep_for(1ms);
    time_point t1 = system_clock::now();
    cout << t1 << '\n';
    duration dur = t1 - t0;
    cout << dur << '\n';
    cout << duration_cast<milliseconds>(dur).count() << "ms\n";
    cout << duration_cast<nanoseconds>(dur).count() << "ns\n";

    auto day = 2024y / 1 / 15;
    cout << weekday(day) << '\n';  // Mon

    auto bad_day = January / 0 / 2024;
    if (!bad_day.ok()) cout << bad_day << '\n';

    zoned_time ztp{current_zone(), t1};
    cout << ztp << '\n';
    // 2024-01-15 18:40:03.721743000 CST
}
