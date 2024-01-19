#pragma once

#include "calc.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <list>
#include <map>

namespace fs = boost::filesystem;

/**
* @brief Get double files
* @param[in] iDir are directories for scanning files
* @param[in] eDir are directories to exclude from scanning
* @param[in] rex are masks of file names allowed for comparison (do not depend on register)
* @param[in] level is scanning level 
* (one for all directories, 0 - only the specified one directory without subfolders)
* @param[in] minFileSize is minimum file size for scanning in bytes
* @param[in] s is block size used to read files
* @param[in] hash_func is hashing algorithm
* @return multimap of file hash and path
*/
std::multimap<std::string, fs::path> scanDoubleFiles(const std::vector<std::string> &iDir
                        , const std::vector<std::string> &eDir
                        , const std::vector<boost::regex> &rex
                        , const std::size_t level, const std::size_t minFileSize
                        , const std::size_t s
                        , hash_func_type hash_func);
