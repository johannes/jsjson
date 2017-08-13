#ifndef JSJSON_SMARTPTR_H
#define JSJSON_SMARTPTR_H

#include <memory>

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

/*
  We could generialize this, by detecting something whether the type fulfills
  a rule like this:
     PtrT<InnerT> ptr;
     decltype(*ptr) == InnerT;
  but for now this special rule for shared_ptr is enough.
*/
template <typename T>
struct Serializer<std::shared_ptr<T>> {
  static std::ostream &serialize(std::ostream &os,
                                 const std::shared_ptr<T> ptr) {
    return os << jsjson::serialize(*ptr);
  }
};
}
}

#endif
