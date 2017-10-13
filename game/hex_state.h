#pragma once
#include <bitset>
#include <sstream>

#include "base/constexpr_ceil.h"
#include "base/logging.h"
#include "game/hex_board.h"
#include "game/zobrist.h"

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

// The board state class is optimized to be able to run playouts quickly (and
// determine when the game is over). This is why the state is stored a little
// unintuitively. The pieces are kept as bitvectors of connected components for
// each color(horizontal or vertical).
//
// The groups include sentinel pieces for each side: The horizontal groups
// include a "lower" piece for the left side(col 0) and an "upper" piece of the
// right side. The vertical groups have a lower piece for the top side(row 0),
// and an upper piece for the bottom side.
//
// There are two groups for each color initially which consist of only the
// sentinel pieces created in InitialHorizontalGroups()
//
// Whenever a piece is added to the board we grab the precomputed neighbor mask
// and AND it with all of the connected components of that color. This tells
// us 1: Which connected component to add this piece too, and also 2: whether we
// need to now merge some previous connected components.
//
// The game is over whenever you try to merge the last two groups which
// indicates that there is now a single connected component containing the lower
// and the upper sentinels.
template <int Size>
class HexState {
 public:
  static_assert(Size > 1, "Size must be 2 or greater");
  static constexpr int kSize = Size;
  static constexpr int kNumCells = Size * Size;
  static constexpr int kNumMoves = kNumCells;

  HexState()
      : move_number_(0),
        to_move_(PieceType::kHorizontal),
        first_move_index_(-1),
        winner_(PieceType::kEmpty),
        hash_(0) {}
  // Default copy, move, assign
  HexState(const HexState&) = default;
  HexState& operator=(const HexState&) = default;
  HexState(HexState&&) = default;
  HexState& operator=(HexState&&) = default;

  uint64_t Hash() const {
    return hash_;
  }
  static const ZobristHasher<kNumCells, uint64_t>& Hasher() {
    return hasher_;
  };
  PieceType ToMove() const {
    return to_move_;
  }
  PieceType Winner() const {
    return winner_;
  }
  bool GameIsOver() const {
    return winner_ != PieceType::kEmpty;
  }
  int MoveNumber() const {
    return move_number_;
  }
  HexBoard<Size> Board() const {
    return board_;
  }

  void Resign() {
    DCHECK(!GameIsOver());
    winner_ = FlipPlayer(to_move_);
  }

  void MakeMove(int index) {
    SetPiece(index);
    ++move_number_;
    to_move_ = FlipPlayer(to_move_);
  }

  // Every spot on the board, swap, and resign.
  std::array<bool, kNumMoves> AvailableMoves() const {
    std::array<bool, kNumMoves> available;
    available.fill(true);
    // All spaces are available for the first two moves. If the same space is
    // played twice, that counts as a swap.
    if (move_number_ > 1) {
      board_.AvailableSpaces(&available);
    }
    return available;
  }

  // Return what the hash would be if this move were made.
  uint64_t HashIf(int index) const {
    auto hash = Hash();
    if (move_number_ == 1) {
      if (first_move_index_ == index) {
        // Swap did happen, pull the first move out of the hash.
        hasher_.FlipPiece(first_move_index_, PieceType::kHorizontal, &hash);
      }
    }

    hasher_.FlipPiece(index, to_move_, &hash);
    return hash;
  }

 private:
  void SetPiece(int index) {
    DCHECK_LT(index, kNumCells) << index;
    DCHECK_EQ(board_.GetCell(index), PieceType::kEmpty);
    DCHECK(!GameIsOver());
    DCHECK(to_move_ == PieceType::kHorizontal ||
           to_move_ == PieceType::kVertical);
    if (move_number_ == 0) {
      // Set aside the first move in case we invoke the swap rule in the second
      // move.
      first_move_index_ = index;
      hasher_.FlipPiece(index, to_move_, &hash_);
      return;
    } else if (move_number_ == 1) {
      DCHECK_EQ(to_move_, PieceType::kVertical);
      if (first_move_index_ != index) {
        // Swap doesn't happen, commit original first move.
        board_.SetHorizontalPiece(first_move_index_);
      } else {
        // Swap did happen, pull the first move out of the hash.
        hasher_.FlipPiece(first_move_index_, PieceType::kHorizontal, &hash_);
      }
    }

    if (to_move_ == PieceType::kHorizontal) {
      board_.SetHorizontalPiece(index);
    } else {
      DCHECK_EQ(to_move_, PieceType::kVertical);
      board_.SetVerticalPiece(index);
    }
    hasher_.FlipPiece(index, to_move_, &hash_);
    winner_ = board_.ConnectionStatus();
  }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const HexState<Size>& state) {
    if (state.move_number_ == 1) {
      auto board = state.board_;
      if (state.to_move_ == PieceType::kHorizontal) {
        board.SetVerticalPiece(state.first_move_index_);
      } else {
        board.SetHorizontalPiece(state.first_move_index_);
      }
      os << board;
      return os;
    }
    os << state.board_;
    return os;
  }

 private:
  static ZobristHasher<kNumCells, uint64_t> hasher_;

  int move_number_;
  HexBoard<Size> board_;
  PieceType to_move_;
  int first_move_index_;
  PieceType winner_;
  uint64_t hash_;
};

template <int Size>
ZobristHasher<HexState<Size>::kNumCells, uint64_t> HexState<Size>::hasher_ =
    ZobristHasher<HexState<Size>::kNumCells, uint64_t>();
