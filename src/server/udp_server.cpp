#include "udp_server.hpp"


#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring>

namespace {

std::string addr_to_string(in_addr_t addr) {
  std::string res;
  res.reserve(sizeof(addr) * 4);
  for (int i = 0; i < sizeof(addr); ++i) {
    if (i != 0) res += '.';
    res += std::to_string(addr & 0xFF);
    addr = addr >> 2;
  }
  return res;
}

std::string receive(int socket_fd, std::string& buffer, sockaddr_in& cliaddr, socklen_t& cliaddr_len) {
  cliaddr_len = sizeof(cliaddr);
  memset(&cliaddr, 0, sizeof(cliaddr));

  int n = recvfrom(socket_fd, buffer.data(), buffer.size(), MSG_DONTWAIT,
                   (sockaddr*)&cliaddr, &cliaddr_len);
  if (n <= 0) {
    return {};
  }

  return std::string(buffer.data(), n);
}

void send(int socket_fd, const std::string& response, sockaddr_in& cliaddr, socklen_t& cliaddr_len) {
  sendto(socket_fd, response.data(), response.size(), MSG_CONFIRM,
         (sockaddr*)&cliaddr, cliaddr_len);
}

} // namespace

udp::Server::Server(uint16_t port, size_t max_request_size)
  : port_(port)
  , max_request_size_(max_request_size)
  , socket_fd_(-1)
  , stop_receive_thread_(false)
{
  socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd_ < 0) {
      std::clog << "socket creation failed" << std::endl;
      return;
  }

  sockaddr_in servaddr;
  const sockaddr* p_servaddr = (sockaddr*)&servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);

  int bind_rv = bind(socket_fd_, p_servaddr, sizeof(servaddr));
  if (bind_rv < 0) {
      std::clog << "socket(port = " << port << ") bind failed" << std::endl;
      close(socket_fd_);
      socket_fd_ = -1;
      return;
  }

  receive_thread_ = std::thread(&udp::Server::ReceiveThread, this);
}

udp::Server::~Server() {
  stop_receive_thread_ = true;
  if (receive_thread_.joinable())
    receive_thread_.join();

  if (socket_fd_ >= 0) {
    close(socket_fd_);
  }
  std::clog << "closed socket(port = " << port_ << ")" << std::endl;
}

void udp::Server::ReceiveThread() {
  std::string buffer;
  buffer.resize(max_request_size_);

  std::clog << "listening socket(port = " << port_ << ")" << std::endl;

  while (!stop_receive_thread_) {
    sockaddr_in cliaddr;
    socklen_t cliaddr_len;

    std::string request = receive(socket_fd_, buffer, cliaddr, cliaddr_len);
    if (request.empty()) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }
    std::string response = Handle(request);
    send(socket_fd_, response, cliaddr, cliaddr_len);

    std::clog << "Responce sent to addr="
              << addr_to_string(cliaddr.sin_addr.s_addr)
              << " port=" << ntohs(cliaddr.sin_port) << std::endl;

  }
}

std::string udp::Server::Handle(const std::string& request) {
  std::clog << "Received : " << request << std::endl;
  return "I am server";
}
