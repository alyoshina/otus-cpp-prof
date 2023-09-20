#include "version.h"

#include <gtest/gtest.h>

TEST(Test_version, Get_version) {
    EXPECT_TRUE(get_version() > 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
