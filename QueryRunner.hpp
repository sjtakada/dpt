#ifndef _DPT_QUERY_RUNNER_HPP
#define _DPT_QUERY_RUNNER_HPP

#include <vector>
#include "LdnsQuery.hpp"

/// Query Runner
class QueryRunner
{
public:
  QueryRunner(std::size_t count, unsigned int interval)
    : count_(count), interval_(interval)
  { }

  void setDomain(std::string domain) {
    domains_.push_back(domain);
  }

  void run() {
    for (DomainVector::const_iterator it = domains_.cbegin();
         it != domains_.cend(); ++it) {
      LdnsQuery lq(*it, LDNS_RR_TYPE_A);

      // perform DNS query
      try {
        lq.run();
      } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
      } catch (...) {
        std::cout << "other error " << std::endl;
      }
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
