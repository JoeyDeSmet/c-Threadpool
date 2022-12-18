# Threadpool c++

simple threadpool class to run jobs on different threads.

## Example use

```cpp
#include <mutex>
#include <iostream>
#include "./threadpool.hpp"

Threading::ThreadPool thread_pool;

std::mutex print_mutex;

#define threadsave_log(...) do {\
  std::unique_lock<std::mutex> lock(print_mutex);\
  std::cout << __VA_ARGS__ << std::endl;\
} while(0)\

int main() {
  uint32_t actual_received_threads = thread_pool.start(3);
  std::cout << "Asked for 3 threads received " << actual_received_threads << std::endl;

  thread_pool.queue_job({
    Threading::JobPriority::MEDIUM,
    "RandomLoop",
    []() {
      for (int i = 0; i < 10; i++) {
        std::cout << "Job running " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  });

  thread_pool.wait_all_running_jobs();

  for (int i = 0; i < 6; i++) {
    thread_pool.queue_job({
      Threading::JobPriority::LOW,
      "Priority",
      [i]() {
        threadsave_log("[START]: Job with low priority started");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threadsave_log("[STOP]: A job with low priority finished");
      }
    });
  }

  for (int i = 0; i < 6; i++) {
    thread_pool.queue_job({
      Threading::JobPriority::HIGH,
      "Priority",
      [i]() {
        threadsave_log("[START]: Job with high priority started");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threadsave_log("[STOP]: A job with high priority finished");
      }
    });
  }

  thread_pool.wait_all_running_jobs();

  thread_pool.stop();
  
  return 0;
}
```

## Compile example

```bash
g++ ./main.cpp ./threadpool.cpp -o threadpoolexample
```
