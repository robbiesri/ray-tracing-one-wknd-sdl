#include "TaskProcessor.h"

void tasks::start(std::size_t N) {
  for (std::size_t i = 0; i < N; ++i) {
    // each thread is a std::async running this->thread_task():
    finished.push_back(
        std::async(std::launch::async, [this] { thread_task(); }));
  }
}

void tasks::abort() {
  cancel_pending();
  finish();
}

void tasks::cancel_pending() {
  std::unique_lock<std::mutex> l(m);
  work.clear();
}

void tasks::finish() {
  {
    std::unique_lock<std::mutex> l(m);
    for (auto &&unused : finished) {
      work.push_back({});
    }
  }
  v.notify_all();
  finished.clear();
}

tasks::~tasks() { finish(); }

void tasks::thread_task() {
  while (true) {
    // pop a task off the queue:
    std::packaged_task<void()> f;
    {
      // usual thread-safe queue code:
      std::unique_lock<std::mutex> l(m);
      if (work.empty()) {
        v.wait(l, [&] { return !work.empty(); });
      }
      f = std::move(work.front());
      work.pop_front();
    }
    // if the task is invalid, it means we are asked to abort:
    if (!f.valid())
      return;
    // otherwise, run the task:
    f();
  }
}