#include "server_base.hpp"

#include <iostream>
#include <sstream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "common/async_queue.hpp"

namespace {

uint64_t NextWorkerNum() {
  static std::atomic_uint64_t next_worker_num = 0;
  return next_worker_num++;
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

std::string Endpoint(const sockaddr_in& cliaddr) {
  return (std::stringstream()
          << addr_to_string(cliaddr.sin_addr.s_addr)
          << ":"
          << ntohs(cliaddr.sin_port)
          ).str();
}

class RequestInfo {
 public:
  RequestInfo() { Reset(); }
  void Reset();

  sockaddr_in cliaddr;
  socklen_t cliaddr_len;
  std::string request;
};

void RequestInfo::Reset() {
  cliaddr_len = sizeof(cliaddr);
  std::memset(&cliaddr, 0, cliaddr_len);
  request.clear();
}

using Queue = AsyncQueue<RequestInfo>;

bool receive(int socket_fd, std::string& buffer, RequestInfo& request) {
  int n = recvfrom(socket_fd,
                   buffer.data(), buffer.size(),
                   MSG_DONTWAIT,
                   (sockaddr*)&request.cliaddr, &request.cliaddr_len);
  if (n <= 0) {
    return false;
  }
  request.request = std::string(buffer.data(), n);

  return true;
}

void send(int socket_fd, const RequestInfo& request,
          const std::string& response) {
  sendto(socket_fd, response.data(), response.size(), MSG_CONFIRM,
         (sockaddr*)&request.cliaddr, request.cliaddr_len);
}

} // namespace

ServerBase::ServerBase(uint16_t port, size_t request_length,
                       uint8_t workers, size_t queue_size)
  : request_length_(request_length)
  , socket_fd_(-1)
  , queue_size_(queue_size)
  , queue_(nullptr)
  , stop_threads_(true)
{
  sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);

  name_ = std::string() + "Server(" + Endpoint(servaddr) + ")";

  std::clog << name_
            << " initializing with parameters"
            << " request_length = " << request_length
            << " workers = " << (int) workers
            << " queue_size = " << queue_size
            << std::endl;

  socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd_ < 0) {
      std::clog << "ERROR: " << name_ << " cannot open socket" << std::endl;
      return;
  }

  int bind_rv = bind(socket_fd_, (sockaddr*)&servaddr, sizeof(servaddr));
  if (bind_rv < 0) {
      std::clog << "ERROR: " << name_ << " cannot bind socket" << std::endl;
      close(socket_fd_);
      socket_fd_ = -1;
      return;
  }

  queue_ = new Queue;

  workers_.resize(workers);
}

ServerBase::~ServerBase() {
  Stop();
  Queue* queue = (Queue*)queue_;
  delete queue;
  if (socket_fd_ >= 0) {
    close(socket_fd_);
  }
}

void ServerBase::Start() {
  stop_threads_ = false;

  receive_thread_ = std::thread(&ServerBase::ReceiverThread, this);

  for (int i = 0; i < workers_.size(); ++i) {
    workers_[i] = std::thread(&ServerBase::WorkerThread, this);
  }
  std::clog << name_ << " started" << std::endl;
}

void ServerBase::Stop() {
  stop_threads_ = true;

  if (receive_thread_.joinable())
    receive_thread_.join();

  for (int i = 0; i < workers_.size(); ++i)
    if(workers_[i].joinable())
      workers_[i].join();
  std::clog << name_ << " stopped" << std::endl;
}

void ServerBase::ReceiverThread() {
  std::clog << name_ << " receiver started" << std::endl;
  std::string buffer;
  buffer.resize(request_length_);

  RequestInfo request;
  Queue& queue = *((Queue*)queue_);

  auto last_success = std::chrono::steady_clock::now();
  std::chrono::milliseconds delay(500);
  std::chrono::milliseconds sleep(100);

  while (!stop_threads_) {
    sockaddr_in cliaddr;
    socklen_t cliaddr_len;

    request.Reset();
    bool received = receive(socket_fd_, buffer, request);

    if (!received) {
      if (std::chrono::steady_clock::now() - last_success > delay)
      std::this_thread::sleep_for(sleep);
      continue;
    }

    bool added = queue.EnqueueIfSmaller(request, queue_size_);

    std::clog << (std::stringstream()
                  << ((added)? name_ :
                               std::string("WARNING: ") + name_ + "DROPPED")
                  << " Request from "
                  << Endpoint(request.cliaddr)
                  << " size="
                  << request.request.size()
                  << "\n"
                  ).str() << std::flush;

    last_success = std::chrono::steady_clock::now();
  }
  std::clog << name_ << " receiver stopped" << std::endl;
}

void ServerBase::WorkerThread() {
  std::string worker_name = name_ + " worker("
                            + std::to_string(NextWorkerNum()) + ")";
  std::clog << (worker_name + " started\n") << std::flush;

  RequestInfo request;
  Queue& queue = *((Queue*)queue_);

  std::chrono::milliseconds wait(100);

  while (!stop_threads_) {
    bool received = queue.DequeueWaitFor(request, wait);
    if (!received) {
      continue;
    }
    std::string response = Handle(request.request);
    send(socket_fd_, request, response);

    std::clog << (std::stringstream()
                  << worker_name
                  << ": Responce sent to "
                  << Endpoint(request.cliaddr)
                  << " size="
                  << response.size()
                  << "\n"
                  ).str() << std::flush;
  }
  std::clog << (worker_name + " stopped\n") << std::flush;
}

std::string ServerBase::Handle(const std::string& request) {
  return "I am server";
}
