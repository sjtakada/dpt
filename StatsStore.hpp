#ifndef _DPT_STATS_HPP
#define _DPT_STATS_HPP

#include <string>
#include <exception>
#include <ldns/common.h>
#include <mysql++.h>

class StatsStoreConnectException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "MySQL Connect Error";
  }
};

class StatsStoreSelectException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "MySQL Select Error";
  }
};

class StatsStoreInsertException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "MySQL Insert Error";
  }
};

class StatsStoreUpdateException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "MySQL Update Error";
  }
};

/// Store statistics through MySQL
class StatsStore
{
public:
  /// ctor
  StatsStore(const std::string& user, const std::string& passwd)
    : conn_((_Bool)false), user_(user), passwd_(passwd)
  { }

  /// dtor
  ~StatsStore() {
    if (conn_) {
      conn_.disconnect();
    }
  }

  /// connect MySQL database
  void connectStore();

  /// update latency stats for domain
  void updateStatsForDomain(const std::string& domain,
                            unsigned long latency_ms);

private:
  // mysql++ connection
  mysqlpp::Connection conn_;

  // db username
  std::string user_;

  // db password
  std::string passwd_;

  // insert record if it does not exist previously
  void insertStats(const std::string& domain, unsigned long latency_ms);

  // update stats
  void updateStats(const std::string& domain, unsigned long latency_ms,
                   unsigned int n, double avg, double sn);
};

#endif // _DPT_STATS_HPP
