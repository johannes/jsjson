#include "common.h"
#include "gtest/gtest.h"

#include <limits>

template <typename T>
std::string serializeMin() {
  return serialize(std::numeric_limits<T>::min());
}
template <typename T>
std::string serializeMax() {
  return serialize(std::numeric_limits<T>::max());
}

TEST(jsjson, integers) {
  EXPECT_EQ(serializeMin<int32_t>(), "-2147483648");
  EXPECT_EQ(serializeMin<uint32_t>(), "0");
  EXPECT_EQ(serializeMin<int64_t>(), "-9223372036854775808");
  EXPECT_EQ(serializeMin<uint64_t>(), "0");

  EXPECT_EQ(serializeMax<int32_t>(), "2147483647");
  EXPECT_EQ(serializeMax<uint32_t>(), "4294967295");
  EXPECT_EQ(serializeMax<int64_t>(), "9223372036854775807");
  EXPECT_EQ(serializeMax<uint64_t>(), "18446744073709551615");
}

TEST(jsjson, floats) {
  EXPECT_EQ(serialize<float>(0), "0");
  EXPECT_EQ(serialize<float>(.1), "0.1");
  EXPECT_EQ(serialize<float>(-.1), "-0.1");
  EXPECT_EQ(serialize<float>(0.000001), "1e-06");
  EXPECT_EQ(serialize<float>(10000000), "1e+07");

  EXPECT_EQ(serialize<double>(0), "0");
  EXPECT_EQ(serialize<double>(.1), "0.1");
  EXPECT_EQ(serialize<double>(-.1), "-0.1");
  EXPECT_EQ(serialize<double>(0.000001), "1e-06");
  EXPECT_EQ(serialize<double>(10000000), "1e+07");
}

TEST(jsjson, strings) {
  using namespace std::string_literals;

  EXPECT_EQ(serialize(""), "\"\"");
  EXPECT_EQ(serialize(const_cast<char *>("")), "\"\"");
  EXPECT_EQ(serialize(""s), "\"\"");

  EXPECT_EQ(serialize("some string"), R"("some string")");
  EXPECT_EQ(serialize("some\\string"), R"("some\\string")");
  EXPECT_EQ(serialize("some\bstring"), R"("some\bstring")");
  EXPECT_EQ(serialize("some\fstring"), R"("some\fstring")");
  EXPECT_EQ(serialize("some\nstring"), R"("some\nstring")");
  EXPECT_EQ(serialize("some\rstring"), R"("some\rstring")");
  EXPECT_EQ(serialize("some\tstring"), R"("some\tstring")");

  EXPECT_EQ(serialize("some string"s), R"("some string")");
  EXPECT_EQ(serialize("some\\string"s), R"("some\\string")");
  EXPECT_EQ(serialize("some\bstring"s), R"("some\bstring")");
  EXPECT_EQ(serialize("some\fstring"s), R"("some\fstring")");
  EXPECT_EQ(serialize("some\nstring"s), R"("some\nstring")");
  EXPECT_EQ(serialize("some\rstring"s), R"("some\rstring")");
  EXPECT_EQ(serialize("some\tstring"s), R"("some\tstring")");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}
