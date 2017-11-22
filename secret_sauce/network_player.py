import random
import copy

from secret_sauce.hex_env import HexEnv
from secret_sauce.learner import Learner 

class NetworkPlayer:
  def __init__(self, board_size):
    self.learner = Learner(board_size)
    self.batch_size = 64

  def get_action_and_store(self, env):
    action, prob_dist = self.learner.exploration_policy(env)
    self.learner.store_observation(env, action)
    self.update_learner()
    return action

  def make_move(self, env):
    assert env.winner is None

    action = self.get_action_and_store(env)
    env.make_move(action)

  def update_learner(self):
    self.learner.learn(self.batch_size)
