#ifndef SRC_SERVER_SERVER_PROPERTIES_HPP__
#define SRC_SERVER_SERVER_PROPERTIES_HPP__

#include <string>

#include "common/cmd_parser.hpp"

class ServerProperties {
 public:
  int port_ = 8888;
  int request_length_ = 1024;
  int workers_ = 4;
  int queue_size_ = 10;
  std::string content_filepath_ = "./resource_content.conf";
  ServerProperties(CmdParser& cmd);
  static void PrintUsage();
};

#endif // SRC_SERVER_SERVER_PROPERTIES_HPP__
