#ifndef JSJSON_MAP
#define JSJSON_MAP

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

template <typename KeyT, typename T>
struct Serializer<std::map<KeyT, T>> {
  static std::ostream &serialize(std::ostream &os,
                                 const std::map<KeyT, T> &vec) {
    JSONObject obj(os);
    for (const auto &entry : vec) {
      obj(entry.first, entry.second);
    }
    return os;
  }
};

}
}

#endif
