#ifndef HAVE_COMMON_H
#define HAVE_COMMON_H

#include "gtest/gtest.h"
#include "jsjson/jsjson_base.h"

#include <sstream>

namespace test {
template <typename T>
std::string serialize(T&& value) {
  std::stringstream s;
  s << jsjson::serialize(std::forward<T>(value));
  return s.str();
}
}

#endif
