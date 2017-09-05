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
enum struct PieceType : uint8_t { kEmpty = 0, kHorizontal = 1, kVertical = 2 };

template <typename Enumeration>
auto as_underlying(Enumeration const value) ->
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

class CellState {
 public:
  CellState() : data_(0) {}
  CellState(PieceType type) : data_(as_underlying(type)) {}

  // First two bits go to determining which piece, the next 4 hold whether the
  // piece is strongly connected to a given side or not.
  bool IsEmpty() { return ~data_; }

  bool IsHorizontal() { return data_ & 0b00000001; }

  bool IsVertical() { return data_ & 0b00000010; }

  bool IsNotEmpty() { return data_; }

  PieceType GetPieceType() const {
    return static_cast<PieceType>(data_ & 0b00000011);
  }

  void SetPieceType(PieceType piece) { data_ |= as_underlying(piece); }

  void SetConnectedToUpper() { data_ |= 0b00000100; }
  void SetConnectedToLower() { data_ |= 0b00001000; }

  bool IsConnectedToUpper() const { return data_ & 0b00000100; }
  bool IsConnectedToLower() const { return data_ & 0b00001000; }

  const uint8_t& data() const { return data_; };

  uint8_t& data() { return data_; };

  bool operator==(const CellState& other) const {
    return data() == other.data();
  }

  bool operator!=(const CellState& other) const {
    return data() != other.data();
  }

 private:
  uint8_t data_;
};

template <typename OStream>
inline OStream& operator<<(OStream& os, const CellState& val) {
  os << val.GetPieceType();
  return os;
}
