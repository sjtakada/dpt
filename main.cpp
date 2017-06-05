#include <iostream>
#include <string>
#include "LdnsQuery.hpp"

int
main(int argc, char **argv)
{
  // command line argument
  LdnsQuery lq(argv[1], LDNS_RR_TYPE_A);

  try {
    lq.run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "other error " << std::endl;
  }

  return 0;
}
