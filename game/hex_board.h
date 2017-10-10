#pragma once
#include "base/constexpr_ceil.h"
#include "base/logging.h"
#include "game/piece_type.h"

template <int Size>
class HexBoard {
 public:
  static constexpr int kSize = Size;
  static constexpr int kNumCells = Size * Size;
  static constexpr int kMaxNumGroups =
      ConstexprCeil(kSize / 2.0) * ConstexprCeil(kSize / 2.0);

  HexBoard()
      : num_horizontal_groups_(2),
        horizontal_groups_(InitialHorizontalGroups()),
        num_vertical_groups_(2),
        vertical_groups_(InitialVerticalGroups()),
        winner_(PieceType::kEmpty) {}
  // Default copy, move, assign
  HexBoard(const HexBoard&) = default;
  HexBoard& operator=(const HexBoard&) = default;
  HexBoard(HexBoard&&) = default;
  HexBoard& operator=(HexBoard&&) = default;

  // Every spot on the board
  template <typename T>
  void AvailableSpaces(T* out) const {
    std::bitset<kNumCells + 4> not_available = horizontal_groups_[0];
    for (int i = 1; i < horizontal_groups_.size(); ++i) {
      not_available |= horizontal_groups_[i];
    }
    for (const auto& group : vertical_groups_) {
      not_available |= group;
    }
    for (int i = 0; i < kNumCells; ++i) {
      (*out)[i] = !not_available[i];
    }
  }

  // Which sides are connected, or kEmpty if neither.
  PieceType ConnectionStatus() {
    return winner_;
  }

  void SetHorizontalPiece(int row, int col) {
    SetHorizontalPiece(Index(row, col));
  }

  void SetHorizontalPiece(int index) {
    DCHECK_LT(index, kNumCells);
    const auto& neighbor_mask = neighbor_masks_[index];

    // Assign to lowest neighboring group
    int merge_into = 0;
    for (; merge_into < num_horizontal_groups_; ++merge_into) {
      auto& group = horizontal_groups_[merge_into];
      if ((neighbor_mask & group).any()) {
        // Merge group
        group[index] = true;
        // Now it has been merged into a group and we know it has neighbors, so
        // we need to potentially merge other groups.
        for (int j = num_horizontal_groups_ - 1; j > merge_into; --j) {
          if ((neighbor_mask & horizontal_groups_[j]).any()) {
            // Merge into group j
            MergeHorizontalGroups(merge_into, j);
          }
        }
        return;
      }
    }

    // There was no neighboring group, create a group for it.
    horizontal_groups_[num_horizontal_groups_].reset();
    horizontal_groups_[num_horizontal_groups_++][index] = true;
  }

  void SetVerticalPiece(int row, int col) {
    SetVerticalPiece(Index(row, col));
  }

  void SetVerticalPiece(int index) {
    const auto& neighbor_mask = neighbor_masks_[index];

    // Assign to lowest neighboring group
    int merge_into = 0;
    for (; merge_into < num_vertical_groups_; ++merge_into) {
      auto& group = vertical_groups_[merge_into];
      if ((neighbor_mask & group).any()) {
        // Merge group
        group[index] = true;

        // Now it has been merged into a group and we know it has neighbors, so
        // we need to potentially merge other groups.
        for (int j = num_vertical_groups_ - 1; j > merge_into; --j) {
          if ((neighbor_mask & vertical_groups_[j]).any()) {
            // Merge into group j
            MergeVerticalGroups(merge_into, j);
          }
        }
        return;
      }
    }

    // There was no neighboring group, create a group for it.
    vertical_groups_[num_vertical_groups_].reset();
    vertical_groups_[num_vertical_groups_++][index] = true;
  }

  bool MergeVerticalGroups(int a, int b) {
    DCHECK_LT(a, b);
    if (a == 0 && b == 1) {
      winner_ = PieceType::kVertical;
      return true;
    } else {
      vertical_groups_[a] |= vertical_groups_[b];
      // Erase b
      DCHECK_GT(num_vertical_groups_, 2);
      vertical_groups_[b] = vertical_groups_[--num_vertical_groups_];
      return false;
    }
  }

  bool MergeHorizontalGroups(int a, int b) {
    DCHECK_LT(a, b);
    if (a == 0 && b == 1) {
      winner_ = PieceType::kHorizontal;
      return true;
    } else {
      horizontal_groups_[a] |= horizontal_groups_[b];
      // Erase b
      DCHECK_GT(num_horizontal_groups_, 2);
      horizontal_groups_[b] = horizontal_groups_[--num_horizontal_groups_];
      return false;
    }
  }

  PieceType GetCell(int row, int col) const {
    return GetCell(Index(row, col));
  }

  PieceType GetCell(int index) const {
    DCHECK_LT(index, kNumCells);
    DCHECK_GE(index, 0);
    for (const auto& group : horizontal_groups_) {
      if (group[index]) {
        return PieceType::kHorizontal;
      }
    }

    for (const auto& group : vertical_groups_) {
      if (group[index]) {
        return PieceType::kVertical;
      }
    }

    return PieceType::kEmpty;
  }

 public:
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const HexBoard<Size>& state) {
    static const std::string letters = "abcdefghijklmnopqrstuvwxyz";
    os << '\n' << " ";
    for (int i = 0; i < Size; ++i) {
      os << "║ " << letters.at(i) << " ";
    }
    os << "║";
    os << '\n';

    os << "═";
    for (int j = 0; j < Size; ++j) {
      os << "╩═╦═";
    }

    os << "╩═╗" << '\n';
    std::string white_space = "";
    for (int i = 0; i < Size - 1; ++i) {
      std::string row_label = std::to_string(i + 1);
      if (row_label.size() == 1) {
        os << " " << row_label << " ║";
      } else {
        os << row_label << " ║";
      }
      for (int j = 0; j < Size; ++j) {
        os << " " << state.GetCell(i, j) << " ║";
      }
      os << '\n' << white_space << "═══";
      for (int j = 0; j < Size; ++j) {
        os << "╩═╦═";
      }
      os << "╩═╗";
      white_space.append("  ");
      os << '\n' << white_space;
    }
    {
      int i = Size - 1;
      std::string row_label = std::to_string(i + 1);
      if (row_label.size() == 1) {
        os << " " << row_label << " ║";
      } else {
        os << row_label << " ║";
      }
      for (int j = 0; j < Size; ++j) {
        os << " " << state.GetCell(i, j) << " ║";
      }
      os << '\n' << white_space << "═══";
      for (int j = 0; j < Size; ++j) {
        os << "╩═══";
      }
      os << "╝";
      white_space.append("  ");
      os << '\n' << white_space;
    }
    return os;
  }

 private:
  static int Index(int row, int col) {
    DCHECK_GE(row, 0);
    DCHECK_GE(col, 0);
    DCHECK_LT(row, Size);
    DCHECK_LT(col, Size);
    return row * Size + col;
  }

  static int SafeIndex(int row, int col) {
    if (col < 0) {
      // Horizontal lower
      return kNumCells;
    } else if (col >= Size) {
      // Horizontal upper
      return kNumCells + 1;
    } else if (row < 0) {
      // Vertical lower
      return kNumCells + 2;
    } else if (row >= Size) {
      // Vertical upper
      return kNumCells + 3;
    } else {
      return Index(row, col);
    }
  }

  // Precompute indices for the neighbors of each cell.
  static std::array<std::array<int, 6>, kNumCells> ComputeNeighbors() {
    std::array<std::array<int, 6>, kNumCells> out;
    for (int row = 0; row < Size; ++row) {
      for (int col = 0; col < Size; ++col) {
        auto& to_set = out[Index(row, col)];
        to_set[0] = SafeIndex(row - 1, col);
        to_set[1] = SafeIndex(row - 1, col + 1);
        to_set[2] = SafeIndex(row, col - 1);
        to_set[3] = SafeIndex(row, col + 1);
        to_set[4] = SafeIndex(row + 1, col - 1);
        to_set[5] = SafeIndex(row + 1, col);
      }
    }
    return out;
  }

  static std::array<std::bitset<kNumCells + 4>, kNumCells> ComputeNeighborMasks(
      const std::array<std::array<int, 6>, kNumCells>& neighbors) {
    std::array<std::bitset<kNumCells + 4>, kNumCells> out;
    for (int i = 0; i < kNumCells; ++i) {
      for (const auto& neighbor : neighbors[i]) {
        out[i][neighbor] = true;
      }
    }
    return out;
  }

  static std::array<std::bitset<kNumCells + 4>, kMaxNumGroups>
  InitialHorizontalGroups() {
    std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> groups;
    // Column -1 sentinel
    groups[0][kNumCells] = true;
    // Column Max sentinel
    groups[1][kNumCells + 1] = true;
    return groups;
  }

  static std::array<std::bitset<kNumCells + 4>, kMaxNumGroups>
  InitialVerticalGroups() {
    std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> groups;
    // Row -1 sentinel
    groups[0][kNumCells + 2] = true;
    // Row Max sentinel
    groups[1][kNumCells + 3] = true;
    return groups;
  }

 private:
  static std::array<std::array<int, 6>, kNumCells> neighbors_;
  static std::array<std::bitset<kNumCells + 4>, kNumCells> neighbor_masks_;

  int num_horizontal_groups_;
  std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> horizontal_groups_;
  int num_vertical_groups_;
  std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> vertical_groups_;

  PieceType winner_;
};

template <int Size>
std::array<std::array<int, 6>, HexBoard<Size>::kNumCells>
    HexBoard<Size>::neighbors_ = HexBoard<Size>::ComputeNeighbors();

template <int Size>
std::array<std::bitset<HexBoard<Size>::kNumCells + 4>,
           HexBoard<Size>::kNumCells>
    HexBoard<Size>::neighbor_masks_ =
        HexBoard<Size>::ComputeNeighborMasks(HexBoard<Size>::neighbors_);
