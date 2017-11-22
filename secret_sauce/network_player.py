import random
import copy

from secret_sauce.memory import Observation, Memory 
from secret_sauce.hex_env import HexEnv
from secret_sauce.learner import Learner 

class NetworkPlayer:
  def __init__(self, board_size):
    self.memory = Memory(10000)
    self.learner = Learner(board_size)

  def get_action(self, env):
    action, prob_dist = self.learner.exploration_policy(env)
    return action

  def make_move(self, env):
    assert env.winner is None

    # Store the state before action
    obs = Observation()
    obs.set_state(copy.deepcopy(env))

    # random policy
    action = self.get_action(env)

    obs.set_action(copy.deepcopy(action))
    env.make_move(action)

    if env.winner is not None:
      obs.set_reward(1)
    else:
      obs.set_reward(0)
    self.memory.store(obs)
