import numpy as np
import sys 

class HexEnv(object):
  HORIZONTAL = 0
  VERTICAL = 1

  @staticmethod
  def flip_player(player):
    return int(not player)

  def __init__(self, board_size=11, pie_rule=False):
    self.reset(board_size, pie_rule)

  # State is stored as 2 boolean boards
  def reset(self, board_size, pie_rule):
    assert isinstance(board_size, int) and board_size >= 1, 'Invalid board size: {}'.format(board_size)
    self.board_size = board_size
    self.board = np.zeros((2, board_size, board_size), dtype=bool)
    self.to_play = HexEnv.HORIZONTAL
    self.winner = None
    self.move_count = 0
    self.pie_rule = pie_rule

  def game_state(self):
    return copy.deepcopy(self.board)

  def to_play(self):
    return self.to_play

  def resign_move(self):
    return self.board_size ** 2

  def valid_move(self, action):
    if self.winner is not None:
      return False

    if action is self.resign_move():
      return True

    coords = self.action_to_coordinate(action)
    if coords[0] >= self.board_size or coords[1] >= self.board_size:
      return False

    if self.pie_rule and self.move_count is 1:
      return True

    taken = self.board[HexEnv.HORIZONTAL, coords[0], coords[1]] or self.board[HexEnv.VERTICAL, coords[0], coords[1]]
    return not taken

  def coordinate_to_action(self, coords):
    return coords[0] * self.board.shape[-1] + coords[1]

  def action_to_coordinate(self, action):
    return action // self.board.shape[-1], action % self.board.shape[-1]

  def get_possible_actions(self):
    if self.winner:
      return []
    if self.move_count is 1 and self.pie_rule:
      free_x, free_y = np.where(np.add.reduce(self.board, 0) >= 0)
    else:
      free_x, free_y = np.where(np.add.reduce(self.board, 0) == 0)
    out = [self.coordinate_to_action([x, y]) for x, y in zip(free_x, free_y)]
    out.append(self.resign_move())
    return out

  def make_move(self, action):
    coords = self.action_to_coordinate(action)
    if not self.valid_move(action):
      raise ValueError
    if action is self.resign_move():
      self.to_play = HexEnv.flip_player(self.to_play)

    self.board[self.to_play, coords[0], coords[1]] = True
    self.to_play = HexEnv.flip_player(self.to_play)

    # If using pie rule, make sure the opponents board is clear
    if self.pie_rule and self.move_count is 1:
      self.board[self.to_play, coords[0], coords[1]] = False 

    self.move_count = self.move_count + 1
    self.winner = self.game_finished()

  def render(self):
    board = self.board
    outfile = sys.stdout
    outfile.write(' ' * 5)
    for j in range(board.shape[1]):
      outfile.write(' ' +  str(j + 1) + '  | ')
    outfile.write('\n')
    outfile.write(' ' * 5)
    outfile.write('-' * (board.shape[1] * 6 - 1))
    outfile.write('\n')
    for i in range(board.shape[1]):
      outfile.write(' ' * (2 + i * 3) +  str(i + 1) + '  |')
      for j in range(board.shape[1]):
        if board[HexEnv.HORIZONTAL, i, j]:
          outfile.write('  H  ')
        elif board[HexEnv.VERTICAL, i, j]:
          outfile.write('  V ')
        else:
          outfile.write('  O  ')
        outfile.write('|')
      outfile.write('\n')
      outfile.write(' ' * (i * 3 + 1))
      outfile.write('-' * (board.shape[1] * 7 - 1))
      outfile.write('\n')

  def game_finished(self):
    # Returns 1 if player 1 wins, -1 if player 2 wins and 0 otherwise
    d = self.board_size 

    inpath = set()
    newset = set()

    for i in range(d):
      if self.board[HexEnv.HORIZONTAL, i, 0] == 1:
        newset.add(i)

    while len(newset) > 0:
      for i in range(len(newset)):
        v = newset.pop()
        inpath.add(v)
        cy = v // d
        cx = v % d
        # Left
        if cy > 0 and self.board[HexEnv.HORIZONTAL, cx, cy - 1] == 1:
          v = (cy - 1) * d + cx
          if v not in inpath:
            newset.add(v)
        # Right
        if cy + 1 < d and self.board[HexEnv.HORIZONTAL, cx, cy + 1] == 1:
          if cy + 1 == d - 1:
            return HexEnv.HORIZONTAL
          v = (cy + 1) * d + cx
          if v not in inpath:
            newset.add(v)
        # Up
        if cx > 0 and self.board[HexEnv.HORIZONTAL, cx - 1, cy] == 1:
          v = cy * d + cx - 1
          if v not in inpath:
            newset.add(v)
        # Down
        if cx + 1 < d and self.board[HexEnv.HORIZONTAL, cx + 1, cy] == 1:
          v = cy * d + cx + 1
          if v not in inpath:
            newset.add(v)
        # Up Right
        if cx > 0 and cy + 1 < d and self.board[HexEnv.HORIZONTAL, cx - 1, cy + 1] == 1:
          if cy + 1 == d - 1:
            return HexEnv.HORIZONTAL
          v = (cy + 1) * d + cx - 1
          if v not in inpath:
            newset.add(v)
        # Left Down
        if cx + 1 < d and cy > 0 and self.board[HexEnv.HORIZONTAL, cx + 1, cy - 1] == 1:
          v = (cy - 1) * d + cx + 1
          if v not in inpath:
            newset.add(v)

    inpath.clear()
    newset.clear()
    for i in range(d):
      if self.board[HexEnv.VERTICAL, 0, i]:
        newset.add(i)

    while len(newset) > 0:
      for i in range(len(newset)):
        v = newset.pop()
        inpath.add(v)
        cx = v // d
        cy = v % d
        # Left
        if cy > 0 and self.board[HexEnv.VERTICAL, cx, cy - 1] == 1:
          v = cx * d + cy - 1
          if v not in inpath:
            newset.add(v)
        # Right
        if cy + 1 < d and self.board[HexEnv.VERTICAL, cx, cy + 1] == 1:
          v = cx * d + cy + 1
          if v not in inpath:
            newset.add(v)
        # Up
        if cx > 0 and self.board[HexEnv.VERTICAL, cx - 1, cy] == 1:
          v = (cx - 1) * d + cy
          if v not in inpath:
            newset.add(v)
        # Down
        if cx + 1 < d and self.board[HexEnv.VERTICAL, cx + 1, cy] == 1:
          if cx + 1 == d - 1:
            return HexEnv.VERTICAL
          v = (cx + 1) * d + cy
          if v not in inpath:
            newset.add(v)
        # Up Right
        if cx > 0 and cy + 1 < d and self.board[HexEnv.VERTICAL, cx - 1, cy + 1] == 1:
          v = (cx - 1) * d + cy + 1
          if v not in inpath:
            newset.add(v)
        # Down Left
        if cx + 1 < d and cy > 0 and self.board[HexEnv.VERTICAL, cx + 1, cy - 1] == 1:
          if cx + 1 == d - 1:
            return HexEnv.VERTICAL
          v = (cx + 1) * d + cy - 1
          if v not in inpath:
            newset.add(v)
    return None
