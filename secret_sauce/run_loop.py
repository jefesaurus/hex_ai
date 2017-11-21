from secret_sauce.hex_env import HexEnv
from secret_sauce.random_player import RandomPlayer 
from secret_sauce.network_player import NetworkPlayer 

import tensorflow as tf


def run(sess, board_size, num_episodes, players):
  win_count = {HexEnv.HORIZONTAL:0, HexEnv.VERTICAL:0}
  env = HexEnv(board_size)
  for i in range(num_episodes):
    while env.winner is None:
      to_play = env.to_play
      players[to_play].make_move(sess, env)
    win_count[env.winner] = win_count[env.winner] + 1
    env.reset()
  return win_count


BOARD_SIZE = 11

players = {}
players[HexEnv.HORIZONTAL] = NetworkPlayer(BOARD_SIZE)
players[HexEnv.VERTICAL] = RandomPlayer()

init = tf.global_variables_initializer()
with tf.Session() as sess:
  sess.run(init)
  count = run(sess, BOARD_SIZE, 100, players)
  print count
