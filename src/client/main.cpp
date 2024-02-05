// Client side implementation of UDP client-server model
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void handle(const std::string& response) {
  printf("Received : %s\n", response.data());
}

bool client(uint16_t port, size_t max_request_size) {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd < 0) {
    std::clog << "socket creation failed" << std::endl;
    return false;
  }

  sockaddr_in servaddr;
  sockaddr* p_servaddr = (sockaddr*)&servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  const char *request = "I am client";

  sendto(socket_fd, request, strlen(request), MSG_CONFIRM,
         p_servaddr, sizeof(servaddr));

  std::cout << "Request sent" << std::endl;

  char* buffer = new char[max_request_size];
  socklen_t len = sizeof(servaddr);

  int n = recvfrom(socket_fd, buffer, max_request_size,
              MSG_WAITALL, p_servaddr, &len);

  handle(std::string(buffer, n));

  close(socket_fd);
  delete[] buffer;

  return true;
}

int main() {
  client(8888, 1024);
}
