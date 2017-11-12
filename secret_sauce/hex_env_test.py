import unittest
from secret_sauce.hex_env import HexEnv 

class HexEnvTest(unittest.TestCase):
  def test_smoke(self):
    env = HexEnv(11, True)
    self.assertEqual(env.board_size, 11)
    self.assertEqual(env.winner, None)
    self.assertEqual(env.move_count, 0)
    env.make_move(0)
    env.make_move(0)
    env.make_move(1)
    self.assertEqual(env.winner, None)

  def test_smoke_horizontal_across(self):
    env = HexEnv(3, True)
    env.make_move(0)
    env.make_move(3)
    env.make_move(1)
    env.make_move(5)
    env.make_move(2)
    self.assertEqual(env.winner, HexEnv.HORIZONTAL)

  def test_smoke_horizontal_diagonal(self):
    env = HexEnv(3, True)
    env.make_move(2)
    env.make_move(3)
    env.make_move(4)
    env.make_move(5)
    env.make_move(6)
    self.assertEqual(env.winner, HexEnv.HORIZONTAL)

  def test_smoke_vertical_across(self):
    env = HexEnv(3, True)
    env.make_move(0)
    env.make_move(1)
    env.make_move(2)
    env.make_move(4)
    env.make_move(3)
    env.make_move(7)
    self.assertEqual(env.winner, HexEnv.VERTICAL)

  def test_smoke_vertical_diagonal(self):
    env = HexEnv(3, True)
    env.make_move(0)
    env.make_move(2)
    env.make_move(1)
    env.make_move(4)
    env.make_move(7)
    env.make_move(6)
    self.assertEqual(env.winner, HexEnv.VERTICAL)

if __name__ == '__main__':
  unittest.main()
