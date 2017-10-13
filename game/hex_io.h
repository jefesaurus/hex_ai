#pragma once

#include "game/hex_board.h"
#include "proto/hex_state.pb.h"

PieceType FromProto(const hex_ai::proto::PieceType& proto_piecetype) {
  return PieceType(as_underlying(proto_piecetype));
}

template <int Size>
HexBoard<Size> FromProto(const hex_ai::proto::HexBoard& proto_board) {
  int size = proto_board.size();
  CHECK_EQ(size, Size);
  CHECK_EQ(proto_board.positions_size(), HexBoard<Size>::kNumCells);
  HexBoard<Size> board;
  for (int i = 0; i < HexBoard<Size>::kNumCells; ++i) {
    if (FromProto(proto_board.positions(i)) != PieceType::kEmpty) {
      board.SetPiece(i, FromProto(proto_board.positions(i)));
    }
  }
  return board;
}

// TODO(glalonde) ToProto
