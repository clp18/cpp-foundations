#include <gtest/gtest.h>

#include <memory>  // std::unique_ptr
#include <string>
#include <type_traits>

#include "cpp_foundations/result.hpp"

// TODO(clp18): Result tests are incomplete. Still to cover:
//   - value() and error() overloads: &, const&, &&, const&&
//   - Unexpected::error() const& and &&
//   - operator bool, copy and move, Result<int, int>,
//     converting and braced-list constructors

// ----- Compile-time checks -----
using IntResult = cf::Result<int, std::string>;
static_assert(std::is_convertible_v<int, IntResult>,
              "Return value -  must work");
static_assert(std::is_convertible_v<cf::Unexpected<std::string>, IntResult>,
              "Return Unexpected(e) - must work");
static_assert(!std::is_constructible_v<IntResult, std::string>,
              "A bare E must go through cf::Unexpected");
static_assert(std::is_copy_constructible_v<IntResult>);

static_assert(
    !std::is_copy_constructible_v<cf::Result<std::unique_ptr<int>, int>>);

constexpr cf::Result<int, int> kValue = 5;
static_assert(kValue.has_value() && kValue.value() == 5);

constexpr cf::Result<int, int> kError = cf::Unexpected(6);
static_assert(!kError.has_value() && kError.error() == 6);

// ----- Constructor Tests -----
TEST(ResultTest, ConstructorWithLvalue) {
  const std::string text = "hello";
  cf::Result<std::string, int> res(text);
  ASSERT_TRUE(res.has_value());
  EXPECT_EQ(res.value(), "hello");
  EXPECT_EQ(text, "hello");  // copied, not moved
}

TEST(ResultTest, ConstructorWithRvalue) {
  cf::Result<std::unique_ptr<int>, int> res(std::make_unique<int>(7));
  ASSERT_TRUE(res.has_value());
  EXPECT_EQ(*res.value(), 7);
}

// ----- Operator Test: has_value -----

// ----- Operator Test: value() -----

// ----- Operator Test: error() -----

// ----- Operator Test: Unexpected -----
