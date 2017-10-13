#include "base/init.h"
#include "mcts/mcts.h"
#include "networks/game_state_checker/data.pb.h"

DEFINE_int32(num_trials, 100, "Num games to play out.");
DEFINE_int32(num_iters, 100, "Num tree iterations for each horizontal move.");
DEFINE_string(file_out, "", "Where to store the file with serialized data.");

void GenerateData() {
  // We'll store all of the boards we've seen.
  hex_ai::proto::HexBoards boards;
  for (int i = 0; i < FLAGS_num_trials; ++i) {
    HexState<11> game;
    while (!game.GameIsOver()) {
      HexTree<11> tree(game);
      const int num_iters = FLAGS_num_iters;
      for (int j = 0; j < num_iters; ++j) {
        tree.RunSimulation();
      }
      const auto best_move = tree.GetBestMove();
      game.MakeMove(best_move.first);
    }
    LOG(INFO) << "Game " << i << " result: winner is " << game.Winner();
    auto* board = boards.add_boards();
    board->set_size(11);
  }
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  return 0;
}
