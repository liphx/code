#include <fstream>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    std::ofstream out(argv[1]);
    int n = 0;

    while (true) {
        out << "line " << ++n << "\n";
        out.flush();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
