#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace IpUtils {

using IpPoolType = std::vector<std::vector<std::string> >;
using IpType = std::vector<std::string>;

void print(IpPoolType& ip_pool);
IpType split(const std::string &str, char d);
void sort(IpPoolType& ip_pool);
IpPoolType filter(IpPoolType& ip_pool, const std::string &str);
IpPoolType filter(IpPoolType& ip_pool, const std::vector<std::string>& v);
}