#ifndef SRC_SERVER_SERVER_HPP__
#define SRC_SERVER_SERVER_HPP__

#include <atomic>
#include <thread>
#include <vector>
#include <string>

#include "server_base.hpp"
#include "resource_content.hpp"

class Server : public ServerBase {
 public:
  Server(uint16_t port, size_t max_request_size,
         uint8_t workers, size_t max_queue_size,
         const std::string& content_filepath);
  ~Server() override;

  bool IsResourceEmpty() const { return resource_.IsEmpty(); }

 protected:
  std::string Handle(const std::string& request) override;

 private:
  ResourceContent resource_;
};

#endif // SRC_SERVER_SERVER_HPP__
