#include "ip_utils.h"

#include <algorithm>

namespace IpUtils {

void print(IpPoolType& ip_pool) {
    for(auto ip = ip_pool.cbegin(), pool_end = ip_pool.cend(); ip != pool_end; ++ip) {
        for(auto ip_part = ip->cbegin(), begin = ip->cbegin(), end = ip->cend(); ip_part != end; ++ip_part) {
            if (ip_part != begin) {
                std::cout << ".";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

IpType split(const std::string &str, char d) {
    IpType r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos) {
        r.emplace_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.emplace_back(str.substr(start));
    return r;
}

void sort(IpPoolType& ip_pool) {
    auto ip_compare = [](IpType l, IpType r) {
        auto is_eq = [](auto& lhs, auto& rhs) { return std::atoi(lhs.c_str()) == std::atoi(rhs.c_str()); };
        auto comp = [](auto& lhs, auto& rhs) { return std::atoi(lhs.c_str()) > std::atoi(rhs.c_str()); };
        for (auto l_it = l.cbegin(), r_it = r.cbegin(), end = l.cend(); l_it != end; ++l_it, ++r_it) {
            if (!is_eq(*l_it, *r_it)) {
                return comp(*l_it, *r_it);
            }
        }
        return false;
    };
    std::sort(ip_pool.begin(), ip_pool.end(), ip_compare);
}

IpPoolType filter(IpPoolType& ip_pool, const std::string &str) {
    IpType expr = split(str, '.');
    while (!expr.empty() && expr.back() == "*") {
        expr.pop_back();
    }
    
    IpPoolType result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result), [&expr](const IpType& ip){
        auto is_eq = [](auto& lhs, auto& rhs)->bool { return std::atoi(lhs.c_str()) == std::atoi(rhs.c_str()); };
        auto ip_end = ip.cend();
        auto expr_end = expr.cend();
        for (auto it = expr.cbegin(), ip_it = ip.cbegin(); it != expr_end && ip_it != ip_end; ++it, ++ip_it) {
            if (*it != "*") {
                if (!is_eq(*it, *ip_it)) {
                    return false;
                }
            }
        }
        return true;
    } );
    return result;
}

IpPoolType filter(IpPoolType& ip_pool, const std::vector<std::string>& v) {
    IpPoolType result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result), [&v](const IpType& ip){
            for (const auto& el : v) {
                if (std::any_of(ip.begin(), ip.end(), [&el] (auto& val) { 
                                return [](auto& lhs, auto& rhs) { 
                                    return std::atoi(lhs.c_str()) == std::atoi(rhs.c_str()); 
                                } (val, el); })) {
                    return true;
                }
            }
            return false;
        } );
    return result;
}
}