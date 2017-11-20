from collections import deque

class Observation:
  # The state before the action is applied
  def set_state(self, state):
    self.state = state

  # The action that is applied to the state
  def set_action(self, action):
    self.action = action

  # The reward yielded from this action applied to this state
  def set_reward(self, reward):
    self.reward = reward

class Memory:
  def __init__(self, size):
    self.observations = deque([],size)

  def store(self, observation):
    self.observations.append(observation)

  # TODO(glalonde) sample
