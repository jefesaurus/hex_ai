import unittest
import numpy as np
import tensorflow as tf
from secret_sauce.hex_env import HexEnv
from secret_sauce.learner import Learner


class LearnerTest(unittest.TestCase):
  def test_init(self):
    learner = Learner(11)

  def test_run_estimator(self):
    env = HexEnv(11)
    env.make_move(2)
    env.make_move(3)
    state = Learner.convert_state(env)
    state_batch = np.expand_dims(state, 0)
    learner = Learner(11)
    init = tf.global_variables_initializer()
    learner.run_estimator(state_batch)

  def test_exploration_policy(self):
    env = HexEnv(11)
    env.make_move(2)
    env.make_move(3)
    learner = Learner(11)
    action, value_dist = learner.exploration_policy(env)

  def test_vis(self):
    learner = Learner(11)
    learner.visualize_graph()

if __name__ == '__main__':
  unittest.main()
