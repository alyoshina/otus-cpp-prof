#pragma once

#include "hash_algorithms.h"

#include <boost/filesystem.hpp>
#include <boost/range/numeric.hpp>

namespace fs = boost::filesystem;

/** @brief Read file in blocks, block hash calculation 
*
*/
class File {
public:
    File(fs::path &p, std::size_t s, hash_func_type hash_f)
        : path(p)
        , blockSize(s)
        , fillTailZeros(fs::file_size(p) % blockSize)
        , maxSize(fs::file_size(p) / blockSize + (fillTailZeros ? 1 : 0))
        , hash_func(hash_f) {}
    fs::path path;
    const std::size_t blockSize; /**< block size in bytes*/
    bool fillTailZeros;
    const std::size_t maxSize; /**< number of blocks in file*/
    hash_func_type hash_func; /**< hash calculation function*/

    std::list<std::string> hashes; /**< list of blocks hash*/
    std::string fileHash; /**< file hash, calculated when all blocks in the file have been read*/

    /** @brief Calculate file hash if all blocks in the file have been read
    * @return file hash string
    */
    std::string getFileHash();

    bool operator==(File& r);

    /** @brief Read @a k block of file, if not already read, calculate block hash
    * @param[in] k is number of block in file
    * @return @a k block hash string
    */
    std::string get(const std::size_t k);

    /** @brief Read @a k block of file and calculate block hash
    * @param[in] k is number of block in file
    * @return @a k block hash string
    */
    std::string blockRead(const std::size_t k);
};