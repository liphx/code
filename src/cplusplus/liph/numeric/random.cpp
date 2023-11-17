#include "liph/numeric/random.h"

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

std::random_device randomizer::r;
std::default_random_engine randomizer::e(randomizer::r());

}  // namespace liph
