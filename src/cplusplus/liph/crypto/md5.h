#ifndef LIPH_CRYPTO_MD5_H_
#define LIPH_CRYPTO_MD5_H_

#include <string>

namespace liph {

// md5sum of str, returns 32 uppercase hexadecimal letters
std::string md5(const std::string& str);

}  // namespace liph

#endif  // LIPH_CRYPTO_MD5_H_
