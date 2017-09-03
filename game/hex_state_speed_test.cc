#include "base/init.h"
#include "base/wall_timer.h"
#include "game/hex_state.h"

#include <random>

int ReadFromState(const PackedHexState<11>& state,
                  const std::vector<std::pair<int, int>>& coords) {
  int num_white(0);
  for (const auto& coord : coords) {
    if (state.GetCell(coord.first, coord.second) == CellState::kBlack) {
      ++num_white;
    }
  }
  return num_white;
}

int ReadFromState(const HexState<11>& state,
                  const std::vector<std::pair<int, int>>& coords) {
  int num_white(0);
  for (const auto& coord : coords) {
    if (state.GetCell(coord.first, coord.second) == CellState::kBlack) {
      ++num_white;
    }
  }
  return num_white;
}

void SetStateToBlack(const std::vector<std::pair<int, int>>& coords,
                     PackedHexState<11>* state) {
  for (const auto& coord : coords) {
    state->SetCell(coord.first, coord.second, CellState::kBlack);
  }
}

void SetStateToBlack(const std::vector<std::pair<int, int>>& coords,
                     HexState<11>* state) {
  for (const auto& coord : coords) {
    state->SetCell(coord.first, coord.second, CellState::kBlack);
  }
}

int main(int argc, char** argv) {
  Init(&argc, &argv);

  // Trying to compare access speeds in a packed bits structure.
  PackedHexState<11> state;
  HexState<11> state_control;
  LOG()->info("Size of packed state: {}", sizeof(state));
  LOG()->info("Size of normal state: {}", sizeof(state_control));

  std::mt19937 rng;
  rng.seed(0);
  std::uniform_int_distribution<uint32_t> uniform(0, 10);
  std::vector<std::pair<int, int>> coords;
  const int kCount = 100000000;
  for (int i = 0; i < kCount; ++i) {
    coords.emplace_back(uniform(rng), uniform(rng));
  }

  WallTimer timer;
  timer.Start();
  SetStateToBlack(coords, &state);
  timer.Stop();
  LOG()->info("Random write packed: {}", timer.Elapsed());
  timer.Start();
  SetStateToBlack(coords, &state_control);
  timer.Stop();
  LOG()->info("Random write normal: {}", timer.Elapsed());
  timer.Start();
  int result_packed = ReadFromState(state, coords);
  timer.Stop();
  LOG()->info("Random read packed: {}", timer.Elapsed());
  timer.Start();
  int result_normal = ReadFromState(state_control, coords);
  timer.Stop();
  LOG()->info("Random read normal: {}", timer.Elapsed());
  if (result_packed == result_normal) {
    LOG()->info("Need to use the results so they don't get elided.");
  }
  return 0;
}
