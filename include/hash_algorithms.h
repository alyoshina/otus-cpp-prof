#pragma once


#include <boost/function.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <string>

using hash_func_type = boost::function<std::string (const char *, const std::size_t)>;
namespace ud = boost::uuids::detail;

/** @brief Creates crc32 string from input string data.
* @param [in] block is pointer to data
* @param [in] size is data size
* @return crc32 string
*/
std::string crc32(const char *block, const std::size_t size);

/** @brief Creates md5 string from input string data.
* @param [in] block is pointer to data
* @param [in] size is data size
* @return md5 string
*/
std::string md5(const char *block, const std::size_t size);