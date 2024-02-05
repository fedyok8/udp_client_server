#include "client.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Client::Client() {
  socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd_ < 0) {
      std::clog << "ERROR: cannot open socket" << std::endl;
      return;
  }
}

std::string Client::Send(uint16_t port, const std::string& request,
                         size_t max_responce_size) {
  if (!IsOpen()) {
    return {};
  }
  sockaddr_in servaddr;
  sockaddr* p_servaddr = (sockaddr*)&servaddr;
  std::memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  sendto(socket_fd_, request.data(), request.size(),
         MSG_CONFIRM, p_servaddr, sizeof(servaddr));

  std::string buffer;
  buffer.resize(max_responce_size);

  socklen_t len = sizeof(servaddr);

  int n = recvfrom(socket_fd_, buffer.data(), buffer.size(),
              MSG_WAITALL, p_servaddr, &len);

  if (n <= 0) {
    return {};
  }

  return std::string(buffer.data(), n);
}
