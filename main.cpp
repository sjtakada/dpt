#include <iostream>
#include <string>
#include "QueryRunner.hpp"

int
main(int argc, char **argv)
{
  std::size_t count = 10;
  unsigned int interval_ms = 1000;	// milliseconds

  // parse command line argument
  if (argc == 1 || argc > 3) {
    std::cout << "Usage: " << argv[0]
              << " [count] [interval(ms)]"
              << std::endl;
    return 0;
  }
  if (argc > 1) {
    count = std::stoul(argv[1]);
  }
  if (argc > 2) {
    interval_ms = std::stoul(argv[2]);
  }

  QueryRunner qr(count, interval_ms);
  
  // TODO: read domain list from file or somewhere
  qr.setDomain("google.com");
  qr.setDomain("youtube.com");
  qr.setDomain("facebook.com");
  qr.setDomain("baidu.com");
  qr.setDomain("wikipedia.org");
  qr.setDomain("yahoo.com");
  qr.setDomain("google.co.in");
  qr.setDomain("reddit.com");
  qr.setDomain("qq.com");
  qr.setDomain("taobao.com");

  qr.run();

  return 0;
}
