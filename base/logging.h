#pragma once
#define ELPP_STL_LOGGING
#include <easylogging++.h>
#include <gflags/gflags.h>
#include <thread>
#include <sstream>
INITIALIZE_EASYLOGGINGPP

// Read flags for EasyLogging
inline void ConfigureEasyLogging(int* argc, char*** argv) {
  START_EASYLOGGINGPP(*argc, *argv);
  el::Configurations default_conf;
  default_conf.setToDefault();
  default_conf.setGlobally(el::ConfigurationType::Format,
                           "[%levshort|%datetime|%loc] %msg");
  default_conf.setGlobally(el::ConfigurationType::ToFile, "false");
  el::Loggers::reconfigureLogger("default", default_conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
}
