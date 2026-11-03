#include <gtest/gtest.h>

#include "cpp_foundations/math.hpp"

static_assert(cf::Add(2, 3) == 5);

TEST(MathTest, AddsTwoIntegers) {
  EXPECT_EQ(cf::Add(2, 3), 5);
  EXPECT_EQ(cf::Add(-4, 4), 0);
}
