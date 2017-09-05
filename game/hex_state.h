#pragma once
#include <bitset>
#include <sstream>
#include <unordered_set>
#include "base/logging.h"
#include "game/cell_state.h"

template <int Size>
class HexState {
 public:
  static constexpr int kSize = Size;
  static constexpr int kNumCells = Size * Size;

  HexState() : winner_(PieceType::kEmpty) {}

  void SetCell(int row, int col, CellState val) {
    data_[row * Size + col] = val;
  }

  void PropagateConnections(const CellState& base_piece, int base_index,
                            PieceType base_type) {
    std::unordered_set<int> to_color;
    {
      const auto& neighbor_data = neighbors_[base_index];
      const auto& num_neighbors = neighbor_data.first;
      const auto& neighbor_array = neighbor_data.second;
      for (int i = 0; i < num_neighbors; ++i) {
        const auto& neighbor_index = neighbor_array[i];
        auto& neighbor = data_[neighbor_index];
        if (neighbor.data() & as_underlying(base_type)) {
          if (neighbor != base_piece) {
            to_color.insert(neighbor_index);
          }
        }
      }
    }

    while (!to_color.empty()) {
      auto it = to_color.begin();
      const int current = *it;
      to_color.erase(it);
      data_[current] = base_piece;
      const auto& neighbor_data = neighbors_[current];
      const auto& num_neighbors = neighbor_data.first;
      const auto& neighbor_array = neighbor_data.second;
      for (int i = 0; i < num_neighbors; ++i) {
        const auto& neighbor_index = neighbor_array[i];
        auto& neighbor = data_[neighbor_index];
        if (neighbor.data() & as_underlying(base_type)) {
          if (neighbor != base_piece) {
            to_color.insert(neighbor_index);
          }
        }
      }
    }
  }

  bool PropagateNewPiece(int index, PieceType type) {
    const auto& neighbor_data = neighbors_[index];
    const auto& num_neighbors = neighbor_data.first;
    const auto& neighbor_array = neighbor_data.second;
    auto& new_piece = data_[index];

    // Copy in connection data from neighbors.
    for (int i = 0; i < num_neighbors; ++i) {
      const auto& neighbor_index = neighbor_array[i];
      const auto& neighbor = data_[neighbor_index];
      if (neighbor.data() & as_underlying(type)) {
        new_piece.data() |= neighbor.data();
      }
    }
    if (new_piece.IsConnectedToLower() && new_piece.IsConnectedToUpper()) {
      // Early out if this was the final connection.
      winner_ = type;
      return true;
    }

    // Propagate connection data to neighbors.
    PropagateConnections(new_piece, index, type);
    return false;
  }

  bool SetHorizontalPiece(int row, int col) {
    const int index = Index(row, col);
    auto& new_piece = data_[index];
    DCHECK_EQ(new_piece.GetPieceType(), PieceType::kEmpty);
    new_piece = CellState(PieceType::kHorizontal);
    if (col <= 0) {
      new_piece.SetConnectedToLower();
    }
    if (col >= Size - 1) {
      new_piece.SetConnectedToUpper();
    }
    return PropagateNewPiece(index, PieceType::kHorizontal);
  }

  bool SetVerticalPiece(int row, int col) {
    const int index = Index(row, col);
    auto& new_piece = data_[index];
    DCHECK_EQ(new_piece.GetPieceType(), PieceType::kEmpty);
    new_piece = CellState(PieceType::kVertical);
    if (row <= 0) {
      new_piece.SetConnectedToLower();
    }
    if (row >= Size - 1) {
      new_piece.SetConnectedToUpper();
    }
    return PropagateNewPiece(index, PieceType::kVertical);
  }

  bool GameIsOver() { return winner_ != PieceType::kEmpty; }
  PieceType Winner() { return winner_; }

  CellState GetCell(int row, int col) const { return data_[row * Size + col]; }

  CellState GetCell(int index) const { return data_[index]; }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const HexState<Size>& state) {
    int count = 0;
    for (const auto& cell : state.data()) {
      if (count % Size == 0) {
        os << '\n';
      }
      os << cell;
      ++count;
    }
    return os;
  }

  const std::array<CellState, kNumCells>& data() const { return data_; }

  static int Index(int row, int col) {
    DCHECK_GE(row, 0);
    DCHECK_GE(col, 0);
    DCHECK_LE(row, Size);
    DCHECK_LE(col, Size);
    return row * Size + col;
  }

  static int SafeIndex(int row, int col) {
    if (row < 0 || row >= Size || col < 0 || col >= Size) {
      return -1;
    }
    return Index(row, col);
  }

  // Precompute indices for the neighbors of each cell.
  static std::array<std::pair<int, std::array<int, 6>>, kNumCells>
  ComputeNeighbors() {
    std::array<std::pair<int, std::array<int, 6>>, kNumCells> out;
    for (int row = 0; row < Size; ++row) {
      for (int col = 0; col < Size; ++col) {
        auto& to_set = out[Index(row, col)];
        std::vector<int> maybe_neighbors;
        maybe_neighbors.push_back(SafeIndex(row - 1, col));
        maybe_neighbors.push_back(SafeIndex(row - 1, col + 1));
        maybe_neighbors.push_back(SafeIndex(row, col - 1));
        maybe_neighbors.push_back(SafeIndex(row, col + 1));
        maybe_neighbors.push_back(SafeIndex(row + 1, col - 1));
        maybe_neighbors.push_back(SafeIndex(row + 1, col + 1));
        int count = 0;
        for (int maybe_neighbor : maybe_neighbors) {
          if (maybe_neighbor >= 0) {
            to_set.second[count++] = maybe_neighbor;
          }
        }
        to_set.first = count;
      }
    }
    return out;
  }

 private:
  // Fake a max size static array.
  static std::array<std::pair<int, std::array<int, 6>>, kNumCells> neighbors_;

  // Cells are stored row-major order.
  std::array<CellState, kNumCells> data_{};

  PieceType winner_;
};

template <int Size>
std::array<std::pair<int, std::array<int, 6>>, HexState<Size>::kNumCells>
    HexState<Size>::neighbors_ = HexState<Size>::ComputeNeighbors();
