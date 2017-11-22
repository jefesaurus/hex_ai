import unittest
from secret_sauce.memory import Memory 

class MemoryTest(unittest.TestCase):
  def test_smoke(self):
    memory = Memory(10)
    for i in xrange(50):
      memory.store(i)
    print memory.sample(10)

if __name__ == '__main__':
  unittest.main()
