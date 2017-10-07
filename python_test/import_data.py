"""Imports board data from a file and converts it into a numpy
  array for tensorflow to use.
"""

import random

import numpy as np
import tensorflow as tf
# TODO - install protoc in the Docker container
from hex_state_pb2 import HexState, PieceType

BOARD_SIZE = 11


def create_proto_file(filename, num_lines=5):
  with open(filename, 'w+') as f:
    for i in xrange(num_lines):
      h = HexState()
      h.size = BOARD_SIZE
      h.to_move = 1
      for pieceIndex in xrange(BOARD_SIZE * BOARD_SIZE):
        h.positions.append(random.randint(0, 2))

      f.write(h.SerializeToString() + "\n")

  return


def extract_data(filename, board_size=BOARD_SIZE):
  """Extract the boards into a 4D tensor, with the fourth dimension always
  set to 1:

      [board index, y, x, 1]
  """
  print 'Extracting: ' + str(filename)

  returnData = []

  with open(filename, 'r') as f:
    for plaintextProto in f.readlines():
      p = HexState()
      p.ParseFromString(plaintextProto[:-1])

      # Converting repeated field of enum piece types into a numpy array
      board = np.array([x for x in p.positions])
      board = board.reshape(p.size, p.size)

      returnData.append(board)

  return np.array(returnData)


if __name__ == "__main__":
  create_proto_file('test_data.txt', 5)
  data = extract_data('test_data.txt')
  print '------'
  print data
