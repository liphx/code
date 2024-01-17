#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <ranges>
#include <stop_token>
#include <thread>
#include <vector>
using namespace std;
using namespace std::chrono;
using namespace std::literals;

bool find(const vector<int>& data, int target) {
    atomic<bool> found = false;
    int mid = data.size() / 2;
    const int *begin = &data[0];

    auto func = [&](stop_token tok, const int *begin, const int *end, atomic<bool>& done) {
        for (; begin != end && !tok.stop_requested(); ++begin) {
            if (*begin == target) {
                found = true;
                return;
            }
        }
        done = true;
    };

    atomic<bool> done = false;
    stop_source ss;
    jthread t(func, ss.get_token(), begin, begin + mid, ref(done));

    atomic<bool> done2 = false;
    stop_source ss2;
    jthread t2(func, ss2.get_token(), begin + mid, begin + data.size(), ref(done2));

    while (true) {
        if (found) {
            ss.request_stop();
            ss2.request_stop();
            return true;
        }
        if (done && done2) break;
    }
    return false;
}

int main() {
    vector<int> data(10'000'000);
    data[5'000'000] = 1;

    auto t = system_clock::now();
    assert(find(data, 1));
    cout << "cost " << system_clock::now() - t << '\n';

    t = system_clock::now();
    assert(ranges::find(data, 1) != data.end());
    cout << "cost " << system_clock::now() - t << '\n';
}
