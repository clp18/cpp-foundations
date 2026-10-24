#include <gtest/gtest.h>
#include "../include/cpp_foundations/math.hpp"

static_assert(cf::add(2, 3) == 5);

TEST(MathTest, AddsTwoIntegers) {
    EXPECT_EQ(cf::add(2, 3), 5);
    EXPECT_EQ(cf::add(-4, 4), 0);
}