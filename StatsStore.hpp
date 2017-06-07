#ifndef _DPT_STATS_HPP
#define _DPT_STATS_HPP

#include <string>
#include <vector>
#include <exception>
#include <cmath>
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
  StatsStore(std::string user, std::string passwd)
    : conn_((_Bool)false), user_(user), passwd_(passwd)
  { }

  ~StatsStore() {
    // should close db connection
  }

  void connectStore() {
    if (!conn_.connect("dpt", "localhost", user_.c_str(), passwd_.c_str())) {
      throw StatsStoreConnectException();
    }
  }

  void updateStatsForDomain(const std::string& domain,
                            unsigned long latency_ms) {
    mysqlpp::Query query = conn_.query();
    query << "SELECT * FROM stats WHERE domain = '" << domain << "'";

    if (mysqlpp::StoreQueryResult res = query.store()) {
      // found record, UPDATE
      if (res.num_rows()) {
        // assuming only one record per domain
        updateStats(domain, latency_ms,
                    res[0]["count"], res[0]["avg"], res[0]["std_deviation"]);
      }
      // not found, INSERT
      else {
        insertStats(domain, latency_ms);
      }
    } else {
      throw StatsStoreSelectException();
    }
  }

private:
  // mysql++ connection
  mysqlpp::Connection conn_;

  // db username
  std::string user_;

  // db password
  std::string passwd_;

  // insert record if it does not exist previously
  void insertStats(const std::string& domain, unsigned long latency_ms) {
    unsigned int n = 1;
    double avg = (double)latency_ms;
    double sn = 0.0;

    mysqlpp::Query query = conn_.query();
    query << "INSERT INTO stats VALUES ('"
          << domain << "',"
          << n << ", "
          << avg << ", "
          << sn << ", "
          << "NOW(), "
          << "NOW() "
          << ")";
    try {
      query.execute();
    } catch (...) {
      throw StatsStoreInsertException();
    }
  }

  // update stats
  void updateStats(const std::string& domain, unsigned long latency_ms,
                   unsigned int n, double avg, double sn) {
    // calculate standard deviation incrementaly
    n++;
    double avg2 = (avg * (n - 1) + latency_ms) / (double)n;
    double sn2 =
      std::sqrt(((n - 1) * std::pow(sn, 2) +
                 (latency_ms - avg) * (latency_ms - avg2)) / n);

    mysqlpp::Query query = conn_.query();
    query << "UPDATE stats SET "
          << "count = " << n << ", "
          << "avg = " << avg2 << ", "
          << "std_deviation = " << sn2 << ", "
          << "last_query = NOW() "
          << "WHERE domain = '" << domain << "'";
    try {
      query.execute();
    } catch (...) {
      throw StatsStoreUpdateException();
    }
 }
};

#endif // _DPT_STATS_HPP
