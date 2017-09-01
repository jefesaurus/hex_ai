#include <fstream>

#include "base/file.h"
#include "base/logging.h"

std::string ReadFileOrDie(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.good()) {
    LOG()->info("Reading: '" + filepath + "' failed: " + strerror(errno));
  }
  return std::string(std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>());
}
