#include "game/hex_io.h"
#include "base/googletest.h"

TEST(HexBoardTest, HorizontalPlayout1) {
  HexBoard<5> state;
  state.SetHorizontalPiece(0, 0);
  state.SetHorizontalPiece(0, 1);
  hex_ai::proto::HexBoard board;
  board.set_size(5);
}

TEST(HexIO, FromProto) {
  hex_ai::proto::HexBoard board;
  board.set_size(5);
  for (int i = 0; i < 5 * 5; ++i) {
    board.add_positions(hex_ai::proto::PieceType::kEmpty);
  }
  const auto board_back = FromProto<5>(board);
  EXPECT_EQ(board_back.size(), 5);
  EXPECT_EQ(board_back.ConnectionStatus(), PieceType::kEmpty);
}

TEST(HexIO, FromProtoHorizontal) {
  hex_ai::proto::HexBoard board;
  board.set_size(5);
  for (int i = 0; i < 5 * 5; ++i) {
    board.add_positions(hex_ai::proto::PieceType::kHorizontal);
  }
  const auto board_back = FromProto<5>(board);
  EXPECT_EQ(board_back.size(), 5);
  EXPECT_EQ(board_back.ConnectionStatus(), PieceType::kHorizontal);
}

TEST(HexIO, FromProtoVertical) {
  hex_ai::proto::HexBoard board;
  board.set_size(5);
  for (int i = 0; i < 5 * 5; ++i) {
    board.add_positions(hex_ai::proto::PieceType::kVertical);
  }
  const auto board_back = FromProto<5>(board);
  EXPECT_EQ(board_back.size(), 5);
  EXPECT_EQ(board_back.ConnectionStatus(), PieceType::kVertical);
}

GTEST_MAIN();
