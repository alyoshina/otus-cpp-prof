#include "ip_utils.h"

#include <gtest/gtest.h>
#include <stdio.h>

using namespace IpUtils;

// ("",  '.') -> [""]
TEST(Ip_utils_test, Split) {
    EXPECT_TRUE(split(std::string{""}, '.') == std::vector<std::string>{""});
}

// ("11", '.') -> ["11"]
TEST(Ip_utils_test, Split_one_byte) {
    EXPECT_TRUE(split(std::string{"11"}, '.') == std::vector<std::string>{"11"});
}

// ("..", '.') -> ["", "", ""]
TEST(Ip_utils_test, Split_two_points) {
    EXPECT_TRUE(split(std::string{".."}, '.') == (std::vector<std::string>{"", "", ""}));
}

// // ("11.", '.') -> ["11", ""]
TEST(Ip_utils_test, Split_value_point) {
    EXPECT_TRUE(split(std::string{"11."}, '.') == (std::vector<std::string>{"11", ""}));
}

// (".11", '.') -> ["", "11"]
TEST(Ip_utils_test, Split_point_value) {
    EXPECT_TRUE(split(std::string{".11"}, '.') == (std::vector<std::string>{"", "11"}));
}

// ("11.22", '.') -> ["11", "22"]
TEST(Ip_utils_test, Split_value_point_value) {
    EXPECT_TRUE(split(std::string{"11.22"}, '.') == (std::vector<std::string>{"11", "22"}));
}

TEST(Ip_utils_test, Md5sum) {
    char md5sum[100] = {'\0'};
    FILE *fp = popen("cat ./ip_filter.tsv | ./ip_filter | md5sum", "r");
    if (fp) {
        while (fgets(md5sum, sizeof(md5sum), fp) != NULL) {
            ;
        }
    }
    pclose(fp);
    EXPECT_EQ(split(std::string{md5sum}, ' ')[0], std::string{"24e7a7b2270daee89c64d3ca5fb3da1a"});
}

TEST(Ip_utils_test, Sort) {
    IpPoolType ip_pool;
    ip_pool.emplace_back(IpType{1, 1, 1, 1});
    ip_pool.emplace_back(IpType{1, 2, 1, 1});
    ip_pool.emplace_back(IpType{1, 10, 1, 1});
    sort(ip_pool);

    IpPoolType sort_ip_pool;
    sort_ip_pool.emplace_back(IpType{1, 10, 1, 1});
    sort_ip_pool.emplace_back(IpType{1, 2, 1, 1});
    sort_ip_pool.emplace_back(IpType{1, 1, 1, 1});

    EXPECT_TRUE(ip_pool == sort_ip_pool);
}