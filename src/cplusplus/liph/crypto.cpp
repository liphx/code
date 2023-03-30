#include "liph/crypto.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>

namespace liph {

namespace crypto {

std::string md5(const std::string& msg) {
    using namespace CryptoPP;
    Weak::MD5 hash;
    std::string digest;
    hash.Update((const uint8_t *)msg.data(), msg.size());
    digest.resize(hash.DigestSize());
    hash.Final((uint8_t *)&digest[0]);
    std::string out;
    StringSource ss(digest, true, new HexEncoder(new StringSink(out)));
    return out;
}

}  // namespace crypto

}  // namespace liph
