#pragma once
#include <sstream>
#include <unordered_set>
#include "base/logging.h"
#include "game/cell_state.h"

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

  HexState() : winner_(PieceType::kEmpty), empty_spaces_(kNumCells) {}

  bool SetPiece(int index, PieceType type) {
    auto& new_piece = data_[index];
    DCHECK_EQ(new_piece.GetPieceType(), PieceType::kEmpty);
    new_piece = cell_templates_[index][as_underlying(type)];
    --empty_spaces_;
    return PropagateNewPiece(index, type);
  }

  bool SetPiece(int row, int col, PieceType type) {
    return SetPiece(Index(row, col), type);
  }

  std::unordered_set<int> AvailableCells() const {
    std::unordered_set<int> cells;
    for (int i = 0; i < kNumCells; ++i) {
      if (!data_[i].data()) {
        cells.insert(i);
      }
    }
    return cells;
  }

  bool GameIsOver() { return winner_ != PieceType::kEmpty; }

  PieceType Winner() { return winner_; }

  CellState GetCell(int row, int col) const { return data_[row * Size + col]; }

  CellState GetCell(int index) const { return data_[index]; }

  void PropagateConnections(const CellState& base_piece, int base_index,
                            PieceType base_type) {
    std::vector<int> to_color;
    {
      for (int neighbor_index : neighbors_[base_index]) {
        auto& neighbor = data_[neighbor_index];
        if (neighbor.data() & as_underlying(base_type)) {
          if (neighbor != base_piece) {
            to_color.push_back(neighbor_index);
          }
        }
      }
    }

    while (!to_color.empty()) {
      const int current = to_color.back();
      to_color.pop_back();
      data_[current] = base_piece;
      for (int neighbor_index : neighbors_[current]) {
        auto& neighbor = data_[neighbor_index];
        if (neighbor.data() & as_underlying(base_type)) {
          if (neighbor != base_piece) {
            to_color.push_back(neighbor_index);
          }
        }
      }
    }
  }

  bool PropagateNewPiece(int index, PieceType type) {
    auto& new_piece = data_[index];

    // Copy in connection data from neighbors.
    for (int neighbor_index : neighbors_[index]) {
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
        os << " " << state.GetCell(i, j).GetPieceType() << " ║";
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
        os << " " << state.GetCell(i, j).GetPieceType() << " ║";
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

  const std::array<CellState, kNumCells + 1>& data() const { return data_; }

  static int Index(int row, int col) {
    DCHECK_GE(row, 0);
    DCHECK_GE(col, 0);
    DCHECK_LT(row, Size);
    DCHECK_LT(col, Size);
    return row * Size + col;
  }

  static int SafeIndex(int row, int col) {
    if (row < 0 || row >= Size || col < 0 || col >= Size) {
      return kNumCells;
    }
    return Index(row, col);
  }

  // Precompute indices for the neighbors of each cell.
  static std::array<std::array<int, 6>, kNumCells>
  ComputeNeighbors() {
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

  static std::array<std::array<CellState, 3>, kNumCells>
  ComputeCellTemplates() {
    std::array<std::array<CellState, 3>, kNumCells> out;
    for (int row = 0; row < Size; ++row) {
      for (int col = 0; col < Size; ++col) {
        const int index = Index(row, col);
        {
          auto& new_piece = out[index][as_underlying(PieceType::kHorizontal)];
          new_piece = CellState(PieceType::kHorizontal);
          if (col <= 0) {
            new_piece.SetConnectedToLower();
          }
          if (col >= Size - 1) {
            new_piece.SetConnectedToUpper();
          }
        }
        {
          auto& new_piece = out[index][as_underlying(PieceType::kVertical)];
          new_piece = CellState(PieceType::kVertical);
          if (row <= 0) {
            new_piece.SetConnectedToLower();
          }
          if (row >= Size - 1) {
            new_piece.SetConnectedToUpper();
          }
        }
      }
    }
    return out;
  }

 private:
  static std::array<std::array<int, 6>, kNumCells> neighbors_;
  static std::array<std::array<CellState, 3>, kNumCells> cell_templates_;

  // Cells are stored row-major order, with one more on the end as a sentinel.
  std::array<CellState, kNumCells + 1> data_{};

  PieceType winner_;

  int empty_spaces_;
};

template <int Size>
std::array<std::array<int, 6>, HexState<Size>::kNumCells>
    HexState<Size>::neighbors_ = HexState<Size>::ComputeNeighbors();

template <int Size>
std::array<std::array<CellState, 3>, HexState<Size>::kNumCells>
    HexState<Size>::cell_templates_ = HexState<Size>::ComputeCellTemplates();
