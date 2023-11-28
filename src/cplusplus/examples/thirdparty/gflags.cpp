#include "gflags/gflags.h"

#include <iostream>

DEFINE_string(module, "aa", "one of aa|ab|bc");
DEFINE_validator(module, [](const char *, const std::string& value) {
    if (value != "aa" && value != "ab" && value != "bc") {
        return false;
    }
    return true;
});

DEFINE_int32(a, 0, "");
DEFINE_int32(b, 0, "");

DECLARE_string(flagfile);

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "module: " << FLAGS_module << std::endl;

    std::string str = "--a=42\n--b=100";
    google::ReadFlagsFromString(str, google::ProgramInvocationName(), true);
    std::cout << FLAGS_a << " " << FLAGS_b << std::endl;

    FLAGS_flagfile = "num.conf";
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << FLAGS_a << " " << FLAGS_b << std::endl;
}
