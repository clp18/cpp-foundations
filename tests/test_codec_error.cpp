#include <gtest/gtest.h>

#include <cctype>
#include <string>

#include "cpp_foundations/codec_error.hpp"

// Create a test fixture class inheriting from testing::TestWithParam
class StatusTest : public ::testing::TestWithParam<cf::CodecError> {};

// Define the parameterized test case using TEST_P
TEST_P(StatusTest, ConvertsToStringCorrectly) {
  // Get the current enum value for this test iteration
  cf::CodecError current_status = GetParam();

  // Perform your assertions
  std::string_view result = ToString(current_status);
  EXPECT_NE(result, "Unknown Error");
}

// Builds a GoogleTest-safe test name from ToString (letters and digits only)
std::string ParamName(const ::testing::TestParamInfo<cf::CodecError>& info) {
  std::string name;
  for (char c : cf::ToString(info.param)) {
    if (std::isalnum(static_cast<unsigned char>(c)) != 0) {
      name += c;
    } else {
      name += '_';
    }
  }
  return name;
}

// Instantiate the test suite with all the enum values
INSTANTIATE_TEST_SUITE_P(AllStatusValues, StatusTest,
                         ::testing::Values(cf::CodecError::kBufferTooShort,
                                           cf::CodecError::kInvalidWidth),
                         ParamName);

// An out-of-range value falls through to the default string
TEST(CodecErrorTest, UnknownValueReturnsUnknownError) {
  EXPECT_EQ(cf::ToString(static_cast<cf::CodecError>(100)), "Unknown Error");
}
