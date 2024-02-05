#ifndef SRC_CLIENT_CLIENT_REQUEST_HPP__
#define SRC_CLIENT_CLIENT_REQUEST_HPP__

#include <cstdint>
#include <string>

#include "common/cmd_parser.hpp"

class ClientRequest {
 public:
  ClientRequest(CmdParser& cmd);
  static void PrintUsage();
  std::string to_string() const;
  uint16_t port_ = 8888;
  std::string resource_ = "test_resource";
  uint32_t max_responce_length_ = 1024;
};

#endif // SRC_CLIENT_CLIENT_REQUEST_HPP__
