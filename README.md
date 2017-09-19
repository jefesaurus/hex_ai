# Monte Carlo tree search for computer [Hex](https://en.wikipedia.org/wiki/Hex_(board_game))

# Required
  * bazel
  * gflags
  * libgoogle-perftools-dev

# To run targets

`source activate.sh`

`bazel run path:target`

# Play against itself:
`bazel run mcts:tree_test`

Still need to implement the pie rule, parallelize, and speed up single thread.
