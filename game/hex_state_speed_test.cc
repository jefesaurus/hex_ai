#include "base/init.h"
#include "base/wall_timer.h"
#include "game/hex_state.h"

#include <random>

DEFINE_int32(num_trials, 100, "Num trials");

void RandomPlayouts(int num_trials) {
  std::vector<int> moves;
  for (int i = 0; i < HexState<11>::kNumCells; ++i) {
    moves.push_back(i);
  }
  std::array<PieceType, 2> players = {PieceType::kVertical,
                                      PieceType::kHorizontal};

  std::random_shuffle(moves.begin(), moves.end());

  WallTimer timer;

  std::array<int, 3> wins = {0, 0, 0};
  Duration placing_duration(0);
  for (int i = 0; i < num_trials; ++i) {
    HexState<11> state;
    int move_number(0);
    while (!state.GameIsOver()) {
      timer.Start();
      state.SetPiece(moves[move_number], players[move_number % 2]);
      timer.Stop();
      placing_duration+= timer.Elapsed();
      move_number++;
    }
    ++wins[as_underlying(state.Winner())];
  }
  LOG(INFO) << std::vector<int>(wins.begin(), wins.end());
  LOG(INFO) << "Placing duration: " << placing_duration;
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  RandomPlayouts(FLAGS_num_trials);
  return 0;
}
