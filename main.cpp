#include <iostream>
#include <string>
#include "QueryRunner.hpp"

int
main(int argc, char **argv)
{
  std::size_t count = 10;
  unsigned int interval = 1000;

  // parse command line argument
  if (argc == 1 || argc > 3) {
    std::cout << "Usage: " << argv[0]
              << " [count] [interval]"
              << std::endl;
    return 0;
  }
  if (argc > 1) {
    count = std::stoul(argv[1]);
  }
  if (argc > 2) {
    interval = std::stoul(argv[2]);
  }

  QueryRunner qr(count, interval);
  
  qr.setDomain("x.google.com");
  qr.setDomain("x.youtube.com");
  qr.run();

  return 0;
}
