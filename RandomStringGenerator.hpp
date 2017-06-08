#ifndef _DPT_RANDOM_STRING_GENERATOR_HPP
#define _DPT_RANDOM_STRING_GENERATOR_HPP

#include <random>
#include <algorithm>

static const char alphanum[] =
  "0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";

/// Random String Generator
class RandomStringGenerator
{
public:
  RandomStringGenerator(std::size_t length)
    : length_(length)
  { }

  inline std::string getString() {
    std::mt19937 gen(rd_());
    std::uniform_int_distribution<int> dist(0, sizeof(alphanum) - 1);
    std::string str;

    for (unsigned int i = 0; i < length_; ++i) {
      str += alphanum[dist(gen)];
    }

    return str;
  }

private:
  /// length of random string
  std::size_t length_;

  /// random device
  std::random_device rd_;
};

#endif // _DPT_RANDOM_STRING_GENERATOR_HPP
