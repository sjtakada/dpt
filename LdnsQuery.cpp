//

#include "LdnsQuery.hpp"
#include "RandomStringGenerator.hpp"

#define RANDOM_DOMAIN_NAME_LENGTH 8

LdnsQuery::~LdnsQuery()
{
  // free domain
  if (domain_) {
    ldns_rdf_deep_free(domain_);
  }

  // free DNS packet
  if (pkt_) {
    ldns_pkt_free(pkt_);
  }

  // free resolver
  if (res_) {
    ldns_resolver_deep_free(res_);
  }

  // free list of resource records
  if (rr_) {
    ldns_rr_list_deep_free(rr_);
  }
}

std::chrono::milliseconds
LdnsQuery::run()
{
  ldns_status s;
  RandomStringGenerator rsg(RANDOM_DOMAIN_NAME_LENGTH);

  std::string str = rsg.getString() + "." + domain_str_;

  domain_ = ldns_dname_new_frm_str(str.c_str());
  if (!domain_) {
    throw LdnsQueryDomainException();
  }

  s = ldns_resolver_new_frm_file(&res_, NULL);
  if (s != LDNS_STATUS_OK) {
    throw LdnsQueryResolverException();
  }

  // record start timestamp
  auto stime = std::chrono::steady_clock::now();

  pkt_ = ldns_resolver_search(res_, domain_, type_,
                              LDNS_RR_CLASS_IN, LDNS_RD);
  if (!pkt_) {
    throw LdnsQuerySearchException();
  }

  // record end timestamp
  auto etime = std::chrono::steady_clock::now();
  auto diff = etime - stime;
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

  std::cout << str << ": ";
  std::cout << "latency " << ms.count() << " ms" << std::endl;

  return ms;
}
