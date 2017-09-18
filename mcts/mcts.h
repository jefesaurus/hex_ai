#pragma once
#include "base/wall_timer.h"
#include "game/hex_state.h"

template <int Size>
class HexTree {
 private:
  // The state required for one simulated play.
  struct SimulationState {
    PieceType to_move;
    HexState<Size> state;
    std::array<bool, Size * Size> available_moves;

    // If expand is true, that means we're still looking down the part of the
    // tree we've seen befor
    bool expand;
    int breadcrumb_size;
    std::array<TreeNode*, Size * Size> breadcrumb;
  };

 public:
  HexTree(HexState<Size> current_state) {}

  void AddMoveToSim(int move, SimulationState* sim) {
    sim->available_moves[move] = false;
    sim->state.SetPiece(move, sim->to_move);
    sim->to_move = FlipPlayer(sim->to_move);
  }

  void StepTreeExpansion(SimulationState* sim) {
    DCHECK(!sim->state.GameIsOver());
    DCHECK(sim->expand);
    DCHECK_GT(sim->breadcrumb_size, 0);
    DCHECK_LE(sim->breadcrumb_size, Size * Size);

    auto& current_node = sim->breadcrumb[sim->breadcrumb_size - 1];
    static const auto& hasher = HexState<Size>::Hasher();
    auto& children = current_node->children;

    for (int i = 0; i < sim->available_moves.size(); ++i) {
      if (available_moves[i]) {
        if (!children[i]) {
          // This is a branch we haven't tried before, check if we've seen the
          // resulting state before.
          uint64_t next_hash = sim->state.Hash();
          hasher.FlipPiece(i, sim->to_move, &next_hash);
          auto map_it = (*visited_)[player].find(next_hash);
          if (map_it == visited_[player].end()) {
            // Unexpanded moves have infinite score, so choose this.
            AddMoveToSim(i, sim);
            // Create a new tree node
            auto new_map_it =
                visited_[player].emplace(std::make_pair(next_hash, TreeNode()));
            // This is the end of the expansion.
            sim->expand = false;
            children[i] = &(new_map_it.first.second);
            sim->breadcrumb[sim->breadcrumb_size++] =
                &(new_map_it.first.second);
            return;
          } else {
            // We actually have visited this node before, update the branch
            // structure;
            children[i] = &(map_it.second);
          }
        } else {
          // We've already taken this branch before.
        }
      }
    }
  }

  void StepRandom(SimulationState* sim) {
    DCHECK(!sim->state.GameIsOver());
    DCHECK(!sim->expand);
    std::uniform_int_distribution<> dis(1, sim->state.EmptySpaces());
    int available_move_index = dis(gen);
    int move = 0;
    for (; move < sim->available_moves.size() && available_move_index > 0;
         ++i) {
      if (sim->available_moves[move]) {
        --available_move_index;
      }
    }
  }

  void StepSimulation(SimulationState* sim) {
    if (sim->expand) {
      StepTreeExpansion(sim);
    } else {
      StepRandom(sim);
    }
  }

  void RunSimulation() {
    SimulationState sim;
    while (!sim.state.GameIsOver()) {
      StepSimulation(&sim);
    }

    // If the root won, winner_mod_2 = false
    bool root_wins = root.to_move == sim.state.Winner();

    // Backprop.
    const auto& breadcrumb = sim.breadcrumb;
    for (int i = 0; i < sim.breadcrumb_size; ++i) {
      ++breadcrumb[i]->num_visits;
      if (i % 2 != root_wins) {
        ++breadcrumb[i]->num_wins;
      }
    }
  }

  // The base state for this game tree.
  SimulationState root;
  HexState<Size> root_state_;
  PieceType root_to_move_;

  // The corresponding tree node.
  TreeNode* root_node_;

  std::array<bool, Size * Size> root_available_moves_;

  // Node storage.
  std::array<std::unordered_map<uint64_t, TreeNode>, 3> visited_;

  std::mt19937 gen;
};

struct TreeNode {
  std::array<TreeNode*, Size * Size> children;
  int num_wins;
  int num_visits;
};
