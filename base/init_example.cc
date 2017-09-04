#include "base/init.h"

DEFINE_string(suffix, "rofl", "Suffix for string");

int main(int argc, char** argv) {
  Init(&argc, &argv);
  LOG(INFO, "yoo {}", FLAGS_suffix);
  return 0;
}
