#pragma once
#include <bitset>
#include <climits>

#include "base/logging.h"
#include "game/piece_type.h"

template <int NumCells, typename HashType>
class ZobristHasher {
 public:
  static constexpr int kNumPieceTypes = as_underlying(PieceType::kSize);

  // Adding and Removing are the same operation.
  // Do not call this for empty pieces.
  static void FlipPiece(int cell_index, PieceType piece, HashType* hash) {
    DCHECK_NE(piece, PieceType::kEmpty);
    *hash ^= GetBitString(cell_index, piece);
  }

 private:
  static const HashType& GetBitString(int cell_index, PieceType piece) {
    DCHECK_GE(cell_index, 0);
    DCHECK_LT(cell_index, NumCells);
    return ZobristHasher<NumCells, HashType>::table_[cell_index +
                                                     as_underlying(piece) *
                                                         NumCells];
  }

  static HashType RandomBitstring() {
    static constexpr int kNumBits = sizeof(HashType) * CHAR_BIT;
    HashType out = 0;
    HashType one_bit = 1;
    for (int i = 0; i < kNumBits; ++i) {
      if (rand() % 2) {
        out |= one_bit;
      }
      one_bit <<= 1;
    }
    return out;
  }

  static std::array<HashType, NumCells * kNumPieceTypes> InitTable() {
    std::array<HashType, NumCells * kNumPieceTypes> output;
    for (auto& elt : output) {
      elt = RandomBitstring();
    }
    return output;
  }

  static std::array<HashType, NumCells * kNumPieceTypes> table_;
};

template <int NumCells, typename HashType>
std::array<HashType,
           NumCells * ZobristHasher<NumCells, HashType>::kNumPieceTypes>
    ZobristHasher<NumCells, HashType>::table_ =
        ZobristHasher<NumCells, HashType>::InitTable();
