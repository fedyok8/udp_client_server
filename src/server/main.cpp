#include <csignal>
#include <atomic>
#include <iostream>

#include "common/cmd_parser.hpp"
#include "server_properties.hpp"
#include "server.hpp"

namespace {
std::atomic<bool> stop_thread = false;
} // namespace

void signal_handler(int /*signal_code*/) {
  stop_thread = true;
}

void main_thread(const ServerProperties& properties) {
  Server server(properties.port_,
                properties.request_length_,
                properties.workers_,
                properties.queue_size_,
                properties.content_filepath_);
  server.Start();

  while (!stop_thread && server.IsOpen() && !server.IsResourceEmpty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

int main(int argc, char** argv) {
  CmdParser cmd(argc, argv);

  if (cmd.FindFlag('h', "help")) {
    ServerProperties::PrintUsage();
    return 0;
  }

  ServerProperties properties(cmd);

  // Set handlers for signal SIGTERM (default kill signal) SIGINT (Ctrl-C)
  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);
  std::clog << "main thread started" << std::endl;
  main_thread(properties);
  std::clog << "main thread stopped" << std::endl;
  return 0;
}
