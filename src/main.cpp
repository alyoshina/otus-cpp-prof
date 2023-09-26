#include "ip_utils.h"

using namespace IpUtils;

#include <array>
#include <algorithm>
//cat ../ip_filter_simple.tsv | ./ip_filter
int main(int argc, char const *argv[])
{
    try {
        IpPoolType ip_pool;

        for(std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');
            std::vector<std::string> ip_str = split(v.at(0), '.');
            ip_pool.emplace_back(IpType{});
            std::transform(ip_str.cbegin(), ip_str.cend(), ip_pool.back().begin(), [](auto& val) { return std::atoi(val.c_str()); });   
        }
        
        //reverse lexicographically sort
        sort(ip_pool);
        print(ip_pool);

        //filter by first byte and output
        {
            //IpPoolType f = filter(ip_pool, std::string("1.*.*.*"));
            IpPoolType f = filter(ip_pool, [](const IpType& ip ) { return ip[0] == 1; });
            print(f);
        }
        //filter by first byte and output
        {
            //IpPoolType f = filter(ip_pool, std::string("46.70.*"));
            IpPoolType f = filter(ip_pool, [](const IpType& ip ) { return ip[0] == 46 && ip[1] == 70; });
            print(f);
        }
        //filter by any byte and output
        {
            //std::vector<std::uint8_t> v{46, 29};
            //std::vector<std::uint8_t> v{46};
            //IpPoolType f = filter(ip_pool, v);
            IpPoolType f = filter(ip_pool, [](const IpType& ip ) {
                bool ret_val = false;
                for (auto& ip_n: ip) {
                    ret_val = ret_val || (ip_n == 46);
                }
                return ret_val; 
            });
            print(f);
        }
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
