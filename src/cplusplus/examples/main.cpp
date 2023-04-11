#include "absl/strings/string_view.h"
#include "liph/liph.h"

using namespace liph;

int main() {
    D(__GNUC__);
    D(0x1a);
    D(std::numeric_limits<uint64_t>::max());
    absl::string_view sv("hello");
    D(sv);
}
