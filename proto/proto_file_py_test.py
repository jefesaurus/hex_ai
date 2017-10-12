import unittest
import tempfile

from proto import proto_file 
from proto import test_pb2

class BinaryProtoTest(unittest.TestCase):
  def test_write(self):
    message = test_pb2.HelloProto()
    message.number = 5
    message.vals.append(0)
    message.vals.append(1)
    message.vals.append(2)
    message.vals.append(3)
    message.vals.append(4)
    message.label = 'hello'
    tmp = tempfile.NamedTemporaryFile().name
    proto_file.WriteBinaryProto(message, tmp)
    message_back = proto_file.ReadBinaryProto(message, tmp)
    self.assertEqual(message_back.number, message.number)

class TextProtoTest(unittest.TestCase):
  def test_write(self):
    message = test_pb2.HelloProto()
    message.number = 5
    message.vals.append(0)
    message.vals.append(1)
    message.vals.append(2)
    message.vals.append(3)
    message.vals.append(4)
    message.label = 'hello'
    tmp = tempfile.NamedTemporaryFile().name
    proto_file.WriteTextProto(message, tmp)
    message_back = proto_file.ReadTextProto(message, tmp)
    self.assertEqual(message_back.number, message.number)

if __name__ == '__main__':
    unittest.main()
