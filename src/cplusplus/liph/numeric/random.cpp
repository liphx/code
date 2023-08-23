#include "liph/numeric/random.h"

namespace liph {

std::random_device randomizer::r;
std::default_random_engine randomizer::e(randomizer::r());

}  // namespace liph
