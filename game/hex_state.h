#pragma once
#include <bitset>
#include <sstream>
#include <unordered_set>
#include "base/logging.h"
#include "game/piece_type.h"
#include "game/zobrist.h"

constexpr int32_t ConstexprCeil(float num) {
  return (static_cast<float>(static_cast<int32_t>(num)) == num)
             ? static_cast<int32_t>(num)
             : static_cast<int32_t>(num) + ((num > 0) ? 1 : 0);
}

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
      : to_move_(PieceType::kHorizontal),
        num_horizontal_groups_(2),
        horizontal_groups_(InitialHorizontalGroups()),
        num_vertical_groups_(2),
        vertical_groups_(InitialVerticalGroups()),
        winner_(PieceType::kEmpty),
        empty_spaces_(kNumCells),
        hash_(0) {}
  // Default copy, move, assign
  HexState(const HexState&) = default;
  HexState& operator=(const HexState&) = default;
  HexState(HexState&&) = default;
  HexState& operator=(HexState&&) = default;

  uint64_t Hash() { return hash_; }
  int EmptySpaces() { return empty_spaces_; }
  static const ZobristHasher<kNumCells, uint64_t>& Hasher() { return hasher_; };
  PieceType ToMove() { return to_move_; }

  void SetPiece(int row, int col) { SetPiece(Index(row, col)); }

  void SetPiece(int index) {
    DCHECK_EQ(GetCell(index), PieceType::kEmpty);
    DCHECK(!GameIsOver());
    DCHECK(to_move_ == PieceType::kHorizontal ||
           to_move_ == PieceType::kVertical);
    if (to_move_ == PieceType::kHorizontal) {
      SetHorizontalPiece(index);
    } else {
      DCHECK_EQ(to_move_, PieceType::kVertical);
      SetVerticalPiece(index);
    }
    --empty_spaces_;
    hasher_.FlipPiece(index, to_move_, &hash_);
    to_move_ = FlipPlayer(to_move_);
  }

  bool GameIsOver() { return winner_ != PieceType::kEmpty; }

  PieceType Winner() { return winner_; }

 private:
  void SetHorizontalPiece(int index) {
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

    return PieceType::kEmpty;
  }

 public:
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
    groups[0][kNumCells] = true;
    groups[1][kNumCells + 1] = true;
    return groups;
  }

  static std::array<std::bitset<kNumCells + 4>, kMaxNumGroups>
  InitialVerticalGroups() {
    std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> groups;
    groups[0][kNumCells + 2] = true;
    groups[1][kNumCells + 3] = true;
    return groups;
  }

 private:
  static std::array<std::array<int, 6>, kNumCells> neighbors_;
  static std::array<std::bitset<kNumCells + 4>, kNumCells> neighbor_masks_;
  static ZobristHasher<kNumCells, uint64_t> hasher_;

  PieceType to_move_;

  int num_horizontal_groups_;
  std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> horizontal_groups_;
  int num_vertical_groups_;
  std::array<std::bitset<kNumCells + 4>, kMaxNumGroups> vertical_groups_;

  PieceType winner_;

  int empty_spaces_;
  uint64_t hash_;
};

template <int Size>
std::array<std::array<int, 6>, HexState<Size>::kNumCells>
    HexState<Size>::neighbors_ = HexState<Size>::ComputeNeighbors();

template <int Size>
std::array<std::bitset<HexState<Size>::kNumCells + 4>,
           HexState<Size>::kNumCells>
    HexState<Size>::neighbor_masks_ =
        HexState<Size>::ComputeNeighborMasks(HexState<Size>::neighbors_);
