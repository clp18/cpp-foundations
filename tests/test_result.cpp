#include <gtest/gtest.h>

#include <memory>  // std::unique_ptr
#include <string>
#include <type_traits>

#include "cpp_foundations/codec_error.hpp"
#include "cpp_foundations/result.hpp"

// ---------- Test Cases for cf::Result<T, E> ----------

// TODO(clp18): ResultTest is incomplete. Still to cover:
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

// ---------- End of Test Cases for cf::Result<T, E> ----------

// ---------- Test Cases for cf::Result<void, E> ----------

// TODO(clp18): VoidResultTest is incomplete. Still to cover:
//   - error() overloads: const&, &&, const&&
//   - operator bool on an error, copy and move, value() on a const result

// Helper Functions for testing
namespace {
using VoidResult = cf::Result<void, cf::CodecError>;
VoidResult ReturnSuccess() { return {}; }
VoidResult ReturnError() {
  return cf::Unexpected(cf::CodecError::kInvalidWidth);
}
}  // namespace

// Compile-time Checks
static_assert(std::is_default_constructible_v<VoidResult>);
constexpr VoidResult kVoid{};
static_assert(kVoid.has_value());
static_assert(!std::is_constructible_v<VoidResult, cf::CodecError>,
              "A bare E must go through cf::Unexpected");
static_assert(!std::is_constructible_v<VoidResult, int>,
              "There is no value to construct from");

TEST(VoidResultTest, ReturnBracesIsSuccess) {
  VoidResult res = ReturnSuccess();
  EXPECT_TRUE(res.has_value());
  EXPECT_TRUE(static_cast<bool>(res));
  res.value();  // must not abort
}

TEST(VoidResultTest, ReturnUnexpectedIsError) {
  VoidResult res = ReturnError();
  ASSERT_FALSE(res.has_value());
  EXPECT_EQ(res.error(), cf::CodecError::kInvalidWidth);
}
// ---------- End of Test Cases for cf::Result<void, E> ----------
