import sys
import tensorflow as tf

from secret_sauce.hex_env import HexEnv
from secret_sauce.random_player import RandomPlayer 
from secret_sauce.network_player import NetworkPlayer 


def run(board_size, num_episodes, players):
  win_count = {HexEnv.HORIZONTAL:0, HexEnv.VERTICAL:0}
  env = HexEnv(board_size)
  for i in range(num_episodes):
    print i, win_count
    sys.stdout.flush()
    while env.winner is None:
      to_play = env.to_play
      players[to_play].make_move(env)
    win_count[env.winner] = win_count[env.winner] + 1
    env.reset()
  return win_count


BOARD_SIZE = 11

# tf graphs are set up when the player object is created.
players = {}
network_player = NetworkPlayer(BOARD_SIZE, False)
iteration = 4617
network_player.learner.load_model('/tmp/hex_models/%s_network-player-v0.ckpt' % iteration)
players[HexEnv.HORIZONTAL] = network_player 
players[HexEnv.VERTICAL] = RandomPlayer() 
count = run(BOARD_SIZE, 1000, players)
print count
