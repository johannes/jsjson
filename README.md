# JSJSON

[![Build Status](https://travis-ci.org/johannes/jsjson.svg?branch=master)](https://travis-ci.org/johannes/jsjson)

> A simple, yet powerful JSON serializer for C++11/C++14

This is a simple JSON serialisation library for C++11/C++14. It is easy to
use, yet has powerful features for supporting custom types.

This lis a header-only library and can therefore be included easily into
arbitrary projects and build-systems.

Support status: This is a small library I use in some toy projects with
limited requirements. There can be rough edges.

## Example:

```c++
#include <jsjson/jsjson.h>
#include <iostream>

// Your custom data type
struct MyType {
  int counter;
  std:string str;
}

// An adaptor, as MyType only has public properties this is quite straight forward:
JSON_ADAPT_OBJECT_BEGIN(MyType) {
  ADD_PROP(counter);
  ADD_PROP(str);
} JSON_ADAPT_OBJECT_END();

int main() {
  MyType object{42, "hello world"};
  std::cout << jsjson::serialize(object); // Will print something like { "counter": 42, "str": "hello world" }
}
  
```

The [tests directory](tests/) contains more examples in form of gtest tests.
For building the tes suite you need cmake, otherwise only C++11 or newer is
required.

## Future plans

The future plan is to replace the adaptor macros with C++20 reflection and 
compile-time programming. A prototype to verify the idea exists. Till that's
available one has to live with the adaptor macros.

## About the name

Egocentric name choice - JS are my initials and the doubling felt amusing.

## License

Currently using [3-clause-BSD-license](LICENSE). Could eventually be reliensed 
on request.
