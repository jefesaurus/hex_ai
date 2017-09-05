#include "base/init.h"
#include "base/wall_timer.h"
#include "game/hex_state.h"

#include <random>

void RandomPlayouts(int num_trials) {
  std::vector<int> moves;
  for (int i = 0; i < HexState<11>::kNumCells; ++i) {
    moves.push_back(i);
  }
  std::array<PieceType, 2> players = {PieceType::kVertical,
                                      PieceType::kHorizontal};

  std::vector<std::vector<int>> move_sets(num_trials);
  for (int i = 0; i < num_trials; ++i) {
    move_sets[i] = moves;
    std::random_shuffle(move_sets[i].begin(), move_sets[i].end());
  }

  WallTimer timer;

  std::array<int, 3> wins = {0, 0, 0};
  timer.Start();
  for (int i = 0; i < num_trials; ++i) {
    HexState<11> state;
    int move_number(0);
    while (!state.GameIsOver()) {
      state.SetPiece(move_sets[i][move_number], players[move_number % 2]);
      move_number++;
    }
    ++wins[as_underlying(state.Winner())];
  }
  timer.Stop();
  LOG(INFO) << std::vector<int>(wins.begin(), wins.end());
  LOG(INFO) << timer.Elapsed();
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  RandomPlayouts(1000000);
  return 0;
}
