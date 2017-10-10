#pragma once
#ifdef NDEBUG
#define ELPP_DISABLE_DEBUG_LOGS
#endif
#include <easylogging++.h>
#include <gflags/gflags.h>
#include <backward.hpp>
#include <sstream>
#include <thread>
INITIALIZE_EASYLOGGINGPP

void PrintStackTrace(int sig) {
  LOG(ERROR) << "Crashed with signal: " << sig;
  backward::StackTrace st;
  st.load_here(99);     // Limit the number of trace depth to 99
  st.skip_n_firsts(0);  // This will skip some backward internal function
  backward::Printer p;
  p.snippet = true;
  p.object = true;
  p.color_mode = backward::ColorMode::automatic;
  p.address = true;
  p.print(st, stderr);
  el::Helpers::crashAbort(sig);
}

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
  el::Helpers::setCrashHandler(PrintStackTrace);
}
