#pragma once
#include <bitset>
#include <sstream>
#include "base/logging.h"

enum struct CellState : uint8_t { kEmpty = 0, kWhite = 1, kBlack = 2 };

template <typename Enumeration>
auto as_underlying(Enumeration const value) ->
    typename std::underlying_type<Enumeration>::type {
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

template <int Size>
class HexState {
 public:
  static constexpr int kNumCells = Size * Size;

  HexState() {}

  void SetCell(int row, int col, CellState val) {
    data_[row * Size + col] = val;
  }

  CellState GetCell(int row, int col) const { return data_[row * Size + col]; }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const HexState<Size>& state) {
    int count = 0;
    for (const auto& cell : state.data()) {
      if (count % Size == 0) {
        os << '\n';
      }
      os << cell;
      ++count;
    }
    return os;
  }

  const std::array<CellState, kNumCells>& data() const { return data_; }

 private:
  // Cells are stored row-major order.
  std::array<CellState, kNumCells> data_{};
};

template <typename OStream>
inline OStream& operator<<(OStream& os, const CellState& val) {
  switch (val) {
    case CellState::kEmpty:
      os << '-';
      break;
    case CellState::kWhite:
      os << 'W';
      break;
    case CellState::kBlack:
      os << 'B';
      break;
    default:
      os << '?';
      break;
  }
  return os;
}

class FourCells {
 public:
  FourCells() : data_(0) {}

  template <int Index>
  void SetCell(CellState val) {
    data_ = ((data_ & inverse_masks_[Index]) |
             (as_underlying(val) << shifts_[Index]));
  }

  void SetCell(std::size_t index, CellState val) {
    data_ = ((data_ & inverse_masks_[index]) |
             (as_underlying(val) << shifts_[index]));
  }

  template <int Index>
  CellState GetCell() const {
    return static_cast<CellState>((masks_[Index] & data_) >> shifts_[Index]);
  }

  CellState GetCell(std::size_t index) const {
    return static_cast<CellState>((masks_[index] & data_) >> shifts_[index]);
  }

  const uint8_t& data() const { return data_; }

 private:
  static constexpr std::array<uint8_t, 4> masks_ =
      std::array<uint8_t, 4>{0b00000011, 0b00001100, 0b00110000, 0b11000000};
  static constexpr std::array<uint8_t, 4> inverse_masks_ =
      std::array<uint8_t, 4>{0b11111100, 0b11110011, 0b11001111, 0b00111111};
  static constexpr std::array<std::size_t, 4> shifts_ =
      std::array<std::size_t, 4>{0, 2, 4, 6};
  uint8_t data_;
};

constexpr int32_t ConstExprCeil(float num) {
  return (static_cast<float>(static_cast<int32_t>(num)) == num)
             ? static_cast<int32_t>(num)
             : static_cast<int32_t>(num) + ((num > 0) ? 1 : 0);
}

// A version of HexState that packs the bits tightly so it requires bit shifts
// during access. This appears to be about twice as slow for random read writes.
// Presumably sequential can be optimized a bit.
//
// Takes 1/4 the space however.
template <int Size>
class PackedHexState {
 public:
  static constexpr int kNumCells = Size * Size;
  static constexpr int kNumChars = ConstExprCeil(kNumCells / 4.0);

  PackedHexState() {}

  void SetCell(int row, int col, CellState val) {
    int bit_index = row * Size + col;
    FourCells& cells = data_[bit_index >> 2];
    cells.SetCell(bit_index % 4, val);
  }

  CellState GetCell(int row, int col) const {
    int bit_index = row * Size + col;
    const FourCells& cells = data_[bit_index >> 2];
    return cells.GetCell(bit_index % 4);
  }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const PackedHexState<Size>& state) {
    int count = 0;
    for (const auto& four_cells : state.data()) {
      for (int i = 0; i < 4 && count < PackedHexState<Size>::kNumCells; ++i) {
        if (count % Size == 0) {
          os << '\n';
        }
        os << four_cells.GetCell(i);
        ++count;
      }
    }
    return os;
  }

  const std::array<FourCells, kNumChars>& data() const { return data_; }

 private:
  // Cells are stored row-major order.
  std::array<FourCells, kNumChars> data_{};
};
