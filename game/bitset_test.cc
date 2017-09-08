#include <x86intrin.h>
#include "base/init.h"
#include "game/static_simd_bitset.h"

void TestSet() {
  Bitset<100> bits;
  bits.Set(0);
  LOG(INFO) << bits;
  bits.Set(99);
  LOG(INFO) << bits;
}

void TestIntersects() {
  Bitset<100> bits1;
  Bitset<100> bits2;
  bits1.Set(0);
  bits2.Set(99);
  CHECK(!bits1.Intersects(bits2));
  bits1.Set(99);
  CHECK(bits1.Intersects(bits2));
}

void TestOrEquals() {
  Bitset<100> bits1;
  Bitset<100> bits2;
  bits1.Set(0);
  bits2.Set(99);
  bits1 |= bits2;
  CHECK(bits1.Test(0));
  CHECK(bits1.Test(99));
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  TestSet();
  TestIntersects();
  TestOrEquals();
  return 0;
}
