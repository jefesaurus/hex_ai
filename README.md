# Monte Carlo tree search for computer [Hex](https://en.wikipedia.org/wiki/Hex_(board_game))


[![Build Status](https://travis-ci.org/jefesaurus/hex_ai.svg?branch=master)](https://travis-ci.org/jefesaurus/hex_ai/)

# Required
  * docker (for build environment, see Dockerfile for the actual requirements)

# To run targets

`./start_docker_shell.sh`

`bazel run path:target`

# Play against itself:
`bazel run mcts:tree_test`

Still need to implement the pie rule, parallelize, and speed up single thread.
