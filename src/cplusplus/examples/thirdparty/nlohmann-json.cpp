#include <iostream>

#include "nlohmann/json.hpp"

int main() {
    using json = nlohmann::json;
    json j;
    j["pi"] = 3.14;
    std::cout << j.dump() << std::endl;
}
