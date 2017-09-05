#include "base/init.h"
#include "game/hex_state.h"

void HorizontalPlayout() {
  HexState<5> state;
  state.SetPiece(0, 0, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 1, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 2, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 3, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 4, PieceType::kHorizontal);
  CHECK(state.GameIsOver()) << state;
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void VerticalPlayout() {
  HexState<5> state;
  state.SetPiece(0, 0, PieceType::kVertical);
  CHECK(!state.GameIsOver());
  state.SetPiece(1, 0, PieceType::kVertical);
  CHECK(!state.GameIsOver());
  state.SetPiece(2, 0, PieceType::kVertical);
  CHECK(!state.GameIsOver());
  state.SetPiece(3, 0, PieceType::kVertical);
  CHECK(!state.GameIsOver());
  state.SetPiece(4, 0, PieceType::kVertical);
  CHECK(state.GameIsOver()) << state;
  CHECK_EQ(state.Winner(), PieceType::kVertical);
}

void TestPrint() {
  {
    HexState<5> state;
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
