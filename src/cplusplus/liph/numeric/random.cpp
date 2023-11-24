#include "liph/numeric/random.h"

#include <cstring>

namespace liph {

#if 0
bool random::next_bytes(void *ptr, size_t n) {
    FILE *fp = std::fopen("/dev/urandom", "r");
    if (!fp) return false;
    if (std::fread(ptr, 1, n, fp) != n) return false;
    return std::fclose(fp) == 0;
}
#endif

bool random::next_bytes(void *ptr, size_t n) {
    thread_local auto engine = []() {
        std::random_device r;
        std::seed_seq seeds{r(), r(), r(), r(), r(), r(), r(), r()};
        std::mt19937_64 engine(seeds);
        return engine;
    }();

    uint64_t r = engine();
    while (n >= sizeof(r)) {
        *reinterpret_cast<uint64_t *>(ptr) = r;
        ptr = reinterpret_cast<uint64_t *>(ptr) + 1;
        n -= sizeof(r);
    }
    if (n > 0) std::memcpy(ptr, &r, n);
    return true;
}

std::string random::gen_str(size_t size, std::string_view charset) {
    if (size == 0 || charset.empty()) return "";
    std::string str(size, 0);
    for (size_t i = 0; i < size; ++i) {
        str[i] = charset[gen_int(charset.size())];
    }
    return str;
}

#if 0
std::random_device randomizer::r;
std::default_random_engine randomizer::e(randomizer::r());
#endif

}  // namespace liph
