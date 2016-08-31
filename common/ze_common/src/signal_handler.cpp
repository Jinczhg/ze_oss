// Copyright (C) ETH Zurich, Wyss Zurich, Zurich Eye - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

#include <ze/common/signal_handler.hpp>

#include <cstdint>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <ze/common/logging.hpp>

namespace ze {

// enforce local linkage
namespace {

volatile bool* s_simple_flag = nullptr;

void handleSignalSimple(int signal)
{
  LOG(WARNING) << "Signal handler was called with signal " << signal;
  *s_simple_flag = false;
}

void installSignalHandlerSimple(int sig)
{
  struct sigaction handler;
  handler.sa_handler = handleSignalSimple;
  sigfillset(&handler.sa_mask); // block all pending signals
  handler.sa_flags = 0;
  if (sigaction(sig, &handler, NULL) < 0)
  {
    LOG(FATAL) << "cannot install the signal handler for signal " << sig
               << " : " << std::strerror(errno);
  }
}

void clearSignalHandlerSimple(int sig)
{
  struct sigaction handler;
  handler.sa_handler = SIG_DFL;
  sigemptyset(&handler.sa_mask);
  handler.sa_flags = 0;
  if (sigaction(sig, &handler, NULL) < 0)
  {
    LOG(ERROR) << "failed to uninstall the signal handler for signal "
               << sig << ": " << std::strerror(errno);
  }
}

} // unnamed namespace

SimpleSigtermHandler::SimpleSigtermHandler(volatile bool &flag)
{
  CHECK(s_simple_flag == nullptr) << "Signal handler already installed";
  s_simple_flag = &flag;

  // note: if one of the functions below throws,
  // s_simple_flag will remain set since the destructor will not be called
  // however, an error here will lead to process termination anyway
  installSignalHandlerSimple(SIGHUP);
  installSignalHandlerSimple(SIGTERM);
  installSignalHandlerSimple(SIGINT);
}

SimpleSigtermHandler::~SimpleSigtermHandler()
{
  clearSignalHandlerSimple(SIGINT);
  clearSignalHandlerSimple(SIGTERM);
  clearSignalHandlerSimple(SIGHUP);

  s_simple_flag = nullptr;
}

} // namespace ze