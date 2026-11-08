// All Death tests live here so only this file needs
// -Wno-unsafe-buffer-usage-in-libc-call (EXPECT_DEATH expands to fprintf).

#include <gtest/gtest.h>

#include "cpp_foundations/result.hpp"

// TODO(clp18): Result<T, E> only death-tests value() & and error() &, and
// Result<void, E> only error() &. Add the remaining overloads.

// ----- result.hpp Death Tests -----

// ---------- Death Test Cases for cf::Result<T, E> ----------
TEST(ResultDeathTest, ErrorAbortsWhenHoldingValue) {
  cf::Result<char, int> res('a');
  EXPECT_DEATH(static_cast<void>(res.error()), "");
}

TEST(ResultDeathTest, ValueAbortsWhenHoldingError) {
  cf::Result<char, int> res(cf::Unexpected(15));
  EXPECT_DEATH(static_cast<void>(res.value()), "");
}
// ---------- End of Death Test Cases for cf::Result<T, E> ----------

// ---------- Death Test Cases for cf::Result<void, E> ----------
TEST(VoidResultDeathTest, ValueAbortsWhenHoldingError) {
  cf::Result<void, int> res(cf::Unexpected(1));
  EXPECT_DEATH(res.value(), "");
}

TEST(VoidResultDeathTest, ErrorAbortsWhenHoldingSuccess) {
  cf::Result<void, int> res;
  EXPECT_DEATH(static_cast<void>(res.error()), "");
}
// ---------- End of Death Test Cases for cf::Result<void, E> ----------
