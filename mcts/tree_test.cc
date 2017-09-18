#include "base/init.h"
#include "base/wall_timer.h"
#include "mcts/mcts.h"

DEFINE_int32(num_trials, 100, "Num games to play out.");
DEFINE_int32(num_iters, 100, "Num tree iterations for each move.");

int main(int argc, char** argv) {
  Init(&argc, &argv);
  std::unordered_map<int, int> winners;
  for (int i = 0; i < FLAGS_num_trials; ++i) {
    HexState<11> game;
    while (!game.GameIsOver()) {
      HexTree<11> tree(game);
      for (int j = 0; j < FLAGS_num_iters; ++j) {
        tree.RunSimulation();
      }
      const auto best_move = tree.GetBestMove();
      game.SetPiece(best_move.first);
    }
    LOG(INFO) << "Game " << i << " result: winner is " << game.Winner();
    winners[as_underlying(game.Winner())]++;
  }
  LOG(INFO) << winners;
  return 0;
}
