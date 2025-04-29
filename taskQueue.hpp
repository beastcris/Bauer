#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template<class T>
class TaskQueue {

public:
  TaskQueue() = default;
  ~TaskQueue() = default;

  TaskQueue(const TaskQueue&) = delete;
  TaskQueue(TaskQueue&&) = delete;

  TaskQueue operator=(const TaskQueue&) = delete;
  TaskQueue operator=(TaskQueue&&) = delete;
  
  template<typename U>
  void push(U&& task) {
    std::unique_lock<std::mutex> lck(mtx_);
    q_.push(std::forward<U>(task));
    cv_.notify_one();
  }

  T pop(std::atomic<bool>& working) {
    std::unique_lock<std::mutex> lck(mtx_);
    cv_.wait(lck, [this, &working]() {return !q_.empty() || !working; });
    if (!working && q_.empty()) {
      return {};
    }
    T tmp(std::move(q_.front()));
    q_.pop();
    return tmp;
  };

  bool isEmpty() {
    std::unique_lock<std::mutex> lck(mtx_);
    return q_.empty();
  }
  void awake_all() {
    cv_.notify_all();
  };
private:

  std::queue<T> q_;
  std::condition_variable cv_;
  std::mutex mtx_;
};