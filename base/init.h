#pragma once
#include "base/logging.h"

void Init(int* argc, char*** argv) {
  google::ParseCommandLineFlags(argc, argv, false);
}
