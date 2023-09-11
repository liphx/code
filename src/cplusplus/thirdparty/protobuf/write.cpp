#include <iostream>

#include "conf.pb.h"
#include "google/protobuf/text_format.h"

int main() {
    Conf conf;
    conf.set_name("abc");
    conf.set_size(100);
    std::string output;
    if (google::protobuf::TextFormat::PrintToString(conf, &output)) {
        std::cout << output;
    }
}
