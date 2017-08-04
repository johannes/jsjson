#include "common.h"
#include "gtest/gtest.h"

// custom data types ...
// TODO: Split this into multiple smaller chunks instead of a big thing
struct Example {
  int ex1;
  long ex2;
};

struct Sample {
  const std::string foo;
  int bar;
  double dop;
  std::vector<Example> v;
  const Example &exref;
  Example *exptr;
  std::map<std::string, long> map;
  std::map<int, int> intmap;
  std::map<std::string, long> get_map() const { return map; }
};

JSON_ADAPT_OBJECT_BEGIN(Example) {
  ADD_PROP(ex1);
  ADD_PROP(ex2);
}
JSON_ADAPT_OBJECT_END();

JSON_ADAPT_OBJECT_BEGIN(Sample) {
  ADD_PROP(foo);
  ADD_PROP(bar);
  ADD_PROP(dop);
  ADD_PROP(v);
  ADD_PROP(exref);
  ADD_PTR_PROP(exptr);
  ADD_PROP(map);
  DYNAMIC_PROP_BEGIN("map 2", sample) {
    return sample.get_map();
  } DYNAMIC_PROP_END();
  ADD_GETTER("map 3", get_map);
}
JSON_ADAPT_OBJECT_END();

TEST(struct_, simple) {
  Example e{1, 2};
  EXPECT_EQ(serialize(e), R"({ "ex1": 1, "ex2": 2 })");
}

TEST(sttruct_, complex_) {
  Example e{1, 2};
  Sample s = {"foo\"b\\ar",
              42,
              3.141,
              {{1024, 2048}, {4096, 8192}},
	      e,
	      &e,
              {{"key1", 23}, {"key2", 42}},
              {{0, 1}, {1, 2}}};

  EXPECT_EQ(serialize(s), R"({ "foo": "foo\"b\\ar", "bar": 42, "dop": 3.141, "v": [ { "ex1": 1024, "ex2": 2048 }, { "ex1": 4096, "ex2": 8192 } ], "exref": { "ex1": 1, "ex2": 2 }, "exptr": { "ex1": 1, "ex2": 2 }, "map": { "key1": 23, "key2": 42 }, "map 2": { "key1": 23, "key2": 42 }, "map 3": { "key1": 23, "key2": 42 } })");
}
