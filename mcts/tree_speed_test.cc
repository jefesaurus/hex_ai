#include <gperftools/profiler.h>

#include "base/init.h"
#include "base/wall_timer.h"
#include "mcts/mcts.h"

DEFINE_int32(num_trials, 100, "Num trials");
DEFINE_string(profile_path, "", "Profiler output path");

int main(int argc, char** argv) {
  Init(&argc, &argv);
  HexState<11> game;
  game.SetPiece(1, 1);
  HexTree<11> tree(game);
  if (!FLAGS_profile_path.empty()) {
    ProfilerStart(FLAGS_profile_path.c_str());
  }
  for (int i = 0; i < FLAGS_num_trials; ++i) {
    tree.RunSimulation();
  }
  if (!FLAGS_profile_path.empty()) {
    ProfilerStop();
  }
  return 0;
}
