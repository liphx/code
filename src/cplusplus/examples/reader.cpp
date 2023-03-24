#include <atomic>
#include <fstream>
#include <iostream>
#include <thread>

std::atomic<bool> done = false;
void deal_line(const std::string& str) { std::cout << str << std::endl; }

void deal_file(std::ifstream& in) {
    if (!in.is_open()) {
        return;
    }
    std::string str;
    auto pos = in.tellg();
    while (true) {
        if (std::getline(in, str)) {  // not EOF
            pos = in.tellg();
            deal_line(str);
        } else if (done) {  // modified by another thread
            break;
        } else {  // EOF but not done
            in.clear();
            in.seekg(pos);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    std::ifstream in(argv[1]);
    std::thread t1(deal_file, std::ref(in));
    std::thread t2([]() {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        done = true;
    });
    t1.join();
    t2.join();
}
