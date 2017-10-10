#include "game/hex_state.h"
#include "base/googletest.h"
#include "base/init.h"

TEST(HexStateTest, Swap) {
  HexState<5> state;
  auto moves = state.AvailableMoves();
  EXPECT_TRUE(moves[10]);
  state.MakeMove(10);
  auto start_hash = state.Hash();
  moves = state.AvailableMoves();
  ASSERT_TRUE(moves[10]);
  state.MakeMove(10);
  moves = state.AvailableMoves();
  auto after_swap_hash = state.Hash();
  EXPECT_NE(start_hash, after_swap_hash);
  EXPECT_FALSE(moves[10]);
}

TEST(HexStateTest, SwapMoveNumber) {
  HexState<5> state;
  LOG(INFO) << state.MoveNumber();
  LOG(INFO) << state.AvailableMoves();
  state.MakeMove(10);
  LOG(INFO) << state;
  LOG(INFO) << state.MoveNumber();
  LOG(INFO) << state.AvailableMoves();
  state.MakeMove(11);
  LOG(INFO) << state;
  LOG(INFO) << state.MoveNumber();
  LOG(INFO) << state.AvailableMoves();
}

GTEST_MAIN();
