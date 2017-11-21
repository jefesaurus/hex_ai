import unittest

import numpy as np
import tensorflow as tf
from secret_sauce.hex_env import HexEnv
from secret_sauce.learner import Learner


class LearnerTest(unittest.TestCase):
  def test_init(self):
    learner = Learner(11)
    print "yay"

  def test_evaluate_prob_dist(self):
    env = HexEnv(11)
    env.make_move(2)
    env.make_move(3)
    state = Learner.convert_state(env)
    batch_shape = [1]
    batch_shape.extend(state.shape)
    batch_states = np.zeros(batch_shape)
    batch_states[0] = state
    learner = Learner(11)
    init = tf.global_variables_initializer()
    with tf.Session() as sess:
      sess.run(init)
      learner.evaluate_prob_dist(sess, batch_states)

  def test_exploration_policy(self):
    env = HexEnv(11)
    env.make_move(2)
    env.make_move(3)
    learner = Learner(11)
    init = tf.global_variables_initializer()
    with tf.Session() as sess:
      sess.run(init)
      action, prob_dist = learner.exploration_policy(sess, env)


if __name__ == '__main__':
  unittest.main()
