#include "base/googletest.h"
#include "base/logging.h"

TEST(GTest, Invoke) { LOG(INFO) << "yeay"; }

GTEST_MAIN();
