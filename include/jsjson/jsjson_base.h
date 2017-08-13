#ifndef JSJSON_BASE_H
#define JSJSON_BASE_H

/****************************************************************************
 * jsjson                                                                   *
 ****************************************************************************
 * Copyright 2015-2017 Johannes Schlüter                                    *
 ****************************************************************************
 * This source file is subject to the 3-clause BSD license, that is bundled *
 * with this package in the file LICENSE                                    *
 ****************************************************************************/

#include <ostream>
#include <type_traits>

namespace jsjson {

inline std::ostream &escape_string(std::ostream &os, const std::string &s) {
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

class JSONArray {
  std::ostream &os;
  bool has_elements = false;

 public:
  JSONArray() = delete;
  JSONArray(std::ostream &os) : os(os) { os << "[ "; }
  ~JSONArray() { os << " ]"; }
  template <typename ValueType>
  void operator()(const ValueType &value);
};

class JSONObject {
  std::ostream &os;
  bool has_elements = false;

 public:
  JSONObject() = delete;
  JSONObject(std::ostream &os) : os(os) { os << "{ "; }
  ~JSONObject() { os << " }"; }
  template <typename KeyType, typename ValueType>
  void operator()(const KeyType &key, const ValueType &value);
};

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
static auto test_end(int)
    -> sfinae_true<decltype(std::end(std::declval<T>()))>;
template <typename T>
static auto test_end(long) -> std::false_type;

template <typename T>
struct is_iterator
    : std::integral_constant<bool, decltype(test_begin<T>(1))::value
                                       &&decltype(test_end<T>(1))::value> {};
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
};

template <typename T>
struct SerializerDetected<T, IS_ARITHMETIC> {
  static std::ostream &serialize(std::ostream &os, T t) { return os << t; }
};

template <typename T>
struct Serializer {
  static std::ostream &serialize(std::ostream &os, const T &t) {
    return SerializerDetected<T, typename type_flag<T>::type>::serialize(os, t);
  }
};

template <typename T>
struct Serializer<const T> : Serializer<T> {};

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
struct Serializer<char *const> : Serializer<std::string> {};
template <>
struct Serializer<const char *> : Serializer<std::string> {};
}

template <typename T>
struct Serializable {
  const T &val;
  Serializable(const T &val) : val(val) {}
};

template <size_t N>
struct Serializable<char[N]> {
  const char *val;
  Serializable(const char *val) : val(val) {}
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Serializable<T> &t) {
  adapter::Serializer<typename std::remove_reference<typename std::remove_const<
      decltype(t.val)>::type>::type>::serialize(os, t.val);
  return os;
}

template <typename T>
auto serialize(const T &t) -> auto {
  return Serializable<T>(t);
}

template <typename ValueType>
void JSONArray::operator()(const ValueType &value) {
  if (has_elements) {
    os << ", ";
  }
  has_elements = true;
  os << jsjson::serialize(value);
}

template <typename T>
struct objectKey {
  static std::ostream &encode(std::ostream &os, T key) {
    // TODO: Maybe use C++17's std::to_chars, or depend on stream
    //       or something else, which works with more types
    return escape_string(os, std::to_string(key));
  }
};

template <>
struct objectKey<std::string> {
  static std::ostream &encode(std::ostream &os, const std::string &key) {
    return escape_string(os, key);
  }
};

template <std::size_t N>
struct objectKey<char[N]> : objectKey<std::string> {};

template <>
struct objectKey<char *> : objectKey<std::string> {};

template <typename KeyType, typename ValueType>
void JSONObject::operator()(const KeyType &key, const ValueType &value) {
  if (has_elements) {
    os << ", ";
  }
  has_elements = true;
  objectKey<KeyType>::encode(os, key);
  os << ": " << jsjson::serialize(value);
}
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

#define ADD_NAMED_PROP(name, ref) o(std::string{#name}, t.ref)

#define ADD_PROP(name) o(std::string{#name}, t.name)

#define ADD_PTR_PROP(name) o(std::string{#name}, *t.name)

#define DYNAMIC_PROP_BEGIN(name, object) \
        o(name, ([](const type &object) {
#define DYNAMIC_PROP_END() \
  } )(t));

#define ADD_GETTER(name, getter)                               \
  DYNAMIC_PROP_BEGIN(name, object) { return object.getter(); } \
  DYNAMIC_PROP_END();

#endif
