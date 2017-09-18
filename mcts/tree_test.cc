#include "base/init.h"
#include "base/wall_timer.h"
#include "mcts/mcts.h"

DEFINE_int32(num_trials, 100, "Num trials");

int main(int argc, char** argv) {
  Init(&argc, &argv);
  HexState<11> game;
  while (!game.GameIsOver()) {
    HexTree<11> tree(game);
    for (int i = 0; i < FLAGS_num_trials; ++i) {
      tree.RunSimulation();
    }
    const auto best_move = tree.GetBestMove();
    LOG(INFO) << game.ToMove() << " plays: " << best_move.first
              << " thinks it will win: " << best_move.second;
    game.SetPiece(best_move.first);
    LOG(INFO) << game;
  }
  LOG(INFO) << "Winner is: " << game.Winner();
  return 0;
}
