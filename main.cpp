#include <iostream>
#include "./threadpool.hpp"

Threading::ThreadPool thread_pool;

int main() {
  uint32_t actual_received_threads = thread_pool.start(6);
  std::cout << "Asked for 6 threads received " << actual_received_threads << std::endl;

  thread_pool.queue_job([]() {
    for (int i = 0; i < 10; i++) {
      std::cout << "Job running " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  });

  thread_pool.wait_all_running_jobs();

  for (int i = 0; i < 6; i++) {

    thread_pool.queue_job([i]() {
      std::cout << "Job " << i << " running" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout << "Job " << i << " finished" << std::endl;
    });

  }

  thread_pool.wait_all_running_jobs();

  thread_pool.stop();
  
  return 0;
}
