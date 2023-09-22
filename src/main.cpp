#include "ip_utils.h"

using namespace IpUtils;

//cat ../ip_filter_simple.tsv | ./ip_filter
int main(int argc, char const *argv[])
{
    try {
        IpPoolType ip_pool;

        for(std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }
        //reverse lexicographically sort
        sort(ip_pool);
        print(ip_pool);

        //filter by first byte and output
        {
            IpPoolType f = filter(ip_pool, std::string("1.*.*.*"));
            print(f);
        }
        //filter by first byte and output
        {
            IpPoolType f = filter(ip_pool, std::string("46.70.*"));
            print(f);
        }
        //filter by any byte and output
        {
            //std::vector<std::string> v{"46", "29"};
            std::vector<std::string> v{"46"};
            IpPoolType f = filter(ip_pool, v);
            print(f);
        }
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
