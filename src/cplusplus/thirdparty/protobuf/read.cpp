#include <iostream>

#include "conf.pb.h"
#include "google/protobuf/text_format.h"

int main() {
    Conf conf;
    const std::string text = "name: \"abc\" size: 100";
    if (google::protobuf::TextFormat::ParseFromString(text, &conf)) {
        std::cout << conf.name() << ", " << conf.size() << std::endl;
    }
}
