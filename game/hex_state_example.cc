#include "base/init.h"
#include "game/hex_state.h"

void HorizontalPlayout() {
  HexState<5> state;
  state.SetHorizontalPiece(0, 0);
  CHECK(!state.GameIsOver());
  state.SetHorizontalPiece(0, 1);
  CHECK(!state.GameIsOver());
  state.SetHorizontalPiece(0, 2);
  CHECK(!state.GameIsOver());
  state.SetHorizontalPiece(0, 3);
  CHECK(!state.GameIsOver());
  state.SetHorizontalPiece(0, 4);
  CHECK(state.GameIsOver()) << state;
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void VerticalPlayout() {
  HexState<5> state;
  state.SetVerticalPiece(0, 0);
  CHECK(!state.GameIsOver());
  state.SetVerticalPiece(1, 0);
  CHECK(!state.GameIsOver());
  state.SetVerticalPiece(2, 0);
  CHECK(!state.GameIsOver());
  state.SetVerticalPiece(3, 0);
  CHECK(!state.GameIsOver());
  state.SetVerticalPiece(4, 0);
  CHECK(state.GameIsOver()) << state;
  CHECK_EQ(state.Winner(), PieceType::kVertical);
}

void TestPrint() {
  {
    HexState<5> state;
    state.SetVerticalPiece(0, 0);
    state.SetVerticalPiece(1, 1);
    state.SetVerticalPiece(2, 2);
    state.SetVerticalPiece(3, 3);
    state.SetVerticalPiece(4, 4);
    state.SetHorizontalPiece(0, 4);
    state.SetHorizontalPiece(1, 3);
    state.SetHorizontalPiece(3, 1);
    state.SetHorizontalPiece(4, 0);
    LOG(INFO) << state;
  }
  {
    HexState<19> state;
    LOG(INFO) << state;
  }
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  HorizontalPlayout();
  VerticalPlayout();
  TestPrint();
  return 0;
}
