#include <iostream>
#include <string>
#include <unistd.h>

bool print_progress_bar(double progress, std::string block_style = "#", int block_number_max = 100)
{
    if (progress < 0)
        return false;

    int block_number = std::min<double>(block_number_max, progress * block_number_max);

    std::cout << '\r';
    for (int i = 0; i < block_number; i++)
        std::cout << block_style;
    std::cout << std::flush;

    return progress >= 1;
}

int main(int argc, char *argv[])
{
    double i = 0.65;
    while (!print_progress_bar(i, "=", 60)) {
        i += 0.1;
        sleep(1);
    }
    std::cout << " 100% done" << std::endl;
}
