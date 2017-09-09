#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <bitset>
#include <iostream>
#include <vector>

#include "base/logging.h"

constexpr int32_t ConstexprCeil(float num) {
  return (static_cast<float>(static_cast<int32_t>(num)) == num)
             ? static_cast<int32_t>(num)
             : static_cast<int32_t>(num) + ((num > 0) ? 1 : 0);
}

template <int NumBits>
class SimdBitset256 {
 public:
  static constexpr int kRegisterSize = 256;
  static constexpr int kNumVectors =
      ConstexprCeil(NumBits / static_cast<float>(kRegisterSize));
  static constexpr int kSpan = kRegisterSize / 8;
  static constexpr int kNumBytes = kRegisterSize * kNumVectors;

  SimdBitset256() : data_{} {}

  static uint8_t Bitmask(int index) { return (0x01 << index); }

  void Set(int index) {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, NumBits);
    auto& bytes = *reinterpret_cast<std::array<uint8_t, kNumBytes>*>(&data_);
    auto& byte = bytes[index / 8];
    byte |= (0x01 << (index % 8));
  }

  bool Test(int index) const {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, NumBits);
    auto& bytes =
        *reinterpret_cast<const std::array<uint8_t, kNumBytes>*>(&data_);
    auto& byte = bytes[index / 8];
    return byte & (0x01 << (index % 8));
  }

  void Clear() {
    reinterpret_cast<std::array<uint8_t, kNumBytes>*>(&data_)->fill(0);
  }

  bool Intersects(const SimdBitset256<NumBits>& rhs) const {
    const auto& rdata = rhs.data();
    for (int i = 0; i < kNumVectors; ++i) {
      if (!_mm256_testz_si256(data_[i], rdata[i])) {
        return true;
      }
    }
    return false;
  }

  SimdBitset256<NumBits>& operator|=(const SimdBitset256<NumBits>& rhs) {
    const auto& rdata = rhs.data();
    for (int i = 0; i < kNumVectors; ++i) {
      data_[i] = _mm256_or_si256(data_[i], rdata[i]);
    }
    return *this;
  }

  std::array<__m256i, kNumVectors>& data() { return data_; };
  const std::array<__m256i, kNumVectors>& data() const { return data_; };

  std::bitset<NumBits> ToBitset() const {
    std::bitset<NumBits> out;
    for (int i = 0; i < NumBits; ++i) {
      out[i] = Test(i);
    }
    return out;
  }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const SimdBitset256<NumBits>& bits) {
    os << bits.ToBitset();
    return os;
  }

 private:
  std::array<__m256i, kNumVectors> data_;
};

template <int NumBits>
class SimdBitset128 {
 public:
  static constexpr int kRegisterSize = 128;
  static constexpr int kNumVectors =
      ConstexprCeil(NumBits / static_cast<float>(kRegisterSize));
  static constexpr int kSpan = kRegisterSize / 8;
  static constexpr int kNumBytes = kRegisterSize * kNumVectors;

  SimdBitset128() : data_{} {}

  static uint8_t Bitmask(int index) { return (0x01 << index); }

  void Set(int index) {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, NumBits);
    auto& bytes = *reinterpret_cast<std::array<uint8_t, kNumBytes>*>(&data_);
    auto& byte = bytes[index / 8];
    byte |= (0x01 << (index % 8));
  }

  bool Test(int index) const {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, NumBits);
    auto& bytes =
        *reinterpret_cast<const std::array<uint8_t, kNumBytes>*>(&data_);
    auto& byte = bytes[index / 8];
    return byte & (0x01 << (index % 8));
  }

  void Clear() {
    reinterpret_cast<std::array<uint8_t, kNumBytes>*>(&data_)->fill(0);
  }

  bool Intersects(const SimdBitset128<NumBits>& rhs) const {
    const auto& rdata = rhs.data();
    for (int i = 0; i < kNumVectors; ++i) {
      if (!_mm_testz_si128(data_[i], rdata[i])) {
        return true;
      }
    }
    return false;
  }

  SimdBitset128<NumBits>& operator|=(const SimdBitset128<NumBits>& rhs) {
    const auto& rdata = rhs.data();
    for (int i = 0; i < kNumVectors; ++i) {
      data_[i] = _mm_or_si128(data_[i], rdata[i]);
    }
    return *this;
  }

  std::array<__m128i, kNumVectors>& data() { return data_; };
  const std::array<__m128i, kNumVectors>& data() const { return data_; };

  std::bitset<NumBits> ToBitset() const {
    std::bitset<NumBits> out;
    for (int i = 0; i < NumBits; ++i) {
      out[i] = Test(i);
    }
    return out;
  }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const SimdBitset128<NumBits>& bits) {
    os << bits.ToBitset();
    return os;
  }

 private:
  std::array<__m128i, kNumVectors> data_;
};

template <int NumBits>
class StdBitset {
  void Set(int index) { data_[index] = true; }

  bool Test(int index) const { return data_[index]; }

  void Clear() { data_.reset(); }

  bool Intersects(const StdBitset<NumBits>& rhs) const {
    return (data() & rhs.data()).any();
  }

  template <typename OStream>
  friend OStream& operator<<(OStream& os, const StdBitset<NumBits>& bits) {
    os << bits.data();
    return os;
  }

  std::bitset<NumBits>& data() { return data_; };
  const std::bitset<NumBits>& data() const { return data_; };

 private:
  std::bitset<NumBits> data_;
};
