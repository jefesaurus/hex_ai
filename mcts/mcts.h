#pragma once
#include <random>
#include "base/wall_timer.h"
#include "game/hex_state.h"

// Hex tree is the tree for one playout. Currently no way to iterate a tree down
// itself for reusing computation on the next level.
//
// This creates a tree out of the supplied state of the board where root node
// will aggregate contains the  win/loss stats for that state.
template <int Size>
class HexTree {
 private:
  // The wins are relative to the player NOT to move, that is, the wins of the
  // children are relative to this node, this node's wins are relative to its
  // parent.
  struct TreeNode {
    // Edges down to the next ply
    std::array<TreeNode*, HexState<Size>::kNumMoves> children = {};
    int num_wins = 0;
    int num_visits = 0;
  };

  // The state required for one simulated play.
  struct SimulationState {
    HexState<Size> state;
    std::array<bool, HexState<Size>::kNumMoves> available_moves;

    // If expand is true, that means we're still looking down the part of the
    // tree we've seen before. When this is flipped false, we finish the playout
    // randomly and don't fill out the breadcrumb any further.
    bool expand;

    // Depth is equal to the number of plies traversed by this simulation.
    // Because the sim always starts at the root, this is always 1 or greater.
    int depth;

    // The list of visited nodes out of the root node. The first one will always
    // be the pointer to the root node for the entire HexTree.
    std::array<TreeNode*, HexState<Size>::kNumMoves + 1> breadcrumb;
  };

 public:
  HexTree(HexState<Size> current_state)
      : root_sim_(InitializeRootSim(current_state, &visited_)),
        root_node_(root_sim_.breadcrumb.front()) {}

  static SimulationState InitializeRootSim(
      HexState<Size> root_state,
      std::unordered_map<uint64_t, TreeNode>* visited) {
    SimulationState root_sim;
    root_sim.available_moves = root_state.AvailableMoves();
    root_sim.expand = true;
    root_sim.depth = 1;
    root_sim.breadcrumb.fill(nullptr);
    auto new_map_it =
        visited->emplace(std::make_pair(root_state.Hash(), TreeNode()));
    root_sim.breadcrumb.front() = &(new_map_it.first->second);
    root_sim.state = std::move(root_state);
    return root_sim;
  }

  std::pair<int, double> GetBestMove() {
    double best_prob = -1;
    double best_move = -1;
    for (int i = 0; i < root_sim_.available_moves.size(); ++i) {
      if (root_sim_.available_moves[i] && root_node_->children[i]) {
        double score = static_cast<double>(root_node_->children[i]->num_wins) /
                       root_node_->children[i]->num_visits;
        if (score > best_prob) {
          best_move = i;
          best_prob = score;
        }
      }
    }
    return {best_move, best_prob};
  }

  void StepTreeExpansion(SimulationState* sim) {
    DCHECK(!sim->state.GameIsOver());
    DCHECK(sim->expand);
    DCHECK_GT(sim->depth, 0);
    DCHECK_LE(sim->depth, HexState<Size>::kNumMoves + 1);

    auto& current_node = sim->breadcrumb[sim->depth - 1];
    auto& children = current_node->children;

    // The visited map for the next level down.
    int total_visits = 0;
    for (int i = 0; i < sim->available_moves.size(); ++i) {
      if (sim->available_moves[i]) {
        if (!children[i]) {
          // This is a branch we haven't tried before, check if we've seen the
          // resulting state before.
          uint64_t next_hash = sim->state.HashIf(i);
          auto map_it = visited_.find(next_hash);
          if (map_it == visited_.end()) {
            // Unexpanded moves have infinite score, so choose this.
            sim->state.MakeMove(i);
            // Create a new tree node
            auto new_map_it =
                visited_.emplace(std::make_pair(next_hash, TreeNode()));
            auto* new_node = &(new_map_it.first->second);
            new_node->num_visits = 0;
            new_node->num_wins = 0;
            new_node->children.fill(nullptr);
            // This is the end of the expansion.
            children[i] = new_node;
            sim->breadcrumb[sim->depth++] = new_node;
            if (sim->state.MoveNumber() <= 2) {
              sim->available_moves = sim->state.AvailableMoves();
              // If we haven't gotten past the "swap" rule possibility yet, keep
              // expanding a bit.
            } else {
              // After the second move, available moves will monotonically
              // decrease bits, so we don't need to query anymore.
              sim->expand = false;
              sim->available_moves[i] = false;
            }
            return;
          } else {
            // We actually have visited this node before, update the branch
            // structure;
            children[i] = &(map_it->second);
          }
        }
        // We've already taken this branch before.
        total_visits += children[i]->num_visits;
      }
    }

    int best_move = -1;
    double best_score = -1;

    // Now we have log total.
    total_visits = std::log(total_visits);
    for (int i = 0; i < sim->available_moves.size(); ++i) {
      // We've already taken this branch before.
      if (sim->available_moves[i]) {
        const auto& node = *children[i];
        double score = static_cast<double>(node.num_wins) / node.num_visits +
                       1.4 * std::sqrt(total_visits / node.num_visits);
        if (score > best_score) {
          best_move = i;
          best_score = score;
        }
      }
    }
    DCHECK_GE(best_move, 0);

    // And apply the selected move.
    sim->state.MakeMove(best_move);
    sim->breadcrumb[sim->depth++] = children[best_move];
    if (sim->state.MoveNumber() <= 2) {
      sim->available_moves = sim->state.AvailableMoves();
    } else {
      // After the second move, available moves will monotonically
      // decrease bits, so we don't need to query anymore.
      sim->available_moves[best_move] = false;
    }
  }

  void FinishRandom(SimulationState* sim) {
    DCHECK(!sim->state.GameIsOver());
    DCHECK(!sim->expand);
    std::array<int, HexState<Size>::kNumMoves> moves;
    int n_moves = 0;
    for (int i = 0; i < sim->available_moves.size(); ++i) {
      if (sim->available_moves[i]) {
        moves[n_moves++] = i;
      }
    }

    // Fisher Yates shuffle
    for (int i = n_moves - 1; i > 0; --i) {
      // With modulo bias, call the cops
      int j = gen_() % i;
      sim->state.MakeMove(moves[j]);
      if (sim->state.GameIsOver()) {
        break;
      }
      moves[j] = moves[i];
    }
    if (!sim->state.GameIsOver()) {
      sim->state.MakeMove(moves[0]);
    }
  }

  void StepSimulation(SimulationState* sim) {
    if (sim->expand) {
      StepTreeExpansion(sim);
    } else {
      FinishRandom(sim);
    }
  }

  void RunSimulation() {
    SimulationState sim = root_sim_;
    while (!sim.state.GameIsOver()) {
      StepSimulation(&sim);
    }

    bool root_wins = root_sim_.state.ToMove() == sim.state.Winner();

    // Backprop.
    auto& breadcrumb = sim.breadcrumb;
    for (int i = 0; i < sim.depth; ++i) {
      ++breadcrumb[i]->num_visits;
      // At the 0th level, if root_wins = 1, don't increment.
      if (i % 2 == root_wins) {
        ++breadcrumb[i]->num_wins;
      }
    }
  }

  static std::mt19937 gen_;

  // visited node cache
  std::unordered_map<uint64_t, TreeNode> visited_;

  // The base state for this game tree.
  SimulationState root_sim_;
  const TreeNode* root_node_;
};

template <int Size>
std::mt19937 HexTree<Size>::gen_ = std::mt19937(0);
