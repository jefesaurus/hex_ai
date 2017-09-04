#include "base/logging.h"
#include "base/never_destroyed.h"

namespace {
// Returns the default logger.  NOTE: This function assumes that it is mutexed,
// as in the initializer of a static local.
std::shared_ptr<spdlog::logger> onetime_create_log() {
  // Check if anyone has already set up a logger named "console".  If so, we
  // will just return it; if not, we'll create our own default one.
  std::shared_ptr<spdlog::logger> result(spdlog::get("console"));
  if (!result) {
    // We use the logger_mt (instead of logger_st) so more than one thread can
    // use this logger and have their messages be staggered by line, instead of
    // co-mingling their character bytes.
    result = spdlog::stderr_logger_mt("console");
  }
  result->set_pattern("[%L | %T | %t] %v");
  return result;
}
}  // namespace

spdlog::logger* GetLogger() {
  static const never_destroyed<std::shared_ptr<spdlog::logger>> g_logger(
      onetime_create_log());
  return g_logger.access().get();
}
