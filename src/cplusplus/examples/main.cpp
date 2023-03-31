#include "absl/strings/string_view.h"
#include "liph/liph.h"

int main() {
    P(0x1a);
    P(std::numeric_limits<uint64_t>::max());
    absl::string_view sv("hello");
	P(sv);
}
