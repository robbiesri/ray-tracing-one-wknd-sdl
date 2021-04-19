#pragma once

#include <deque>
#include <future>
#include <mutex>
#include <vector>

// Sourced from https://riptutorial.com/cplusplus/example/15806/create-a-simple-thread-pool

struct tasks {
  // the mutex, condition variable and deque form a single
  // thread-safe triggered queue of tasks:
  std::mutex m;
  std::condition_variable v;
  // note that a packaged_task<void> can store a packaged_task<R>:
  std::deque<std::packaged_task<void()>> work;

  // this holds futures representing the worker threads being done:
  std::vector<std::future<void>> finished;

  // queue( lambda ) will enqueue the lambda into the tasks for the threads
  // to use.  A future of the type the lambda returns is given to let you get
  // the result out.
  template <class F, class R = std::result_of_t<F &()>>
  std::future<R> queue(F &&f) {
    // wrap the function object into a packaged task, splitting
    // execution from the return value:
    std::packaged_task<R()> p(std::forward<F>(f));

    auto r = p.get_future(); // get the return value before we hand off the task
    {
      std::unique_lock<std::mutex> l(m);
      work.emplace_back(std::move(p)); // store the task<R()> as a task<void()>
    }
    v.notify_one(); // wake a thread to work on the task

    return r; // return the future result of the task
  }

  // start N threads in the thread pool.
  void start(std::size_t N = 1);

  // abort() cancels all non-started tasks, and tells every working thread
  // stop running, and waits for them to finish up.
  void abort();

  // cancel_pending() merely cancels all non-started tasks:
  void cancel_pending();

  // finish enques a "stop the thread" message for every thread, then waits for
  // them:
  void finish();

  ~tasks();

private:
  // the work that a worker thread does:
  void thread_task();
};