#include "gflags/gflags.h"

#include <iostream>

DEFINE_string(module, "aa", "one of aa|ab|bc");
DEFINE_validator(module, [](const char *, const std::string& value) {
    if (value != "aa" && value != "ab" && value != "bc") {
        return false;
    }
    return true;
});

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "module: " << FLAGS_module << std::endl;
}
