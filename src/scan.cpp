#include "scan.h"

#include <iostream>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

namespace ba = boost::adaptors;

namespace {
/**
* @brief Get double files from input map
* Delete elements from input @a map
* @return multimap of double file path
*/
std::multimap<std::string, fs::path> getDoubleFile(std::multimap<std::size_t, File> &map) {
    std::multimap<std::string, fs::path> out;
    for(auto it = map.begin(), end = map.end(); it != end; it = map.upper_bound(it->first)) {
        auto key = it->first;
        auto r = map.equal_range(key);
        while (r.first != map.end()) {
            auto f_it = r.first;
            for (auto i = std::next(f_it); i != r.second;) {
                if ((*f_it).second == (*i).second) {
                    out.insert({(*f_it).second.getFileHash(), (*i).second.path});
                    i = map.erase(i);
                } else {
                    ++i;
                }
            }
            if (!(*f_it).second.getFileHash().empty()) {
                out.insert({(*f_it).second.getFileHash(), (*f_it).second.path});
            }
            map.erase(f_it);
            r = map.equal_range(key);
        }
    }
    return out;
}

/**
* @brief Get files from @a iDir directories, exclude files from @a eDir directories,
* @a level subfolders
* @return list of file path
*/
std::list<fs::path> getFiles(const std::vector<std::string> &iDir
                , const std::vector<std::string> &eDir, const std::size_t level)
{
    std::list<fs::path> files;
    for (auto dir : iDir) {
        for (fs::recursive_directory_iterator it(dir), end; it != end; ++it) {
            if ((std::size_t)it.depth() == level
                || (fs::is_directory(it->path())
                    && boost::range::includes(eDir, std::vector<std::string>({it->path().string()})))) {
                it.no_push(); // don't recurse into this directory
            }
            if (fs::is_regular_file(it->path())) {
                files.emplace_back(it->path());
            }
        }
    }
    return files;
}
} //namespace

std::multimap<std::string, fs::path> scanDoubleFiles(const std::vector<std::string> &iDir
                                                        , const std::vector<std::string> &eDir
                                                        , const std::vector<boost::regex> &rex
                                                        , const std::size_t level
                                                        , const std::size_t minFileSize
                                                        , const std::size_t s
                                                        , hash_func_type hash_func) {
    std::multimap<std::size_t, File> out_path;
    std::list<fs::path> files = getFiles(iDir, eDir, level);    
    for (auto &f : files
//        | ba::filtered(static_cast<bool (*)(const fs::path &)>(&fs::is_regular_file))
        | ba::filtered([&rex](const fs::path &path) {
                for (auto r : rex) {
                    if (boost::smatch what; boost::regex_match(path.filename().string(), what, r)) {
                        return true;
                    }
                }
                return rex.empty();
            })
        | ba::filtered([&minFileSize](const fs::path &path) { return fs::file_size(path) > minFileSize; })
        ) {
            out_path.insert({fs::file_size(f), File(f, s, hash_func)});
    }

    std::erase_if(out_path, [&out_path](const auto& item) { return out_path.count(item.first) == 1; });
    return getDoubleFile(out_path);
}