//

#include <cmath>
#include "StatsStore.hpp"

void
StatsStore::connectStore()
{
  if (!conn_.connect("dpt", "localhost", user_.c_str(), passwd_.c_str())) {
    throw StatsStoreConnectException();
  }
}

void
StatsStore::updateStatsForDomain(const std::string& domain,
                                 unsigned long latency_ms)
{
  // TODO:
  //    following select and insert/update should be done with transaction
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

void
StatsStore::insertStats(const std::string& domain, unsigned long latency_ms)
{
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

void
StatsStore::updateStats(const std::string& domain, unsigned long latency_ms,
                        unsigned int n, double avg, double sn)
{
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
