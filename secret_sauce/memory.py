import random

# Store data in a circular buffer of size `size` disposes of the oldest values.
class Memory:
  def __init__(self, size):
    self._data = []
    self._index = 0
    self._size = size

  def full(self):
    return len(self._data) >= self._size

  def store(self, value):
    if len(self._data) == self._size:
        self._data[self._index] = value
    else:
        self._data.append(value)
    self._index = (self._index + 1) % self._size

  def sample(self, num):
    return [self._data[random.randint(0, len(self._data) - 1)] for i in xrange(num)]
