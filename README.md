# Threadpool c++

This is an implementation of a thread pool in C++ that allows for the execution of a large number of tasks concurrently by a group of worker threads. The tasks are added to a queue and are assigned a priority level, with higher priority tasks being executed before lower priority tasks. The thread pool is useful for improving the performance of a program by allowing it to make use of multiple CPU cores and by minimizing the overhead of creating and destroying individual threads for each task.

# Threading namespace

The Threading namespace contains a few classes and enums related to thread management.

## Enum: JobPriority

This enum represents the priority of a Job in the ThreadPool. It has the following values:

- HIGHEST
- HIGH
- MEDIUMHIGH
- MEDIUM
- MEDIUMLOW
- LOW
- LOWEST

# Struct: Job

The Job struct represents a unit of work that can be executed by a thread. It has the following fields:

- priority: a JobPriority value representing the priority of the job.
- name: a string representing the name of the job.
- job: a function object representing the job to be executed.

# ThreadPool class

The ThreadPool class manages a pool of worker threads and allows you to queue up jobs to be executed by those threads.

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
