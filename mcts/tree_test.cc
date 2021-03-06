#include "base/init.h"
#include "base/wall_timer.h"
#include "mcts/mcts.h"

DEFINE_int32(num_trials, 100, "Num games to play out.");
DEFINE_int32(num_iters, 100, "Num tree iterations for each move.");
DEFINE_bool(view_boards, false, "Print boards");

int main(int argc, char** argv) {
  constexpr int kGameSize = 7;
  Init(&argc, &argv);
  std::unordered_map<int, int> winners;
  for (int i = 0; i < FLAGS_num_trials; ++i) {
    HexState<kGameSize> game;
    while (!game.GameIsOver()) {
      HexTree<kGameSize> tree(game);
      for (int j = 0; j < FLAGS_num_iters; ++j) {
        tree.RunSimulation();
      }
      const auto best_move = tree.GetBestMove();
      game.MakeMove(best_move.first);
      if (FLAGS_view_boards) {
        LOG(INFO) << FlipPlayer(game.ToMove()) << " plays " << best_move.first
                  << ", and thinks it has this chance of winning: "
                  << best_move.second;
        LOG(INFO) << game;
      }
    }
    LOG(INFO) << "Game " << i << " result: winner is " << game.Winner();
    winners[as_underlying(game.Winner())]++;
  }
  LOG(INFO) << winners;
  return 0;
}
