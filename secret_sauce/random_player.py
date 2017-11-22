import random
import copy

from secret_sauce.hex_env import HexEnv

class RandomPlayer:
  def make_move(self, env):
    assert env.winner is None

    # random policy
    actions = env.get_possible_actions()
    action = actions[random.randint(0, len(actions) - 1)]
    env.make_move(action)
