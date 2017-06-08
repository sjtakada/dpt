//

#include <thread>
#include <chrono>
#include "QueryRunner.hpp"
#include "LdnsQuery.hpp"
#include "StatsStore.hpp"

void
QueryRunner::run()
{
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
