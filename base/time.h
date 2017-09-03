#pragma once
#include <cassert>
#include <chrono>
#include <iostream>

using ClockType = std::chrono::steady_clock;
using TimePoint = ClockType::time_point;
using Duration = ClockType::duration;

template <class T>
inline T ToSeconds(Duration duration) {
  return std::chrono::duration<T, std::chrono::seconds::period>(duration)
      .count();
}

template <class T>
inline T ToSecondsSinceReference(TimePoint time_point) {
  return ToSeconds<T>(time_point - TimePoint());
}

template <class T>
inline Duration FromSeconds(T seconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::duration<T, std::chrono::seconds::period>(seconds));
}

template <class T>
inline TimePoint FromSecondsSinceReference(T seconds) {
  return TimePoint() + FromSeconds(seconds);
}

inline Duration FromHz(double hz) {
  assert(hz > 0.0);
  return FromSeconds(1.0 / hz);
}

inline double ToHz(Duration period) { return 1.0 / ToSeconds<double>(period); }

template<class OStream>
OStream& operator<<(OStream& os, const TimePoint& time_point) {
  os << ToSecondsSinceReference<double>(time_point) << "s";
  return os;
}

template<class OStream>
OStream& operator<<(OStream& os, const Duration& duration) {
  os << ToSeconds<double>(duration) << "s";
  return os;
}
