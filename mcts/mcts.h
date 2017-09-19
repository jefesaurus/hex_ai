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
    int depth;
    std::array<TreeNode*, Size * Size> breadcrumb;
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
    DCHECK_LE(sim->depth, Size * Size);

    auto& current_node = sim->breadcrumb[sim->depth - 1];
    static const auto& hasher = HexState<Size>::Hasher();
    auto& children = current_node->children;

    const auto& to_move = sim->state.ToMove();

    // The visited map for the next level down.
    int total_visits = 0;
    for (int i = 0; i < sim->available_moves.size(); ++i) {
      if (sim->available_moves[i]) {
        if (!children[i]) {
          // This is a branch we haven't tried before, check if we've seen the
          // resulting state before.
          uint64_t next_hash = sim->state.Hash();
          hasher.FlipPiece(i, to_move, &next_hash);
          auto map_it = visited_.find(next_hash);
          if (map_it == visited_.end()) {
            // Unexpanded moves have infinite score, so choose this.
            sim->state.SetPiece(i);
            // Create a new tree node
            auto new_map_it =
                visited_.emplace(std::make_pair(next_hash, TreeNode()));
            auto* new_node = &(new_map_it.first->second);
            new_node->num_visits = 0;
            new_node->num_wins = 0;
            new_node->children.fill(nullptr);
            // This is the end of the expansion.
            sim->expand = false;
            children[i] = new_node;
            sim->breadcrumb[sim->depth++] = new_node;
            sim->available_moves[i] = false;
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

    // Now we have log total.
    int best_move = -1;
    double best_score = -1;
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
    sim->state.SetPiece(best_move);
    sim->breadcrumb[sim->depth++] = children[best_move];
    sim->available_moves[best_move] = false;
  }

  void FinishRandom(SimulationState* sim) {
    DCHECK(!sim->state.GameIsOver());
    DCHECK(!sim->expand);
    DCHECK_GT(sim->state.EmptySpaces(), 0);
    std::array<int, Size * Size> moves;
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
      sim->state.SetPiece(moves[j]);
      if (sim->state.GameIsOver()) {
        break;
      }
      moves[j] = moves[i];
    }
    if (!sim->state.GameIsOver()) {
      sim->state.SetPiece(moves[0]);
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
    DCHECK_GT(sim.state.EmptySpaces(), 0);
    while (!sim.state.GameIsOver()) {
      StepSimulation(&sim);
    }

    // If the root won, winner_mod_2 = false
    bool root_wins = root_sim_.state.ToMove() != sim.state.Winner();

    // Backprop.
    auto& breadcrumb = sim.breadcrumb;
    for (int i = 0; i < sim.depth; ++i) {
      ++breadcrumb[i]->num_visits;
      if (i % 2 != root_wins) {
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
