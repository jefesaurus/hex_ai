from secret_sauce.hex_env import HexEnv
from secret_sauce.random_player import RandomPlayer 

def run(board_size, num_episodes, players):
  win_count = {HexEnv.HORIZONTAL:0, HexEnv.VERTICAL:0}
  env = HexEnv(board_size)
  for i in range(num_episodes):
    while env.winner is None:
      to_play = env.to_play
      players[to_play].make_move(env)
    win_count[env.winner] = win_count[env.winner] + 1
    env.reset()
  return win_count

players = {}
players[HexEnv.HORIZONTAL] = RandomPlayer()
players[HexEnv.VERTICAL] = RandomPlayer()

count = run(11, 100, players)
print count
