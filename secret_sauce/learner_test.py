import unittest
from secret_sauce.learner import Learner 
from secret_sauce.hex_env import HexEnv 
import tensorflow as tf
import numpy as np


class LearnerTest(unittest.TestCase):
  def test_smoke(self):
    learner = Learner(11)
    print "yay"

  def test_smoke2(self):
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
      print learner.evaluate_prob_dist(sess, batch_states)


    print "yay"

if __name__ == '__main__':
  unittest.main()
