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

  void SetCell(int row, int col, CellState val) {
    data_[row * Size + col] = val;
  }

  void SetPiece(int row, int col, PieceType type) {
    data_[row * Size + col] = CellState(type);
  }

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

  static int Index(int row, int col) { return row * Size + col; }

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
  }

 private:
  // Cells are stored row-major order.
  std::array<CellState, kNumCells> data_{};

  // Fake a max size static array.
  static std::array<std::pair<int, std::array<int, 6>>, kNumCells> neighbors_;
};

template <int Size>
std::array<std::pair<int, std::array<int, 6>>, HexState<Size>::kNumCells>
    HexState<Size>::neighbors_ = HexState<Size>::ComputeNeighbors();
