#ifndef _DPT_LDNS_QUERY_HPP
#define _DPT_LDNS_QUERY_HPP

#include <stdio.h>
#include <ldns/ldns.h>
#include <string>
#include <exception>
#include <chrono>
#include "RandomStringGenerator.hpp"

#define RANDOM_DOMAIN_NAME_LENGTH 8

class LdnsQueryDomainException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "LDNS Create Domain Error";
  }
};

class LdnsQueryResolverException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "LDNS Create Resolver Error";
  }
};

class LdnsQuerySearchException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "LDNS Resolver Search Error";
  }
};

class LdnsQueryRRListException: public std::exception
{
public:
  virtual const char *what() const throw() {
    return "LDNS Resource Record List Error";
  }
};

/// LDNS query wrapper class
class LdnsQuery
{
public:
  /// Ctor
  LdnsQuery(std::string domain_str, ldns_rr_type type)
    : domain_str_(domain_str), type_(type), domain_(NULL), res_(NULL),
      pkt_(NULL), rr_(NULL)
  { }

  /// Dtor
  ~LdnsQuery() {
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
  
  /// Run LDNS query
  int run() {
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
  }

private:
  /// domain string
  std::string domain_str_;

  /// resource record type
  ldns_rr_type type_;

  /// LDNS domain
  ldns_rdf *domain_;

  /// LDNS Resolver
  ldns_resolver *res_;

  /// LDNS Packet
  ldns_pkt *pkt_;

  /// LDNS Resource Record List
  ldns_rr_list *rr_;
};

#endif // _DPT_LDNS_QUERY_HPP
