#pragma once
#include <random>
#include "base/wall_timer.h"
#include "game/hex_state.h"

template <int Size>
class HexTree {
 private:
  struct TreeNode {
    std::array<TreeNode*, Size* Size> children = {};
    int num_wins = 0;
    int num_visits = 0;
  };

  // The state required for one simulated play.
  struct SimulationState {
    HexState<Size> state;
    std::array<bool, Size * Size> available_moves;

    // If expand is true, that means we're still looking down the part of the
    // tree we've seen before
    bool expand;
    int breadcrumb_size;
    std::array<TreeNode*, Size * Size> breadcrumb;
  };

 public:
  HexTree(HexState<Size> current_state)
      : root_sim_(InitializeRootSim(current_state, &visited_)),
        root_node_(root_sim_.breadcrumb.front()) {}

  static SimulationState InitializeRootSim(
      HexState<Size> root_state,
      std::array<std::unordered_map<uint64_t, TreeNode>, 3>* visited) {
    SimulationState root_sim;
    root_sim.available_moves = root_state.AvailableMoves();
    root_sim.expand = true;
    root_sim.breadcrumb_size = 1;
    root_sim.breadcrumb.fill(nullptr);
    root_sim.breadcrumb.front() =
        &((*visited)[as_underlying(root_state.ToMove())][root_state.Hash()]);
    root_sim.state = std::move(root_state);
    return root_sim;
  }

  /*
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
  */

  void FinishRandom(SimulationState* sim) {
    DCHECK(!sim->state.GameIsOver());
    DCHECK(!sim->expand);
    CHECK_GT(sim->state.EmptySpaces(), 0);
    std::vector<int> shuffled_moves;
    shuffled_moves.reserve(sim->state.EmptySpaces());
    for (int i = 0; i < sim->available_moves.size(); ++i) {
      if (sim->available_moves[i]) {
        shuffled_moves.push_back(i);
      }
    }
    std::shuffle(shuffled_moves.begin(), shuffled_moves.end(), gen_);
    for (const auto& move : shuffled_moves) {
      sim->state.SetPiece(move);
      if (sim->state.GameIsOver()) {
        break;
      }
    }
  }

  void StepSimulation(SimulationState* sim) { FinishRandom(sim); }

  void RunSimulation() {
    SimulationState sim = root_sim_;
    sim.expand = false;
    CHECK_GT(sim.state.EmptySpaces(), 0);
    while (!sim.state.GameIsOver()) {
      StepSimulation(&sim);
    }

    // If the root won, winner_mod_2 = false
    bool root_wins = root_sim_.state.ToMove() == sim.state.Winner();

    // Backprop.
    auto& breadcrumb = sim.breadcrumb;
    for (int i = 0; i < sim.breadcrumb_size; ++i) {
      ++breadcrumb[i]->num_visits;
      if (i % 2 != root_wins) {
        ++breadcrumb[i]->num_wins;
      }
    }
    LOG_EVERY_N(10000, INFO)
        << "Win probability: "
        << static_cast<double>(root_node_->num_wins) / root_node_->num_visits;
  }

  static std::mt19937 gen_;

  // The base state for this game tree.
  std::array<std::unordered_map<uint64_t, TreeNode>, 3> visited_;
  SimulationState root_sim_;
  const TreeNode* root_node_;
};

template <int Size>
std::mt19937 HexTree<Size>::gen_ = std::mt19937(std::random_device()());
