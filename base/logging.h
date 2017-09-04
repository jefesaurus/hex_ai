#pragma once
#include <gflags/gflags.h>
#include <spdlog/fmt/ostr.h>
#define SPDLOG_TRACE_ON
#include <spdlog/spdlog.h>

spdlog::logger* GetLogger();

#define LOG(INFO, ...)            \
  GetLogger()->info("[ " __FILE__ \
                    ":" SPDLOG_STR_HELPER(__LINE__) " ] " __VA_ARGS__)
