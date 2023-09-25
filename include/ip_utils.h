#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <array>

namespace IpUtils {

using IpType = std::array<std::uint8_t, 4>;
using IpPoolType = std::vector<IpType>;

const std::uint16_t NOT_IP_NUMBER = 256;
void print(IpPoolType& ip_pool);
std::vector<std::string> split(const std::string &str, char d);
void sort(IpPoolType& ip_pool);
IpPoolType filter(IpPoolType& ip_pool, const std::string &str);
IpPoolType filter(IpPoolType& ip_pool, const std::vector<std::uint8_t>& v);
}