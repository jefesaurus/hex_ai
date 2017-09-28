#include "base/googletest.h"

#include "proto/proto_test.pb.h"

TEST(ProtoTest, Smoke) {
  hex_ai::proto::Test test;
  *test.mutable_label() = "asdf";
  test.set_number(1);
  LOG(INFO) << test.DebugString();
  EXPECT_EQ(test.label(), "asdf");
  EXPECT_EQ(test.number(), 1);
}

GTEST_MAIN();
