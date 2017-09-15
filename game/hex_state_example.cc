#include "base/init.h"
#include "game/hex_state.h"

void RandomPlayout1() {
  HexState<5> state;
  std::vector<int> moves{11, 19, 20, 10, 24, 21, 14, 23, 5,  12, 8, 4, 22,
                         13, 18, 3,  0,  15, 7,  2,  1,  16, 17, 9, 6};
  std::array<PieceType, 2> players{PieceType::kHorizontal,
                                   PieceType::kVertical};
  for (int i = 0; i < moves.size(); ++i) {
    state.SetPiece(moves[i], players[i % 2]);
  }
  CHECK(state.GameIsOver()) << state;
  CHECK_EQ(state.Winner(), PieceType::kVertical);
}

void HorizontalPlayout1() {
  HexState<5> state;
  LOG(INFO) << state.Hash();
  state.SetPiece(0, 0, PieceType::kHorizontal);
  LOG(INFO) << state.Hash();
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 1, PieceType::kHorizontal);
  LOG(INFO) << state.Hash();
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 2, PieceType::kHorizontal);
  LOG(INFO) << state.Hash();
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 3, PieceType::kHorizontal);
  LOG(INFO) << state.Hash();
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 4, PieceType::kHorizontal);
  LOG(INFO) << state.Hash();
  CHECK(state.GameIsOver());  // << state;
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void HorizontalPlayout2() {
  HexState<5> state;
  state.SetPiece(0, 4, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 3, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 1, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 0, PieceType::kHorizontal);
  CHECK(!state.GameIsOver());
  state.SetPiece(0, 2, PieceType::kHorizontal);
  CHECK(state.GameIsOver());  // << state;
  CHECK_EQ(state.Winner(), PieceType::kHorizontal);
}

void HorizontalPlayout3() {
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
  CHECK(state.GameIsOver());  // << state;
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
  CHECK(state.GameIsOver());  // << state;
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
  RandomPlayout1();
  HorizontalPlayout1();
  HorizontalPlayout2();
  HorizontalPlayout3();
  VerticalPlayout();
  TestPrint();
  return 0;
}
