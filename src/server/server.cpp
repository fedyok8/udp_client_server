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

void main_thread(int port, int request_length, int workers, int queue_size) {
  udp::Server server(port, request_length, workers, queue_size);

  while (!stop_thread && server.is_open()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

class CmdLine {
 public:
  CmdLine(int argc, char** argv);
  bool FindFlag(char f_short, std::string f_long) const;
  std::string FindOption(char f_short, std::string f_long) const;

 private:
  static bool IsShortArg(const std::string& arg);
  static bool IsLongArg(const std::string& arg);
  static bool IsArg(const std::string& arg);
  std::vector<std::string> args_;
};

CmdLine::CmdLine(int argc, char** argv)
  : args_(argc) {
  for (int i = 0; i < args_.size(); ++i) {
    args_[i] = std::string(argv[i]);
  }
}

bool CmdLine::FindFlag(char f_short, std::string f_long) const {
  std::string s = std::string("-") + f_short;
  std::string l = std::string("--") + f_long;
  for (int i = 0; i < args_.size(); ++i) {
    if (args_[i] == s || args_[i] == l) {
      return true;
    }
  }
  return false;
}

std::string CmdLine::FindOption(char f_short, std::string f_long) const {
  std::string s = std::string("-") + f_short;
  std::string l = std::string("--") + f_long;
  for (int i = 0; i < args_.size() - 1; ++i) {
    if ((args_[i] == s || args_[i] == l) && !IsArg(args_[i + 1])) {
      return args_[i + 1];
    }
  }
  return {};
}

bool CmdLine::IsShortArg(const std::string& arg) {
  if (arg.size() < 2) {
    return false;
  }
  return arg[0] == '-';
}

bool CmdLine::IsLongArg(const std::string& arg) {
  if (arg.size() < 3) {
    return false;
  }
  return (arg[0] == '-') && (arg[1] == '-');
}

bool CmdLine::IsArg(const std::string& arg) {
  return IsLongArg(arg) || IsShortArg(arg);
}

void PrintHelp() {
  std::clog << "Usage:\n"
            << "\t-h, --help\n"
            << "\t\tprint help message\n"

            << "\t-p, --port <port>\n"
            << "\t\tsetup port (default is 8888)\n"

            << "\t-w, --workers <workers>\n"
            << "\t\tsetup number of workers (default is 4)\n"

            << "\t-s, --size <queue_size>\n"
            << "\t\tsetup queue size (default is 10 requests)\n"

            << "\t-l, --length <request_length>\n"
            << "\t\tsetup maximum request length (default is 1024 bytes)\n"

            << std::flush;
}

bool parse(std::string opt, int& val) {
  if (opt.empty()) {
    return true;
  }
  int tmp;
  try {
    tmp = std::stoi(opt);
  }
  catch(std::invalid_argument& e) {
    return false;
  }
  if (std::to_string(tmp) != opt) {
    return false;
  }
  val = tmp;
  return true;
}

bool parseArgs(const CmdLine& cmd,
               int& port,
               int& request_length,
               int& workers,
               int& queue_size) {
  port = 8888;
  request_length = 1024;
  workers = 4;
  queue_size = 10;

  std::string s_port = cmd.FindOption('p', "port");
  std::string s_workers = cmd.FindOption('w', "workers");
  std::string s_queue_size = cmd.FindOption('s', "size");
  std::string s_request_lenth = cmd.FindOption('l', "length");

  if (!parse(s_port, port)) {
    std::clog << "Cannot parse " << "port" << " from " << s_port << std::endl;
    return false;
  }
  if (!parse(s_workers, workers)) {
    std::clog << "Cannot parse " << "workers" << " from " << s_workers << std::endl;
    return false;
  }
  if (!parse(s_queue_size, queue_size)) {
    std::clog << "Cannot parse " << "queue_size" << " from " << s_queue_size << std::endl;
    return false;
  }
  if (!parse(s_request_lenth, request_length)) {
    std::clog << "Cannot parse " << "request_length" << " from " << s_request_lenth << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  CmdLine cmd(argc, argv);

  if (cmd.FindFlag('h', "help")) {
    PrintHelp();
    return 0;
  }

  int port, max_request_size, workers, queue_size;

  if (!parseArgs(cmd, port, max_request_size, workers, queue_size)) {
    return -1;
  }

  // Set handlers for signal SIGTERM (default kill signal) SIGINT (Ctrl-C)
  std::signal(SIGTERM, signal_handler);
  std::signal(SIGINT, signal_handler);
  std::clog << "main thread started" << std::endl;
  main_thread(port, max_request_size, workers, queue_size);
  std::clog << "main thread stopped" << std::endl;
  return 0;
}
