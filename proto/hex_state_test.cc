#include "proto/hex_state.pb.h"
#include "base/googletest.h"

TEST(HexStateTest, Smoke) {
  hex_ai::proto::HexState state;
  state.set_size(11);
  state.set_to_move(hex_ai::proto::PieceType::kHorizontal);
  state.add_positions(hex_ai::proto::PieceType::kHorizontal);
  state.add_positions(hex_ai::proto::PieceType::kEmpty);
  state.add_positions(hex_ai::proto::PieceType::kVertical);
  EXPECT_EQ(state.size(), 11);
  EXPECT_EQ(state.to_move(), hex_ai::proto::PieceType::kHorizontal);
  ASSERT_EQ(state.positions_size(), 3);
  EXPECT_EQ(state.positions(0), hex_ai::proto::PieceType::kHorizontal);
  EXPECT_EQ(state.positions(1), hex_ai::proto::PieceType::kEmpty);
  EXPECT_EQ(state.positions(2), hex_ai::proto::PieceType::kVertical);
}

GTEST_MAIN();
