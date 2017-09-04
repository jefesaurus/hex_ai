#include "base/init.h"
#include "game/hex_state.h"

#include <bitset>

int main(int argc, char** argv) {
  Init(&argc, &argv);
  HexState<9> state;
  LOG(INFO, "{}", PieceType::kHorizontal);
  LOG(INFO, "{}", PieceType::kVertical);
  state.SetPiece(0, 0, PieceType::kHorizontal);
  state.SetPiece(1, 0, PieceType::kHorizontal);
  state.SetPiece(1, 1, PieceType::kVertical);
  state.SetPiece(0, 1, PieceType::kHorizontal);
  LOG(INFO, "{}", state);
  return 0;
}
