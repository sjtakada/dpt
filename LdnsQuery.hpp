#ifndef _DPT_LDNS_QUERY_HPP
#define _DPT_LDNS_QUERY_HPP

#include <stdio.h>
#include <iostream>
#include <ldns/ldns.h>
#include <string>
#include <exception>
#include <chrono>

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
  LdnsQuery(const std::string& domain_str, ldns_rr_type type)
    : domain_str_(domain_str), type_(type), domain_(NULL), res_(NULL),
      pkt_(NULL), rr_(NULL)
  { }

  /// Dtor
  ~LdnsQuery();
  
  /// Run LDNS query
  std::chrono::milliseconds run();

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
