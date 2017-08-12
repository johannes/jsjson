#ifndef JSJSON_ITERATOR
#define JSJSON_ITERATOR

/****************************************************************************
 * jsjson                                                                   *
 ****************************************************************************
 * Copyright 2015-2017 Johannes Schlüter                                    *
 ****************************************************************************
 * This source file is subject to the 3-clause BSD license, that is bundled *
 * with this package in the file LICENSE                                    *
 ****************************************************************************/

namespace jsjson {
namespace adapter {

template <typename T>
struct SerializerDetected<T, IS_ITERATOR> {
  static std::ostream &serialize(std::ostream &os, const T &vec) {
    JSONArray arr{os};
    for (const auto &entry : vec) {
      arr(entry);
    }
    return os;
  }
};
}
}

#endif
