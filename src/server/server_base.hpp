#ifndef SRC_SERVER_SERVER_BASE_HPP__
#define SRC_SERVER_SERVER_BASE_HPP__

#include <atomic>
#include <thread>
#include <vector>
#include <string>

class ServerBase {
 public:
  ServerBase(uint16_t port, size_t max_request_size = 1024,
             uint8_t workers = 1, size_t max_queue_size = 100);
  virtual ~ServerBase();
  bool IsOpen() const { return socket_fd_ >= 0; }
  void Start();
  void Stop();

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

  std::atomic_bool stop_threads_;
  std::thread receive_thread_;
  std::vector<std::thread> workers_;
};

#endif // SRC_SERVER_SERVER_BASE_HPP__
