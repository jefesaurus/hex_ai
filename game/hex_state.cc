#include "game/hex_state.h"

constexpr std::array<uint8_t, 4> FourCells::masks_;
constexpr std::array<uint8_t, 4> FourCells::inverse_masks_;
constexpr std::array<std::size_t, 4> FourCells::shifts_;
