#pragma once
// The two players are "Horizontal" and "Vertical" instead of white and black,
// where Horizontal is trying to connect column 0 to column board size, and
// vertical is trying to connect row 0 to row board size. Horizontal goes first.
enum struct PieceType : uint8_t {
  kEmpty = 0,
  kHorizontal = 1,
  kVertical = 2,
  kSize = 3
};

// Get the underlying uint8_t/char byte that the enum value is represented with.
// This is good for using the enum directly as an index.
template <typename Enumeration>
constexpr auto as_underlying(Enumeration const value) ->
    typename std::underlying_type<Enumeration>::type {
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

// Steam operator for pretty printing PieceType
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

// Flips Horizontal and Vertical, but empty stays the same.
static constexpr std::array<PieceType, 3> flip_index = {
    PieceType::kEmpty, PieceType::kVertical, PieceType::kHorizontal};
constexpr PieceType FlipPlayer(const PieceType& piece_type) {
  return flip_index[as_underlying(piece_type)];
}
