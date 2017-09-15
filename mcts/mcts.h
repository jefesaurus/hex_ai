

class MCTS {
 public:
  void GetMove() {
    while (time_left_ > calculation_time_) {
      // Run Simulation
    }
  }

  void RunSimulation() {
    // Copy current state and run a playout.
  }

 private:
  TimePoint start_time_;
  Duration calculation_time_;
};
