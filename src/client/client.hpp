#ifndef SRC_CLIENT_CLIENT_HPP__
#define SRC_CLIENT_CLIENT_HPP__

#include <cstdint>
#include <string>

class Client {
 public:
  Client();
  std::string Send(uint16_t port, const std::string& request,
                   size_t max_responce_size);
  bool IsOpen() const { return socket_fd_ >= 0; }
 private:
  int socket_fd_;
};

#endif // SRC_CLIENT_CLIENT_HPP__
