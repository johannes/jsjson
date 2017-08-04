# JSJSON

> A simple, yet powerful JSON serializer for C++

This is a simple JSON serialisation library for C++11. It is easy to
use, yet has powerful features for supporting custom types.

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
  jsjson::serialize(std::cout, object); // Will print something like { "counter": 42, "str": "hello world" }
}
  
```

The [tests directory](tests/) contains more examples in form of gtest tests.
For building the tes suite you need cmake, otherwise only C++11 or newer is
required.

## About the name

Egocentric name choice - JS are my initials and the doubling felt amusing.

## License

Currently using [3-clause-BSD-license](LICENSE). Could eventually be reliensed 
on request.
