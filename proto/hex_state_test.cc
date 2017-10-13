#include "base/googletest.h"
#include "proto/hex_state.pb.h"

TEST(HexStateTest, Smoke) {
  hex_ai::proto::HexState state;
  state.set_to_move(hex_ai::proto::PieceType::kHorizontal);
  auto& board = *state.mutable_board();
  board.set_size(11);
  board.add_positions(hex_ai::proto::PieceType::kHorizontal);
  board.add_positions(hex_ai::proto::PieceType::kEmpty);
  board.add_positions(hex_ai::proto::PieceType::kVertical);
  EXPECT_EQ(board.size(), 11);
  EXPECT_EQ(state.to_move(), hex_ai::proto::PieceType::kHorizontal);
  ASSERT_EQ(board.positions_size(), 3);
  EXPECT_EQ(board.positions(0), hex_ai::proto::PieceType::kHorizontal);
  EXPECT_EQ(board.positions(1), hex_ai::proto::PieceType::kEmpty);
  EXPECT_EQ(board.positions(2), hex_ai::proto::PieceType::kVertical);
}

GTEST_MAIN();
