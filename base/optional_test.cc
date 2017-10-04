#include <optional>

#include "base/googletest.h"
#include "base/logging.h"

// Test for C++17 functionality
TEST(GTest, Invoke) {
  std::optional<int> maybe_int;
  EXPECT_FALSE(maybe_int);
  maybe_int = 5;
  EXPECT_TRUE(maybe_int);
  EXPECT_EQ(*maybe_int, 5);
}

GTEST_MAIN();
