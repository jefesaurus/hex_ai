#pragma once
#include <sstream>
#include <unordered_set>
#include "base/logging.h"
#include "game/piece_type.h"
#include "game/static_simd_bitset.h"

// Lower(row = 0, col = 0)
//
//     ║ a ║ b ║ c ║ d ║ e ║
//    ═╩═╦═╩═╦═╩═╦═╩═╦═╩═╦═╩═╗
//     1 ║ - ║ - ║ - ║ - ║ - ║
//    ═══╩═╦═╩═╦═╩═╦═╩═╦═╩═╦═╩═╗
//       2 ║ - ║ - ║ - ║ - ║ - ║
//      ═══╩═╦═╩═╦═╩═╦═╩═╦═╩═╦═╩═╗
//         3 ║ - ║ - ║ - ║ - ║ - ║
//        ═══╩═╦═╩═╦═╩═╦═╩═╦═╩═╦═╩═╗
//           4 ║ - ║ - ║ - ║ - ║ - ║
//          ═══╩═╦═╩═╦═╩═╦═╩═╦═╩═╦═╩═╗
//             5 ║ - ║ - ║ - ║ - ║ - ║
//            ═══╩═══╩═══╩═══╩═══╩═══╝
//                                Upper
//(row = Size, col = Size)

template <int Size>
class HexState {
 public:
  static constexpr int kSize = Size;
  static constexpr int kNumCells = Size * Size;
  static constexpr int kMaxNumGroups =
      ConstexprCeil(kSize / 2.0) * ConstexprCeil(kSize / 2.0);

  HexState()
      : num_horizontal_groups_(2),
        horizontal_groups_(InitialHorizontalGroups()),
        num_vertical_groups_(2),
        vertical_groups_(InitialVerticalGroups()),
        winner_(PieceType::kEmpty),
        empty_spaces_(kNumCells) {}

  void SetPiece(int row, int col, PieceType type) {
    SetPiece(Index(row, col), type);
  }

  void SetPiece(int index, PieceType type) {
    if (type == PieceType::kHorizontal) {
      SetHorizontalPiece(index);
    } else {
      DCHECK_EQ(type, PieceType::kVertical);
      SetVerticalPiece(index);
    }
    --empty_spaces_;
  }

  void SetHorizontalPiece(int index) {
    const auto& neighbor_mask = neighbor_masks_[index];

    // Assign to lowest neighboring group
    int merge_into = 0;
    for (; merge_into < num_horizontal_groups_; ++merge_into) {
      auto& group = horizontal_groups_[merge_into];
      if (neighbor_mask.Intersects(group)) {
        // Merge group
        group.Set(index);
        // Now it has been merged into a group and we know it has neighbors, so
        // we need to potentially merge other groups.
        for (int j = num_horizontal_groups_ - 1; j > merge_into; --j) {
          if (neighbor_mask.Intersects(horizontal_groups_[j])) {
            // Merge into group j
            MergeHorizontalGroups(merge_into, j);
          }
        }
        return;
      }
    }

    horizontal_groups_[num_horizontal_groups_].Clear();
    horizontal_groups_[num_horizontal_groups_++].Set(index);
  }

  void SetVerticalPiece(int index) {
    const auto& neighbor_mask = neighbor_masks_[index];

    // Assign to lowest neighboring group
    int merge_into = 0;
    for (; merge_into < num_vertical_groups_; ++merge_into) {
      auto& group = vertical_groups_[merge_into];
      if (neighbor_mask.Intersects(group)) {
        // Merge group
        group.Set(index);

        // Now it has been merged into a group and we know it has neighbors, so
        // we need to potentially merge other groups.
        for (int j = num_vertical_groups_ - 1; j > merge_into; --j) {
          if (neighbor_mask.Intersects(vertical_groups_[j])) {
            // Merge into group j
            MergeVerticalGroups(merge_into, j);
          }
        }
        return;
      }
    }

    // There was no neighboring group, create a group for it.
    vertical_groups_[num_vertical_groups_].Clear();
    vertical_groups_[num_vertical_groups_++].Set(index);
  }

  void MergeVerticalGroups(int a, int b) {
    DCHECK_LT(a, b);
    if (a == 0 && b == 1) {
      winner_ = PieceType::kVertical;
    } else {
      vertical_groups_[a] |= vertical_groups_[b];
      // Erase b
      DCHECK_GT(num_vertical_groups_, 2);
      vertical_groups_[b] = vertical_groups_[--num_vertical_groups_];
    }
  }

  void MergeHorizontalGroups(int a, int b) {
    DCHECK_LT(a, b);
    if (a == 0 && b == 1) {
      winner_ = PieceType::kHorizontal;
    } else {
      horizontal_groups_[a] |= horizontal_groups_[b];
      // Erase b
      DCHECK_GT(num_horizontal_groups_, 2);
      horizontal_groups_[b] = horizontal_groups_[--num_horizontal_groups_];
    }
  }

  bool GameIsOver() { return winner_ != PieceType::kEmpty; }

  PieceType Winner() { return winner_; }

  PieceType GetCell(int row, int col) const { return GetCell(Index(row, col)); }

  PieceType GetCell(int index) const {
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
  }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const HexState<Size>& state) {
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

  static std::array<SimdBitset128<kNumCells + 4>, kNumCells>
  ComputeNeighborMasks(
      const std::array<std::array<int, 6>, kNumCells>& neighbors) {
    std::array<SimdBitset128<kNumCells + 4>, kNumCells> out;
    for (int i = 0; i < kNumCells; ++i) {
      for (const auto& neighbor : neighbors[i]) {
        out[i].Set(neighbor);
      }
    }
    return out;
  }

  static std::array<SimdBitset128<kNumCells + 4>, kMaxNumGroups>
  InitialHorizontalGroups() {
    std::array<SimdBitset128<kNumCells + 4>, kMaxNumGroups> groups;
    groups[0].Set(kNumCells);
    groups[1].Set(kNumCells + 1);
    return groups;
  }

  static std::array<SimdBitset128<kNumCells + 4>, kMaxNumGroups>
  InitialVerticalGroups() {
    std::array<SimdBitset128<kNumCells + 4>, kMaxNumGroups> groups;
    groups[0].Set(kNumCells + 2);
    groups[1].Set(kNumCells + 3);
    return groups;
  }

 private:
  static std::array<std::array<int, 6>, kNumCells> neighbors_;
  static std::array<SimdBitset128<kNumCells + 4>, kNumCells> neighbor_masks_;

  int num_horizontal_groups_;
  std::array<SimdBitset128<kNumCells + 4>, kMaxNumGroups> horizontal_groups_;
  int num_vertical_groups_;
  std::array<SimdBitset128<kNumCells + 4>, kMaxNumGroups> vertical_groups_;

  PieceType winner_;

  int empty_spaces_;
};

template <int Size>
std::array<std::array<int, 6>, HexState<Size>::kNumCells>
    HexState<Size>::neighbors_ = HexState<Size>::ComputeNeighbors();

template <int Size>
std::array<SimdBitset128<HexState<Size>::kNumCells + 4>,
           HexState<Size>::kNumCells>
    HexState<Size>::neighbor_masks_ =
        HexState<Size>::ComputeNeighborMasks(HexState<Size>::neighbors_);
