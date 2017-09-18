#include "base/init.h"
#include "base/wall_timer.h"
#include "mcts/mcts.h"

DEFINE_int32(num_trials, 100, "Num trials");

int main(int argc, char** argv) {
  Init(&argc, &argv);
  HexState<11> game;
  game.SetPiece(1, 1);
  HexTree<11> tree(game);
  for (int i = 0; i < FLAGS_num_trials; ++i) {
    tree.RunSimulation();
  }
  return 0;
}
