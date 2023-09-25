#include "ip_utils.h"

#include <algorithm>

namespace IpUtils {

void print(IpPoolType& ip_pool) {
    for(auto ip = ip_pool.cbegin(), pool_end = ip_pool.cend(); ip != pool_end; ++ip) {
        for(auto ip_part = ip->cbegin(), begin = ip->cbegin(), end = ip->cend(); ip_part != end; ++ip_part) {
            if (ip_part != begin) {
                std::cout << ".";
            }
            std::cout << +*ip_part;
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> r;
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
    std::sort(ip_pool.begin(), ip_pool.end(), [](IpType& lhs, IpType& rhs) {
        if (lhs != rhs) return lhs > rhs;
        return false;
    });
}

IpPoolType filter(IpPoolType& ip_pool, const std::string &str) {
    std::vector<std::string> expr_str = split(str, '.');
    while (!expr_str.empty() && expr_str.back() == "*") {
        expr_str.pop_back();
    }
    std::vector<std::uint16_t> expr(expr_str.size());
    std::transform(expr_str.cbegin(), expr_str.cend(), expr.begin(), [](auto& val) {
        if (val == "*") return NOT_IP_NUMBER;
        return static_cast<std::uint16_t>(std::atoi(val.c_str()));
    });
    
    IpPoolType result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result), [&expr](const IpType& ip){
        auto ip_end = ip.cend();
        auto ip_it = ip.cbegin();
        for (auto it = expr.cbegin(), expr_end = expr.cend(); it != expr_end && ip_it != ip_end; ++it, ++ip_it) {
            if (*it < NOT_IP_NUMBER) {
                if (*it != *ip_it) {
                    return false;
                }
            }
        }
        return true;
    } );
    return result;
}

IpPoolType filter(IpPoolType& ip_pool, const std::vector<std::uint8_t>& v) {
    IpPoolType result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result), [&v](const IpType& ip){
            for (const auto& el : v) {
                if (std::any_of(ip.begin(), ip.end(), [&el] (auto& val) { 
                                return [](auto& lhs, auto& rhs) { 
                                    return lhs == rhs; 
                                } (val, el); })) {
                    return true;
                }
            }
            return false;
        } );
    return result;
}
}