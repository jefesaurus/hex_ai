import unittest
import tensorflow as tf

class TensorFlowTest(unittest.TestCase):
  def test_hello(self):
    input =  'Hello, TensorFlow!'
    hello = tf.constant(input)
    sess = tf.Session()
    output = sess.run(hello)
    self.assertEqual(output, input)

if __name__ == '__main__':
    unittest.main()
