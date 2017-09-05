#include "base/logging.h"

int main(int argc, char** argv) {
  ConfigureEasyLogging(&argc, &argv);
  LOG(INFO) << "yoo";
  LOG(INFO) << "yoo"
            << "rofl";
  LOG(WARNING) << "whoah there";
  std::vector<std::string> strings = {"yoo", "rofl"};
  LOG(INFO) << strings;
  CHECK_EQ(2, 2);
  return 0;
}
