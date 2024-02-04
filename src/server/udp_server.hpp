#ifndef SRC_SERVER_UDP_SERVER_HPP__
#define SRC_SERVER_UDP_SERVER_HPP__

#include <atomic>
#include <thread>
#include <vector>
#include <string>

namespace udp {

class Server {
 public:
  Server(uint16_t port, size_t max_request_size = 1024,
         uint8_t workers = 1, size_t max_queue_size = 100);
  virtual ~Server();
  bool is_open() const { return socket_fd_ >= 0; }

 protected:
  virtual std::string Handle(const std::string& request);

 private:
  void ReceiverThread();
  void WorkerThread();

  std::string name_;
  size_t request_length_;

  int socket_fd_;

  size_t queue_size_;
  void* queue_;

  std::atomic_bool stop_receive_thread_;
  std::thread receive_thread_;
  std::vector<std::thread> workers_;
};

} // namespace udp

#endif // SRC_SERVER_UDP_SERVER_HPP__
