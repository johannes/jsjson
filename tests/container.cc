#include <map>
#include <array>
#include <list>
#include <vector>
#include <tuple>
#include "common.h"
#include "jsjson/jsjson.h"

TEST(itertor,vectorOfInteger) {
  std::vector<int> v{};
  EXPECT_EQ(test::serialize(v), "[  ]");
  v.push_back(1);
  EXPECT_EQ(test::serialize(v), "[ 1 ]");
  v.push_back(2);
  EXPECT_EQ(test::serialize(v), "[ 1, 2 ]");
  v.push_back(-3);
  EXPECT_EQ(test::serialize(v), "[ 1, 2, -3 ]");
}

TEST(iterator, vectorOfString) {
  std::vector<std::string> v{};
  EXPECT_EQ(test::serialize(v), "[  ]");
  v.emplace_back("hallo");
  EXPECT_EQ(test::serialize(v), R"([ "hallo" ])");
  v.emplace_back("welt");
  EXPECT_EQ(test::serialize(v), R"([ "hallo", "welt" ])");
  v.emplace_back("!");
  EXPECT_EQ(test::serialize(v), R"([ "hallo", "welt", "!" ])");
}

TEST(iterator, vectorOfVector) {
  std::vector<std::vector<int>> v{};
  EXPECT_EQ(test::serialize(v), "[  ]");
  v.emplace_back(std::vector<int>{});
  EXPECT_EQ(test::serialize(v), R"([ [  ] ])");
  v.emplace_back(std::vector<int>{1});
  EXPECT_EQ(test::serialize(v), R"([ [  ], [ 1 ] ])");
  v.emplace_back(std::vector<int>{2, 3});
  EXPECT_EQ(test::serialize(v), R"([ [  ], [ 1 ], [ 2, 3 ] ])");
}

TEST(iterator, stdarray) {
  std::array<int, 3> a{ {1, 2, 3} };
  EXPECT_EQ(test::serialize(a), "[ 1, 2, 3 ]");
}

TEST(iterator, list) {
  std::list<int> l1 {  };
  EXPECT_EQ(test::serialize(l1), "[  ]");
  std::list<int> l2 = { 1, 2 };
  EXPECT_EQ(test::serialize(l2), "[ 1, 2 ]");
  std::list<int> l3 = { 1, 2, 3 };
  EXPECT_EQ(test::serialize(l3), "[ 1, 2, 3 ]");
}


TEST(map, int) {
  std::map<std::string, int> m{};
  EXPECT_EQ(test::serialize(m), "{  }");
  m.emplace(std::make_pair(std::string{"foo"}, 1));
  EXPECT_EQ(test::serialize(m), R"({ "foo": 1 })");
  m.emplace(std::make_pair(std::string{"new\nline"}, 2));
  EXPECT_EQ(test::serialize(m), R"({ "foo": 1, "new\nline": 2 })");
}

TEST(map, intkey) {
  std::map<int, int> m{};
  EXPECT_EQ(test::serialize(m), "{  }");
  m.emplace(std::make_pair(0, 1));
  EXPECT_EQ(test::serialize(m), R"({ "0": 1 })");
  m.emplace(std::make_pair(1, 2));
  EXPECT_EQ(test::serialize(m), R"({ "0": 1, "1": 2 })");

}

TEST(pair, intsring) {
  using namespace std::string_literals;
  EXPECT_EQ(test::serialize(std::make_pair(1, "one"s)), R"([ 1, "one" ])");
  EXPECT_EQ(test::serialize(std::make_pair(2, "two")), R"([ 2, "two" ])");
}

TEST(tuple, tuple) {
  using namespace std::string_literals;
  EXPECT_EQ(test::serialize(std::make_tuple()), R"([  ])");
  EXPECT_EQ(test::serialize(std::make_tuple("A")), R"([ "A" ])");
  EXPECT_EQ(test::serialize(std::make_tuple("A", "B"s, 1337)), R"([ "A", "B", 1337 ])");
}
