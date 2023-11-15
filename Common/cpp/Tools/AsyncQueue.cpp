#include "AsyncQueue.h"

#include <utility>

namespace reanimated {

AsyncQueue::AsyncQueue(const std::string &name) : name_(name) {
  thread_ = std::thread(&AsyncQueue::runLoop, this);
#ifdef ANDROID
  pthread_setname_np(thread_.native_handle(), name_.c_str());
#endif
}

AsyncQueue::~AsyncQueue() {
  running_ = false;
  cv_.notify_all();
  thread_.join();
}

void AsyncQueue::push(std::function<void()> &&job) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.emplace(job);
  }
  cv_.notify_one();
}

void AsyncQueue::runLoop() {
#if __APPLE__
  pthread_setname_np(name_.c_str());
#endif
  while (running_) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty() || !running_; });
    if (!running_) {
      return;
    }
    if (!queue_.empty()) {
      auto job = std::move(queue_.front());
      queue_.pop();
      lock.unlock();
      job();
    }
  }
}

} // namespace reanimated
