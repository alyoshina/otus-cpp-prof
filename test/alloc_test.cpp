// #include "allocator.h"
// #include "factorial.h"
// #include "linear.h"
// #include "pool.h"
// #include "list.h"

// #include <gtest/gtest.h>
// #include <stdio.h>
// #include <map>

// using namespace Mem;

// template <typename MapAlloc>
// void test_bad_alloc(const std::size_t count, bool expect = true) {
//     std::map<int, int, std::less<int>, MapAlloc> m;
//     bool is_bad_alloc = false;
//     try {
//         for (int i = 0; i < count + 1; ++i) {
//             m[i] = i;
//             if (i == count - 1) {
//                 EXPECT_EQ(m[i], i);
//             }
//         }
//     }
//     catch (const std::bad_alloc& err){
//         is_bad_alloc = true;
//         EXPECT_EQ(err.what(), std::string("std::bad_alloc"));
//     }
//     EXPECT_EQ(expect, is_bad_alloc);
// }

// TEST(Alloc_test, Bad_alloc_for_Linear) {
//     const int count = 2;
//     using MapAlloc = Allocator<std::pair<const int, int>, count, Linear>;
//     test_bad_alloc<MapAlloc>(count);
// }

// TEST(Alloc_test, Not_bad_alloc_for_ExtensionLinear) {
//     const int count = 2;
//     using MapAlloc = Allocator<std::pair<const int, int>, count, ExtensionLinear>;
//     test_bad_alloc<MapAlloc>(count, false);
// }

// TEST(Alloc_test, Not_bad_alloc_for_Pool) {
//     const int count = 2;
//     using MapAlloc = Allocator<std::pair<const int, int>, count, Pool>;
//     test_bad_alloc<MapAlloc>(count, false);
// }

// TEST(Alloc_test, Std_map) {
//     const int block_size = 10;
//     const int el_num = 10;
//     using MapAlloc = Mem::Allocator<std::pair<const int, int>, block_size>;
//     std::map<int, int> std_map;
//     std::map<int, int, std::less<int>, MapAlloc> alloc_std_map;
//     for (int i = 0; i < el_num; ++i) {
//         std_map[i] = fact(i);
//         alloc_std_map[i] = fact(i);
//     }
//     for (int i = 0; i < el_num; ++i) {
//         EXPECT_EQ(std_map[i], fact(i));
//         EXPECT_EQ(std_map[i], alloc_std_map[i]);
//     }
// }

// TEST(Alloc_test, Custom_list) {
//     const int el_num = 10;
//     using ListAlloc = Mem::Allocator<int, el_num>;
//     List<int> list;
//     List<int, ListAlloc> alloc_list;
//     for (int i = 0; i < el_num; ++i) {
//         list.push_back(fact(i));
//         alloc_list.push_back(fact(i));
//     }
//     for (int i = 0; i < el_num; ++i) {
//         EXPECT_EQ(list[i], fact(i));
//         EXPECT_EQ(list[i], alloc_list[i]);
//     }
// }