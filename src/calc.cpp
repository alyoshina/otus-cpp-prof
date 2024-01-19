#include "calc.h"

std::string File::getFileHash() {
    if (fileHash.empty() && maxSize == hashes.size() && hashes.size() > 0) {
        fileHash = boost::accumulate(hashes, std::string("")
                            , [this](const std::string& fh, const std::string& h) {
            std::string str = fh + h;
            return this->hash_func(str.data(), str.size());
        });
    }
    return fileHash;
}

bool File::operator==(File& r) {
    if (maxSize == r.maxSize) {
        for (std::size_t i = 0; i < maxSize; ++i) {
            if (get(i) != r.get(i)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::string File::get(const std::size_t k) {
    if (k < maxSize) {
        if (k < hashes.size()) {
            auto it = hashes.begin();
            std::advance(it, k);
            return *it;
        } else {
            for (auto i = hashes.size(); i <= k; ++i) {
                hashes.emplace_back(blockRead(i));
            }
            return hashes.back();
        }
    }
    return "";
}

std::string File::blockRead(const std::size_t k) {
    std::vector<char> block;
    block.resize(blockSize);
    fs::ifstream f(path);
    f.seekg(blockSize*k).read(block.data(), blockSize);
    return hash_func(block.data(), blockSize);
}