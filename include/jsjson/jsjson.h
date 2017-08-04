#ifndef JSJSON_H
#define JSJSON_H

/****************************************************************************
 * jsjson                                                                   *
 ****************************************************************************
 * Copyright 2015-2017 Johannes Schlüter                                    *
 ****************************************************************************
 * This source file is subject to the 3-clause BSD license, that is bundled *
 * with this package in the file LICENSE                                    *
 ****************************************************************************/

#include <map>
#include <ostream>
#include <type_traits>
#include <vector>

namespace jsjson {
class JSONObject;

inline std::ostream& escape_string(std::ostream &os, const std::string s) {
  // TODO: I'm unreadable!
  os << "\"";
  for (auto c : s) {
    if (c == '\n') {
      os << "\\n";
    } else if (c == '\r') {
      os << "\\r";
    } else if (c == '\b') {
      os << "\\b";
    } else if (c == '\f') {
      os << "\\f";
    } else if (c == '\t') {
      os << "\\t";
    } else {
      if (c == '\\' || c == '"') {
        os << '\\';
      }
      os << c;
    }
  }
  os << '"';

  return os;
}

namespace adapter {

namespace {
namespace {
template <typename>
struct sfinae_true : std::true_type {};

template <typename T>
static auto test_begin(int)
    -> sfinae_true<decltype(std::begin(std::declval<T>()))>;
template <typename T>
static auto test_begin(long) -> std::false_type;

template <typename T>
static auto test_end(int) -> sfinae_true<decltype(std::end(std::declval<T>()))>;
template <typename T>
static auto test_end(long) -> std::false_type;

template <typename T>
struct is_iterator
    : std::integral_constant<bool, decltype(test_begin<T>(1))::value &&decltype(
                                       test_end<T>(1))::value> {};
}

struct IS_UNHANDLED {};
struct IS_ARITHMETIC {};
struct IS_ITERATOR {};

template <bool arithmetic, bool iterator>
struct detect {
  using type = IS_UNHANDLED;
};

template <>
struct detect<true, false> {
  using type = IS_ARITHMETIC;
};

template <>
struct detect<false, true> {
  using type = IS_ITERATOR;
};

template <typename T>
struct type_flag {
  using type = typename detect<std::is_arithmetic<T>::value,
                               is_iterator<T>::value>::type;
};
}

template <typename T, typename TypeFlag>
struct SerializerDetected {
 private:
  class NoMatchingSpecialisation {};

 public:
  static std::ostream &serialize(std::ostream &os, NoMatchingSpecialisation) {
    return os;
  }
  static NoMatchingSpecialisation unserialize(std::istream &is) { return is; }
};

template <typename T>
struct SerializerDetected<T, IS_ARITHMETIC> {
  static std::ostream &serialize(std::ostream &os, T t) { return os << t; }
  static T unserialize(std::istream &is) { return 0; }
};

template <typename T>
struct Serializer {
  static std::ostream &serialize(std::ostream &os, T t) {
    return SerializerDetected<T, typename type_flag<T>::type>::serialize(os, t);
  }
  static T unserialize(std::istream &is) {
    return SerializerDetected<T, typename type_flag<T>::type>::unserialize(is);
  }
};

template <>
struct Serializer<std::string> {
  static std::ostream &serialize(std::ostream &os, const std::string &s) {
    escape_string(os, s);
    return os;
  }
};

template <>
struct Serializer<char *> : Serializer<std::string> {};
template <>
struct Serializer<const char *> : Serializer<std::string> {};

template <typename T>
struct SerializerDetected<T, IS_ITERATOR> {
  static std::ostream &serialize(std::ostream &os, const T &vec) {
    os << "[ ";
    bool begin = true;
    for (const auto &entry : vec) {
      if (begin) {
        begin = false;
      } else {
        os << ", ";
      }
      Serializer<typename std::remove_const<typename std::remove_reference<
          decltype(entry)>::type>::type>::serialize(os, entry);
    }
    os << " ]";
    return os;
  }
};

template <typename T>
struct Serializer<std::map<std::string, T>> {
  static std::ostream &serialize(std::ostream &os,
                                 const std::map<std::string, T> &vec) {
    bool begin = true;
    os << "{ ";
    for (const auto &entry : vec) {
      if (begin) {
        begin = false;
      } else {
        os << ", ";
      }
      // TODO: This should use the JSONObject ...
      escape_string(os, entry.first);
      os << ": ";
      Serializer<T>::serialize(os, entry.second);
    }
    os << " }";
    return os;
  }

  static std::vector<T> unserialize(const std::istream &json) {
    std::vector<T> retval;
    return retval;
  }
};
}

template <typename T>
std::ostream &serialize(std::ostream &os, T t) {
  adapter::Serializer<T>::serialize(os, t);
  return os;
}

template <typename T>
T unserialize(std::ostream &os) {
  adapter::Serializer<T>::unserialize(os);
  return os;
}

class JSONObject {
  std::ostream &os;
  bool has_elements = false;

 public:
  JSONObject() = delete;
  JSONObject(std::ostream &os)
      : os(os) {
    os << "{ ";
  }
  ~JSONObject() { os << " }"; }
  template <typename ValueType>
  void operator()(const std::string key, const ValueType &value) {
    if (has_elements) {
      os << ", ";
    }
    has_elements = true;
    escape_string(os, key);
    os << ": ";
    serialize(os, value);
  }
  template <typename ValueType>
  void operator()(long key, const ValueType &value) {
    if (has_elements) {
      os << ", ";
    }
    has_elements = true;
    os << key << ": ";
    serialize(os, value);
  }
};
}

#define JSON_ADAPT_OBJECT_BEGIN(Type)                                 \
  namespace jsjson {                                                  \
  namespace adapter {                                                 \
  template <>                                                         \
  struct Serializer<Type> {                                           \
    using type = Type;                                                \
    static std::ostream &serialize(std::ostream &os, const Type &t) { \
      jsjson::JSONObject o(os);

#define JSON_ADAPT_OBJECT_END() \
  return os;                    \
  }                             \
  }                             \
  ;                             \
  }                             \
  }

#define JSON_FRIEND_ADAPTER(myname) \
  friend class jsjson::adapter::Serializer<myname>;

#define ADD_NAMED_PROP(name, ref) o(#name, t.ref)

#define ADD_PROP(name) o(#name, t.name)

#define ADD_PTR_PROP(name) o(#name, *t.name)

#define DYNAMIC_PROP_BEGIN(name, object) \
        o(name, ([](const type &object) {
#define DYNAMIC_PROP_END() \
  } )(t));

#define ADD_GETTER(name, getter)                               \
  DYNAMIC_PROP_BEGIN(name, object) { return object.getter(); } \
  DYNAMIC_PROP_END();

#endif
