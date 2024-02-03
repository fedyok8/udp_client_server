#ifndef SRC_SERVER_UDP_SERVER_HPP__
#define SRC_SERVER_UDP_SERVER_HPP__

#include <atomic>
#include <thread>

namespace udp {

class Server {
 public:
  Server(uint16_t port, size_t max_request_size = 1024);
  virtual ~Server();
  bool is_open() const { return socket_fd_ >= 0; }

 protected:
  virtual std::string Handle(const std::string& request);

 private:
  void ReceiveThread();

  uint16_t port_;
  size_t max_request_size_;

  int socket_fd_;

  std::atomic_bool stop_receive_thread_;
  std::thread receive_thread_;
};

} // namespace udp

#endif // SRC_SERVER_UDP_SERVER_HPP__
