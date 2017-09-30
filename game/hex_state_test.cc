#include "game/hex_state.h"
#include "base/googletest.h"
#include "base/init.h"

TEST(HexStateTest, RandomPlayout) {
  HexState<5> state;
  std::vector<int> moves{11, 19, 20, 10, 24, 21, 14, 23, 5, 12, 8,  4,
                         22, 13, 18, 3,  0,  15, 7,  2,  1, 16, 17, 9};
  for (int i = 0; i < moves.size(); ++i) {
    state.SetPiece(moves[i]);
  }
  EXPECT_TRUE(state.GameIsOver());
  EXPECT_EQ(state.Winner(), PieceType::kVertical);
}

TEST(HexStateTest, HorizontalPlayout1) {
  HexState<5> state;
  state.SetPiece(0, 0);
  state.SetPiece(1, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 1);
  state.SetPiece(1, 1);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 2);
  state.SetPiece(1, 2);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 3);
  state.SetPiece(1, 3);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 4);
  EXPECT_TRUE(state.GameIsOver());
  EXPECT_EQ(state.Winner(), PieceType::kHorizontal);
}

TEST(HexStateTest, HorizontalPlayout2) {
  HexState<5> state;
  state.SetPiece(0, 4);
  state.SetPiece(1, 4);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 3);
  state.SetPiece(1, 3);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 1);
  state.SetPiece(1, 1);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 0);
  state.SetPiece(1, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 2);
  EXPECT_TRUE(state.GameIsOver());
  EXPECT_EQ(state.Winner(), PieceType::kHorizontal);
}

TEST(HexStateTest, HorizontalPlayout3) {
  HexState<5> state;
  state.SetPiece(0, 0);
  state.SetPiece(1, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 1);
  state.SetPiece(1, 1);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 2);
  state.SetPiece(1, 2);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 3);
  state.SetPiece(1, 3);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(0, 4);
  EXPECT_TRUE(state.GameIsOver());
  EXPECT_EQ(state.Winner(), PieceType::kHorizontal);
}

TEST(HexStateTest, VerticalPlayout) {
  HexState<5> state;
  state.SetPiece(0, 1);
  state.SetPiece(0, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(1, 1);
  state.SetPiece(1, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(2, 1);
  state.SetPiece(2, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(3, 1);
  state.SetPiece(3, 0);
  EXPECT_FALSE(state.GameIsOver());
  state.SetPiece(4, 1);
  state.SetPiece(4, 0);
  EXPECT_TRUE(state.GameIsOver());
  EXPECT_EQ(state.Winner(), PieceType::kVertical);
}

GTEST_MAIN();
