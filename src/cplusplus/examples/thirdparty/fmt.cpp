#include <vector>

#include "fmt/chrono.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/os.h"
#include "fmt/ranges.h"

int main() {
    fmt::print("Hello, world!\n");

    std::string s = fmt::format("The answer is {}.", 42);
    fmt::print("{}\n", s);
    s = fmt::format("I'd rather be {1} than {0}.", "right", "happy");
    fmt::print("{}\n", s);

    using namespace std::literals::chrono_literals;
    fmt::print("Default format: {} {}\n", 42s, 100ms);
    fmt::print("strftime-like format: {:%H:%M:%S}\n", 3h + 15min + 30s);

    std::vector v{1, 2, 3};
    fmt::print("{}\n", v);

    // s = fmt::format("{:d}", "I am not a number");

    // auto out = fmt::output_file("tmp.txt");
    // out.print("Don't {}", "Panic");

    fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold, "Hello, {}!\n", "world");
    fmt::print(fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) | fmt::emphasis::underline, "Hello, {}!\n",
            "world");
    fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "Hello, {}!\n", "world");
}
