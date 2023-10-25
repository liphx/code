#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>

#include <iostream>
#include <sstream>

template <class T>
void print(const T& t) {
    std::cerr << t << std::endl;
}

using namespace google::protobuf;
using namespace google::protobuf::compiler;

class MyCodeGenerator : public CodeGenerator {
public:
    virtual bool Generate(const FileDescriptor *file, const std::string& parameter, GeneratorContext *generator_context,
            std::string *error) const override {
        print(file->name());
        for (int i = 0; i < file->message_type_count(); ++i) {
            const auto *message = file->message_type(i);
            print(message->name());
            for (int j = 0; j < message->field_count(); ++j) {
                const auto *field = message->field(j);
                std::stringstream ss;
                ss << field->number() << ": " << field->name() << ", " << field->type_name();
                print(ss.str());
            }
        }
        return true;
    }
};

int main(int argc, char **argv) {
    MyCodeGenerator generator;
    return PluginMain(argc, argv, &generator);
}
