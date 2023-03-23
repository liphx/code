#ifndef LIPH_CRYPTO_H_
#define LIPH_CRYPTO_H_

#include <string>

namespace liph {

namespace crypto {

// md5sum of str, returns 32 uppercase hexadecimal letters
std::string md5(const std::string& str);

}  // namespace crypto

}  // namespace liph

#endif  // LIPH_CRYPTO_H_
