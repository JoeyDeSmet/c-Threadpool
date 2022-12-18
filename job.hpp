#pragma once

#include <string>
#include <functional>

namespace Threading {

  enum class JobPriority {
    HIGHEST = 0,
    HIGH,
    MEDIUMHIGH,
    MEDIUM,
    MEDIUMLOW,
    LOW,
    LOWEST
  };

  struct Job {
    JobPriority priority;
    std::string name;
    std::function<void()> job;
  };

  struct JobComparator {
    bool operator() (const Job& lhs, const Job& rhs) {
      return static_cast<int>(lhs.priority) > static_cast<int>(rhs.priority);
    }
  };


}
