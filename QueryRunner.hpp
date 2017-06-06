#ifndef _DPT_QUERY_RUNNER_HPP
#define _DPT_QUERY_RUNNER_HPP

#include <vector>
#include <chrono>
#include <thread>
#include "LdnsQuery.hpp"
#include "StatsStore.hpp"

/// Query Runner
class QueryRunner
{
public:
  QueryRunner(std::size_t count, unsigned int interval)
    : count_(count), interval_(interval)
  { }

  void setDomain(std::string domain) {
    domains_.emplace_back(domain);
  }

  void run() {
    StatsStore ss("dpt", "dpt2017");

    try {
      ss.connectStore();
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
      // won't continue
      return;
    }

    for (std::size_t i = 0; i < count_; ++i) {
      for (DomainVector::const_iterator it = domains_.cbegin();
           it != domains_.cend(); ++it) {
        LdnsQuery lq(*it, LDNS_RR_TYPE_A);

        try {
          // perform DNS query
          std::chrono::milliseconds latency_ms = lq.run();

          // update database
          ss.updateStatsForDomain(*it, latency_ms.count());

        } catch (std::exception& e) {
          std::cout << e.what() << std::endl;
        } catch (...) {
          std::cout << "other error " << std::endl;
        }
      }
      // sleep interval
      std::this_thread::sleep_for(std::chrono::milliseconds(interval_));
    }
  }

private:
  /// # of queries performed for each domain
  std::size_t count_;

  /// query interval in millisecond
  unsigned int interval_;

  /// list of domains to query
  typedef std::vector<std::string> DomainVector;
  DomainVector domains_;
};

#endif // _DPT_QUERY_RUNNER_HPP
