#include "udp_server.hpp"

#include <csignal>
#include <atomic>
#include <iostream>

namespace {
std::atomic<bool> stop_thread = false;
} // namespace

void signal_handler(int /*signal_code*/) {
  stop_thread = true;
}

void main_thread() {
  udp::Server server(8888, 1024);

  while (!stop_thread && server.is_open()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}


int main() {
  // Set handlers for signal SIGTERM (default kill signal) SIGINT (Ctrl-C)
  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);
  std::clog << "Started main_thread" << std::endl;
  main_thread();
  std::clog << "Stopped main_thread" << std::endl;
  return 0;
}
