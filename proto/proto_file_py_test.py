import unittest
import tempfile

from proto import test_pb2

class BinaryProtoTest(unittest.TestCase):
  def test_write(self):
    tmp = tempfile.NamedTemporaryFile().name

if __name__ == '__main__':
    unittest.main()
