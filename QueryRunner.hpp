#ifndef _DPT_QUERY_RUNNER_HPP
#define _DPT_QUERY_RUNNER_HPP

#include <vector>

/// Query Runner
class QueryRunner
{
public:
  /// ctor
  QueryRunner(std::size_t count, unsigned int interval)
    : count_(count), interval_(interval)
  { }

  void setDomain(const std::string& domain) {
    domains_.emplace_back(domain);
  }

  /// Run query runner
  void run();

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
