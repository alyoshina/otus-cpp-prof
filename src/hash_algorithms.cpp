#include "hash_algorithms.h"

#include <boost/crc.hpp>
#include <boost/algorithm/hex.hpp>

std::string crc32(const char *block, const std::size_t size) {
    boost::crc_32_type crc_result;
    crc_result.process_bytes(block, size);
    uint32_t crc32 = crc_result.checksum();
    
    std::string crc32str;
    const auto charDigest = reinterpret_cast<const char *>(&crc32);
    boost::algorithm::hex(charDigest, charDigest + sizeof(crc32), std::back_inserter(crc32str));
    return crc32str;
}

std::string md5(const char *block, const std::size_t size) {
    ud::md5 hash;
    ud::md5::digest_type digest;
    hash.process_bytes(block, size);
    hash.get_digest(digest);

    std::string md5str;
    const auto charDigest = reinterpret_cast<const char *>(&digest);
    boost::algorithm::hex(charDigest, charDigest + sizeof(digest), std::back_inserter(md5str));
    return md5str;
}
