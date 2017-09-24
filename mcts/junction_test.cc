#include <junction/ConcurrentMap_Grampa.h>

#include "base/googletest.h"

TEST(Junction, Grampa) {
  std::string hw = "hello world";
  junction::ConcurrentMap_Grampa<uint64_t, std::string*> string_map;
  string_map.assign(14, &hw);
  auto* foo = string_map.get(14);
  EXPECT_EQ(*foo, hw);
}

GTEST_MAIN();
