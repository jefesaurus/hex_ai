#include "base/init.h"
#include "game/hex_state.h"

void RandomPlayout1() {
  HexState<5> state;
  std::vector<int> moves{11, 19, 20, 10, 24, 21, 14, 23, 5, 12, 8,  4,
                         22, 13, 18, 3,  0,  15, 7,  2,  1, 16, 17, 9};
  for (int i = 0; i < moves.size(); ++i) {
    state.SetPiece(moves[i]);
  }
  CHECK(state.GameIsOver()) << state;
  CHECK_EQ(state.Winner(), PieceType::kVertical);
}

void HorizontalPlayout1() {
  HexState<5> state;
  state.SetPiece(0, 0);
  state.SetPiece(1, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 1);
  state.SetPiece(1, 1);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 2);
  state.SetPiece(1, 2);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 3);
  state.SetPiece(1, 3);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 4);
  CHECK(state.GameIsOver());
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void HorizontalPlayout2() {
  HexState<5> state;
  state.SetPiece(0, 4);
  state.SetPiece(1, 4);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 3);
  state.SetPiece(1, 3);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 1);
  state.SetPiece(1, 1);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 0);
  state.SetPiece(1, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 2);
  CHECK(state.GameIsOver());
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void HorizontalPlayout3() {
  HexState<5> state;
  state.SetPiece(0, 0);
  state.SetPiece(1, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 1);
  state.SetPiece(1, 1);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 2);
  state.SetPiece(1, 2);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 3);
  state.SetPiece(1, 3);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 4);
  CHECK(state.GameIsOver());
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void VerticalPlayout() {
  HexState<5> state;
  state.SetPiece(0, 1);
  state.SetPiece(0, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(1, 1);
  state.SetPiece(1, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(2, 1);
  state.SetPiece(2, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(3, 1);
  state.SetPiece(3, 0);
  CHECK(!state.GameIsOver());
  state.SetPiece(4, 1);
  state.SetPiece(4, 0);
  CHECK(state.GameIsOver());
  CHECK_EQ(state.Winner(), PieceType::kVertical);
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  RandomPlayout1();
  HorizontalPlayout1();
  HorizontalPlayout2();
  HorizontalPlayout3();
  VerticalPlayout();
  return 0;
}
