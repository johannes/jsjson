#include <map>
#include <vector>
#include "common.h"

TEST(vector, integer) {
  std::vector<int> v{};
  EXPECT_EQ(serialize(v), "[  ]");
  v.push_back(1);
  EXPECT_EQ(serialize(v), "[ 1 ]");
  v.push_back(2);
  EXPECT_EQ(serialize(v), "[ 1, 2 ]");
  v.push_back(-3);
  EXPECT_EQ(serialize(v), "[ 1, 2, -3 ]");
}

TEST(vector, string) {
  std::vector<std::string> v{};
  EXPECT_EQ(serialize(v), "[  ]");
  v.emplace_back("hallo");
  EXPECT_EQ(serialize(v), R"([ "hallo" ])");
  v.emplace_back("welt");
  EXPECT_EQ(serialize(v), R"([ "hallo", "welt" ])");
  v.emplace_back("!");
  EXPECT_EQ(serialize(v), R"([ "hallo", "welt", "!" ])");
}

TEST(vector, ofvector) {
  std::vector<std::vector<int>> v{};
  EXPECT_EQ(serialize(v), "[  ]");
  v.emplace_back(std::vector<int>{});
  EXPECT_EQ(serialize(v), R"([ [  ] ])");
  v.emplace_back(std::vector<int>{1});
  EXPECT_EQ(serialize(v), R"([ [  ], [ 1 ] ])");
  v.emplace_back(std::vector<int>{2, 3});
  EXPECT_EQ(serialize(v), R"([ [  ], [ 1 ], [ 2, 3 ] ])");
}

TEST(map, int) {
  std::map<std::string, int> m{};
  EXPECT_EQ(serialize(m), "{  }");
  m.emplace(std::make_pair(std::string{"foo"}, 1));
  EXPECT_EQ(serialize(m), R"({ "foo": 1 })");
  m.emplace(std::make_pair(std::string{"new\nline"}, 2));
  EXPECT_EQ(serialize(m), R"({ "foo": 1, "new\nline": 2 })");
}

TEST(map, intkey) {
  std::map<int, int> m{};
  EXPECT_EQ(serialize(m), "{  }");
  m.emplace(std::make_pair(0, 1));
  EXPECT_EQ(serialize(m), R"({ "0": 1 })");
  m.emplace(std::make_pair(1, 2));
  EXPECT_EQ(serialize(m), R"({ "0": 1, "1": 2 })");

}
