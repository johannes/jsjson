#ifndef JSJSON_PAIR
#define JSJSON_PAIR

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

template <typename T1, typename T2>
struct Serializer<std::pair<T1, T2>> {
  static std::ostream &serialize(std::ostream &os,
                                 const std::pair<T1, T2> &pair) {
    JSONArray arr{os};
    arr(pair.first);
    arr(pair.second);
    return os;
  }
};
}
}

#endif
