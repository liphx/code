#include <google/protobuf/descriptor.h>

#include <cassert>
#include <iostream>

#include "app.pb.h"

using namespace google::protobuf;

int main() {
    const DescriptorPool *pool = DescriptorPool::generated_pool();
    assert(pool);
    const FileDescriptor *file = pool->FindFileByName("app.proto");
    assert(file);
    std::cout << file->name() << std::endl;     // app.proto
    std::cout << file->package() << std::endl;  // app.main
    assert(file->pool() == pool);
    const Descriptor *descriptor = file->FindMessageTypeByName("Request");
    assert(descriptor);
    std::cout << descriptor->name() << std::endl;       // Request
    std::cout << descriptor->full_name() << std::endl;  // app.main.Request
    assert(descriptor->file() == file);
    std::cout << descriptor->DebugString() << std::endl;
    int n = descriptor->field_count();
    for (int i = 0; i < n; i++) {
        const FieldDescriptor *field = descriptor->field(i);
        std::cout << field->name() << std::endl;
        std::cout << field->full_name() << std::endl;
        std::cout << field->type_name() << std::endl;
        std::cout << field->cpp_type_name() << std::endl;
        const FieldOptions& options = field->options();
        if (options.HasExtension(app::main::cpp_type)) {
            std::cout << options.GetExtension(app::main::cpp_type) << std::endl;
        }
        std::cout << std::endl;
    }
}
