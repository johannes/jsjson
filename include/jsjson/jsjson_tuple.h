#ifndef JSJSON_TUPLE
#define JSJSON_TUPLE

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

namespace {
template <int pos, typename... Ts>
struct Tuple {
  static void addTupleElement(JSONArray &arr, const std::tuple<Ts...> &t) {
    Tuple<pos - 1, Ts...>::addTupleElement(arr, t);
    arr(std::get<pos - 1>(t));
  }
};

template <typename... Ts>
struct Tuple<1, Ts...> {
  static void addTupleElement(JSONArray &arr, const std::tuple<Ts...> &t) {
    arr(std::get<0>(t));
  }
};

template <typename... Ts>
struct Tuple<0, Ts...> {
  static void addTupleElement(JSONArray &, const std::tuple<Ts...> &) {
    // Empty tuple
  }
};
}

template <typename... Ts>
struct Serializer<std::tuple<Ts...>> {
  static std::ostream &serialize(std::ostream &os,
                                 const std::tuple<Ts...> &tuple) {
    JSONArray arr{os};
    Tuple<std::tuple_size<typename std::tuple<Ts...>>::value,
          Ts...>::addTupleElement(arr, tuple);
    return os;
  }
};
}
}

#endif
