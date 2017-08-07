#ifndef HAVE_COMMON_H
#define HAVE_COMMON_H

#include "gtest/gtest.h"
#include "jsjson/jsjson.h"

#include <sstream>

template <typename T>
std::string serialize(T value) {
  std::stringstream s;
  s << jsjson::serialize(value);
  return s.str();
}

#endif
