#pragma once
#include "base/wall_timer.h"
#include "game/hex_state.h"

template <int Size>
class MCTS {
 public:
  void GetMove(const Duration& computation_time) {
    TimePoint start_time = timer.Start();
    while (timer.ElapsedDuration() < computation_time) {
      // Run Simulation
    }
  }

  void RunSimulation() {
    // Copy current state and run a playout.
    auto current_board = current_state_;
    while (!current_board.GameIsOver()) {
    }
  }

 private:
  WallTimer timer_;
  HexState<Size> current_state_;

  // Stratify by current to-play to reduce set loading.
  std::array<std::unordered_map<uint64_t, std::unique_ptr<TreeNode>>, 3>
      visited_;
};

struct TreeNode {
  std::array<TreeNode*, Size * Size> children;

  static constexpr double kC = std::sqrt(2.0);

  void ChooseMove(const std::array<bool, Size * Size>& available_moves) {
    double log_total = 0;
    for (int i = 0; i < available_moves.size(); ++i) {
      if (available_moves[i]) {
        if (!children[i]) {
          // This is a branch we haven't tried before.
          next_hash = current_state.GetNextHash(i);
          auto map_it = visited_[player].find(next_hash);
          if (map_it == visited_[player].end()) {
            // Create new tree node
            visited_[player][next_hash] = std::make_unique(TreeNode);
          } else {
            // We actually have visited this node before.
          }
          if (visited_

        } else {
        }
      }
    }
  }

  bool leaf;

  int num_wins;
  int num_visits;
};

void TraverseTree() {
  HexState<Size> current_state;
  TreeNode* current_node;
  std::array<bool, Size * Size> available_moves;
  while (current_node->num_visits > 0) {
    int move = current_node->ChooseMove();
    available_moves[move] = false;
    current_state.SetPiece(move, player);
  }
  if (!current_state.GameIsOver()) {
    // Expand and do a random playout.
  }
}

void ExpandTree() {}
