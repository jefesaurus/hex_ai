#pragma once
#include "base/time.h"

// Measures wall clock time.
class WallTimer {
 public:
  using Clock = std::chrono::high_resolution_clock;

  WallTimer() : started_(false), stopped_(false) {}

  void Start() {
    started_ = true;
    stopped_ = false;
    start_time_ = Clock::now();
  }

  void Stop() {
    stop_time_ = Clock::now();
    stopped_ = true;
  }

  Duration Elapsed() const {
    if (!started_) {
      return FromSeconds(0);
    } else if (!stopped_) {
      return Clock::now() - start_time_;
    } else {
      return stop_time_ - start_time_;
    }
  }

 private:
  bool started_;
  bool stopped_;
  Clock::time_point start_time_;
  Clock::time_point stop_time_;
};
