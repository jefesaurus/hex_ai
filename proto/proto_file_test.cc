#include "proto/proto_file.h"
#include "base/googletest.h"
#include "proto/test.pb.h"

hex_ai::proto::HelloProto MakeTestProto() {
  hex_ai::proto::HelloProto proto;
  proto.set_number(5);
  proto.add_vals(0);
  proto.add_vals(1);
  proto.add_vals(2);
  proto.add_vals(3);
  proto.add_vals(4);
  proto.set_label("hello");
  return proto;
}

TEST(ProtoFile, ReadTextFormat) {
  auto maybe_proto = ReadTextProto<hex_ai::proto::HelloProto>(
      "proto/testdata/test_text_format.txtpb");
  ASSERT_TRUE(maybe_proto);
  const auto& proto_back = *maybe_proto;

  const auto proto = MakeTestProto();
  EXPECT_EQ(proto_back.number(), proto.number());
  ASSERT_EQ(proto_back.vals_size(), proto.vals_size());
  for (int i = 0; i < proto_back.vals_size(); ++i) {
    EXPECT_EQ(proto_back.vals(i), proto.vals(i));
  }
  EXPECT_EQ(proto_back.label(), proto.label());
}

TEST(ProtoFile, WriteTextFormat) {
  const auto proto = MakeTestProto();
  bool result = WriteTextProto(proto, "/tmp/test_text_format.txtpb");
  EXPECT_TRUE(result);
  auto maybe_proto_back =
      ReadTextProto<hex_ai::proto::HelloProto>("/tmp/test_text_format.txtpb");
  ASSERT_TRUE(maybe_proto_back);
  const auto& proto_back = *maybe_proto_back;
  EXPECT_EQ(proto_back.number(), proto.number());
  ASSERT_EQ(proto_back.vals_size(), proto.vals_size());
  for (int i = 0; i < proto_back.vals_size(); ++i) {
    EXPECT_EQ(proto_back.vals(i), proto.vals(i));
  }
  EXPECT_EQ(proto_back.label(), proto.label());
}

TEST(ProtoFile, BinaryFormat) {
  // Write test
  const auto proto = MakeTestProto();
  bool result = WriteBinaryProto(proto, "/tmp/test_format.binpb");
  EXPECT_TRUE(result);

  // Read test
  auto maybe_proto_back =
      ReadBinaryProto<hex_ai::proto::HelloProto>("/tmp/test_format.binpb");
  ASSERT_TRUE(maybe_proto_back);

  const auto& proto_back = *maybe_proto_back;
  EXPECT_EQ(proto_back.number(), proto.number());
  ASSERT_EQ(proto_back.vals_size(), proto.vals_size());
  for (int i = 0; i < proto_back.vals_size(); ++i) {
    EXPECT_EQ(proto_back.vals(i), proto.vals(i));
  }
  EXPECT_EQ(proto_back.label(), proto.label());
}

GTEST_MAIN();
