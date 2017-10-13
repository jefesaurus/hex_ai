#include "game/hex_board.h"
#include "base/googletest.h"

TEST(HexBoardTest, HorizontalPlayout1) {
  HexBoard<5> state;
  state.SetHorizontalPiece(0, 0);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 2);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 3);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 4);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kHorizontal);
}

TEST(HexBoardTest, HorizontalPlayout2) {
  HexBoard<5> state;
  state.SetHorizontalPiece(0, 4);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 3);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 0);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetHorizontalPiece(0, 2);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kHorizontal);
}

TEST(HexBoardTest, VerticalPlayout) {
  HexBoard<5> state;
  state.SetVerticalPiece(0, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetVerticalPiece(1, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetVerticalPiece(2, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetVerticalPiece(3, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kEmpty);
  state.SetVerticalPiece(4, 1);
  EXPECT_EQ(state.ConnectionStatus(), PieceType::kVertical);
}

GTEST_MAIN();
