#include "base/init.h"
#include "base/wall_timer.h"
#include "game/hex_state.h"

#include <gperftools/profiler.h>
#include <random>

DEFINE_int32(num_trials, 100, "Num trials");
DEFINE_string(profile_path, "", "Profiler output path");

/*

bazel build -c opt game:hex_state_speed_test
./bazel-bin/game/hex_state_speed_test --num_trials=1000000
--profiler_output=speed_prof.prof pprof --callgrind
./bazel-bin/game/hex_state_speed_test speed_prof.prof >
profiling_kcachegrind.txt
*/

static constexpr int GameSize = 11;

std::array<int, 3> RunPlayouts(
    const std::vector<std::vector<int>>& shuffled_moves) {
  std::array<int, 3> wins = {0, 0, 0};
  for (int i = 0; i < shuffled_moves.size(); ++i) {
    const auto& moves_current = shuffled_moves[i];
    HexState<GameSize> state;
    int move_number(0);
    while (!state.GameIsOver()) {
      state.SetPiece(moves_current[move_number]);
      move_number++;
    }
    ++wins[as_underlying(state.Winner())];
  }
  return wins;
}

void GenerateMovesetsAndPlay(int num_trials) {
  std::vector<int> moves;
  for (int i = 0; i < HexState<GameSize>::kNumCells; ++i) {
    moves.push_back(i);
  }
  std::vector<std::vector<int>> shuffled_moves(num_trials);
  for (int i = 0; i < num_trials; ++i) {
    auto& shuffled_moves_current = shuffled_moves[i];
    shuffled_moves_current = moves;
    std::random_shuffle(shuffled_moves_current.begin(),
                        shuffled_moves_current.end());
  }
  WallTimer timer;
  timer.Start();
  if (!FLAGS_profile_path.empty()) {
    ProfilerStart(FLAGS_profile_path.c_str());
  }
  const auto results = RunPlayouts(shuffled_moves);
  if (!FLAGS_profile_path.empty()) {
    ProfilerStop();
  }
  timer.Stop();
  LOG(INFO) << "Total duration: " << timer.Elapsed();
  LOG(INFO) << "Playout rate: "
            << num_trials / ToSeconds<double>(timer.Elapsed())
            << " games per second";
  LOG(INFO) << "Game results: " << results;
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  GenerateMovesetsAndPlay(FLAGS_num_trials);
  return 0;
}
