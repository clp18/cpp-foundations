// All Death tests live here so only this file needs
// -Wno-unsafe-buffer-usage-in-libc-call (EXPECT_DEATH expands to fprintf).

#include <gtest/gtest.h>

#include "cpp_foundations/result.hpp"

// TODO(clp18): Only value() & and error() & are death-tested. Add the
// const&, && and const&& overloads.

// ----- result.hpp Death Tests -----
TEST(ResultDeathTest, ErrorAbortsWhenHoldingValue) {
  cf::Result<char, int> res('a');
  EXPECT_DEATH(static_cast<void>(res.error()), "");
}

TEST(ResultDeathTest, ValueAbortsWhenHoldingError) {
  cf::Result<char, int> res(cf::Unexpected(15));
  EXPECT_DEATH(static_cast<void>(res.value()), "");
}
