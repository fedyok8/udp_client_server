// Server side implementation of UDP client-server model
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


std::string handle(const std::string& request) {
  printf("Received : %s\n", request.data());
  return "I am server";
}

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

bool server(uint16_t port, size_t max_request_size) {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd < 0) {
      std::clog << "socket creation failed" << std::endl;
      return false;
  }

  sockaddr_in servaddr;
  const sockaddr* p_servaddr = (sockaddr*)&servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);

  int bind_rv = bind(socket_fd, p_servaddr, sizeof(servaddr));
  if (bind_rv < 0) {
      std::clog << "socket(port= " << port << ") bind failed" << std::endl;
      close(socket_fd);
      return false;
  }

  char* request = new char[max_request_size];

  while (true) {

    sockaddr_in cliaddr;
    sockaddr* p_cliaddr = (sockaddr*)&cliaddr;
    socklen_t cliaddr_len = sizeof(cliaddr);
    memset(&cliaddr, 0, sizeof(cliaddr));

    int n = recvfrom(socket_fd, request, max_request_size, MSG_WAITALL,
                     p_cliaddr, &cliaddr_len);

    std::string response = handle(std::string(request, n));

    sendto(socket_fd, response.data(), response.size(), MSG_CONFIRM,
           p_cliaddr, cliaddr_len);

    std::clog << "Responce sent to addr="
              << addr_to_string(cliaddr.sin_addr.s_addr)
              << " port=" << ntohs(cliaddr.sin_port) << std::endl;

  }

  close(socket_fd);
  delete[] request;

  return true;
}

int main() {
  server(8888, 1024);
  return 0;
}
