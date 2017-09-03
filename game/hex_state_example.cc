#include "base/init.h"
#include "game/hex_state.h"

#include <bitset>

int main(int argc, char** argv) {
  Init(&argc, &argv);
  HexState<9> state;
  LOG()->info(sizeof(state));
  LOG()->info(CellState::kWhite);
  LOG()->info(CellState::kBlack);
  state.SetCell(0, 0, CellState::kWhite);
  state.SetCell(1, 0, CellState::kWhite);
  state.SetCell(1, 1, CellState::kWhite);
  state.SetCell(0, 1, CellState::kWhite);
  LOG()->info(state);
  return 0;
}
