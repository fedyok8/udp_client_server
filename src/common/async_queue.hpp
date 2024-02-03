#ifndef SRC_COMMON_ASYNC_QUEUE_HPP__
#define SRC_COMMON_ASYNC_QUEUE_HPP__

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class AsyncQueue {
 public:
  void Enqueue(const T& t);
  bool EnqueueIfSmaller(const T& t, std::size_t max_size);

  bool Dequeue(T& t);
  T DequeueWait();
  template<class Rep, class Period>
  bool DequeueWaitFor(T& t, std::chrono::duration<Rep, Period> wait_for);

  std::size_t Size() const;

 private:
  std::queue<T> queue_;
  mutable std::mutex muxtex_;
  std::condition_variable cv_;
};

template<class T>
void AsyncQueue<T>::Enqueue(const T& t) {
  std::lock_guard<std::mutex> lock(muxtex_);
  queue_.push(t);
  cv_.notify_one();
}

template<class T>
bool AsyncQueue<T>::EnqueueIfSmaller(const T& t, std::size_t max_size) {
  std::lock_guard<std::mutex> lock(muxtex_);
  if (queue_.size() >= max_size) {
    return false;
  }
  queue_.push(t);
  cv_.notify_one();
  return true;
}

template<class T>
bool AsyncQueue<T>::Dequeue(T& t) {
  std::lock_guard<std::mutex> lock(muxtex_);
  if (queue_.empty()) {
    return false;
  }
  t = queue_.front();
  queue_.pop();
  return true;
}

template<class T>
template<class Rep, class Period>
bool AsyncQueue<T>::DequeueWaitFor(T& t, std::chrono::duration<Rep, Period> d) {
  std::unique_lock<std::mutex> lock(muxtex_);

  if (!queue_.empty()) {
    t = queue_.front();
    queue_.pop();
    return true;
  }

  // release lock as long as the wait and reaquire it afterwards.
  bool success = cv_.wait_for(lock, d, [&]{ return !queue_.empty(); } );
  if (!success) {
    return false;
  }
  t = queue_.front();
  queue_.pop();
  return true;
}

template<class T>
T AsyncQueue<T>::DequeueWait() {
  std::unique_lock<std::mutex> lock(muxtex_);

  if (!queue_.empty()) {
    T val = queue_.front();
    queue_.pop();
    return val;
  }

  // release lock as long as the wait and reaquire it afterwards.
  cv_.wait(lock, [&]{ return !queue_.empty(); } );
  T val = queue_.front();
  queue_.pop();
  return val;
}

template<class T>
std::size_t AsyncQueue<T>::Size() const {
  std::lock_guard<std::mutex> lock(muxtex_);
  return queue_.size();
}

#endif //SRC_COMMON_ASYNC_QUEUE_HPP__
