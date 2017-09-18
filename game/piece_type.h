#pragma once
/*

                           Top       Upper Corner
                         __________
                        /         /
              Left     /         /
                      /         /  Right
                     /_________/

       Lower Corner    Bottom
*/

// Horizontal pieces are trying to connect horizontally, Vertical pieces are
// trying to connect vertically
enum struct PieceType : uint8_t {
  kEmpty = 0,
  kHorizontal = 1,
  kVertical = 2,
  kSize = 3
};

template <typename Enumeration>
constexpr auto as_underlying(Enumeration const value) ->
    typename std::underlying_type<Enumeration>::type {
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

template <typename OStream>
inline OStream& operator<<(OStream& os, const PieceType& val) {
  switch (val) {
    case PieceType::kEmpty:
      os << '-';
      break;
    case PieceType::kHorizontal:
      os << 'H';
      break;
    case PieceType::kVertical:
      os << 'V';
      break;
    default:
      os << '?';
      break;
  }
  return os;
}

static constexpr std::array<PieceType, 3> flip_index = {
    PieceType::kEmpty, PieceType::kVertical, PieceType::kHorizontal};
constexpr PieceType FlipPlayer(const PieceType& piece_type) {
  return flip_index[as_underlying(piece_type)];
}
