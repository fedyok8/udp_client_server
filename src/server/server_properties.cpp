#include "server_properties.hpp"

#include <stdexcept>
#include <iostream>

namespace {

bool ParseInt(std::string opt, int& val) {
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

} // namespace

ServerProperties::ServerProperties(CmdParser& cmd) {
  std::string s_port = cmd.FindOption('p', "port");
  std::string s_workers = cmd.FindOption('w', "workers");
  std::string s_queue_size = cmd.FindOption('s', "size");
  std::string s_request_lenth = cmd.FindOption('l', "length");
  std::string s_content_filepath = cmd.FindOption('c', "content");

  if (!ParseInt(s_port, port_)) {
    std::clog << "Cannot parse " << "port" << " from " << s_port << std::endl;
  }
  if (!ParseInt(s_workers, workers_)) {
    std::clog << "Cannot parse " << "workers" << " from "
              << s_workers << std::endl;
  }
  if (!ParseInt(s_queue_size, queue_size_)) {
    std::clog << "Cannot parse " << "queue_size" << " from "
              << s_queue_size << std::endl;
  }
  if (!ParseInt(s_request_lenth, request_length_)) {
    std::clog << "Cannot parse " << "request_length"
              << " from " << s_request_lenth << std::endl;
  }
  if (!s_content_filepath.empty()) {
    content_filepath_ = s_content_filepath;
  }
}
void ServerProperties::PrintUsage() {
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

            << "\t-c, --content <content_filepath>\n"
            << "\t\tsetup filepath to content file (default is ./resource_content.conf)\n"

            << std::flush;
}
