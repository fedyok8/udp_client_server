#include "client_request.hpp"

#include <sstream>
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

ClientRequest::ClientRequest(CmdParser& cmd) {
  std::string s_port = cmd.FindOption('p', "port");
  std::string s_max_responce_length = cmd.FindOption('l', "length");
  std::string s_resource = cmd.FindOption('r', "resource");

  int tmp_port = -1, tmp_length = -1;

  if (!ParseInt(s_port, tmp_port)) {
    std::clog << "Cannot parse " << "port" << " from " << s_port << std::endl;
  }
  if (!ParseInt(s_max_responce_length, tmp_length)) {
    std::clog << "Cannot parse " << "max_responce_length"
              << " from " << s_max_responce_length << std::endl;
  }

  if (tmp_port > 0)
    port_ = tmp_port;
  if (tmp_length > 0)
    max_responce_length_ = tmp_length;

  if (!s_resource.empty()) {
    resource_ = s_resource;
  }

}

void ClientRequest::PrintUsage() {
  std::clog << "Usage:\n"
            << "\t-h, --help\n"
            << "\t\tprint help message\n"

            << "\t-p, --port <port>\n"
            << "\t\tsetup port (default is 8888)\n"

            << "\t-l, --length <max_responce_length>\n"
            << "\t\tsetup maximum responce length (default is 1024 bytes)\n"

            << "\t-r, --resource <resource>\n"
            << "\t\tsetup resource to send in request (default is test_resource)\n"

            << std::flush;
}

std::string ClientRequest::to_string() const {
  std::stringstream ss;
  ss << "Request(port="
     << port_
     << ", resource="
     << resource_
     << ", max_responce_length="
     << max_responce_length_
     << ")";
  return ss.str();
}
