#include "scan.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/range/algorithm.hpp>

namespace po = boost::program_options;

hash_func_type hash_function;
void hashType(const std::string &type) {
    std::unordered_map<std::string, decltype(hash_function)> map {
        {"md5", &md5},
        {"crc32", &crc32}
    };
    if (map.contains(type)) {
        hash_function = map[type];
    } else {
        hash_function = map["md5"];
    }
}

void options(int argc, char** argv
                , po::options_description& desc, po::variables_map& vm) {
    desc.add_options()
        ("help,h", "Utility to detect duplicate files")
        ("iDir,i", po::value<std::vector<std::string>>() -> multitoken()
            //-> default_value(std::vector<std::string>(), "./")
            , "directories for scanning files")
        ("eDir,e", po::value<std::vector<std::string>>() -> multitoken()
            -> default_value(std::vector<std::string>(), "")
            , "directories to exclude from scanning")
        ("level,l", po::value<std::size_t>() -> default_value(0)
            , "scanning level (one for all directories, "
            "0 - only the specified one directory without subfolders)")
        ("min,m", po::value<std::size_t>() -> default_value(1)
            , "minimum file size for scanning in bytes")
        ("masks", po::value<std::vector<std::string>>() -> multitoken()
            -> default_value(std::vector<std::string>(), "")
            , "masks of file names allowed for comparison (do not depend on register)")
        ("size,S", po::value<std::size_t>() -> default_value(1)
            , "block size used to read files")
        ("hash,H", po::value<std::string>() -> default_value("md5")
            -> notifier(hashType)
            , "hashing algorithms: crc32, md5")
        ;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
}

/**
 * @brief Using the Boost library develop a utility to detect duplicate files.
 *
 * Result list of full file paths with identical content, printed to standard output.
 * One file per line. Identical files are output in one group.
 * The different groups are separated by a empty line.
 * Usage example:
 * ./bayan -h
 * ./bayan -i "./" -l 2 -m 1 --masks "bay(.*)"
 * ./bayan -i "../" -S 8 -l 3 -m 1 --masks "test_test(.*)"
 * ./bayan --iDir "./" --eDir "./1/2" './CMakeFiles' -l 6
 *
 */

int main(int argc, char** argv)
{   
    try {
        //processing сommand line argument
        po::options_description desc {"Options"};
        po::variables_map vm;
        
        options(argc, argv, desc, vm);
        
        //options(argc, argv, desc, vm);
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }

        if (!vm.count("iDir")) {
            std::cout << "Must specify the directories for scanning files.\nExamples:\n"
                        << "./" << PROJECT_NAME << " -i \"./\"\n"
                        << "./" << PROJECT_NAME << " -i \"./dir1\" \"./dir2\"" << std::endl;
            return 0;
        }

        //transform string to regex
        std::vector<boost::regex> rex;
        boost::range::transform(vm["masks"].as<std::vector<std::string>>()
                        , std::back_inserter(rex)
                        , [] (const std::string& str) { return boost::regex(str); });
        
        //get double files
        auto map = scanDoubleFiles(vm["iDir"].as<std::vector<std::string>>()
                                        , vm["eDir"].as<std::vector<std::string>>()
                                        , rex, vm["level"].as<std::size_t>(), vm["min"].as<std::size_t>()
                                        , vm["size"].as<std::size_t>()
                                        , hash_function);

        //output double files to standard output
        {
            auto r = map.equal_range(map.begin()->first);
            for (auto i = r.first; i != r.second; ++i) {
                std::cout << fs::canonical((*i).second).string() << std::endl;
            }
        }
        for(auto it = map.upper_bound(map.begin()->first), end = map.end(); it != end; it = map.upper_bound(it->first)) {
            std::cout << std::endl;
            auto r = map.equal_range(it->first);
            for (auto i = r.first; i != r.second; ++i) {
                std::cout << fs::canonical((*i).second).string() << std::endl;
            }
        }
    } catch (const std::exception& e) { //
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}