#pragma once

#include "taskQueue.hpp"

#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

template <class T>
class ThreadPool {

public:
  ThreadPool(int thread_cnt, TaskQueue<T>& task_q) : thread_count_(thread_cnt), q_(task_q) {
    threads_.reserve(thread_count_);
    for (int i = 0; i < thread_count_; ++i) {
      threads_.emplace_back(&ThreadPool::run, this);
    }
  };

  ~ThreadPool() {
    while (!q_.isEmpty()) {
      q_.awake_all();
    }
    working = false;
    q_.awake_all();
    for (int i = 0; i < thread_count_; ++i) {
      if (threads_[i].joinable()) threads_[i].join();
    }
  };

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

private:

  void run() {

    while (working) {
      T to_do = std::move(q_.pop(working));
      if (to_do) to_do();
    }

  };
  std::mutex mtx_;
  std::vector<std::thread> threads_;
  int thread_count_;
  TaskQueue<T>& q_;
  std::atomic<bool> working = true;
};