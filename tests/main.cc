#include "common.h"
#include "gtest/gtest.h"

#include <limits>
namespace test {
template <typename T>
std::string serializeMin() {
  return test::serialize(std::numeric_limits<T>::min());
}
template <typename T>
std::string serializeMax() {
  return test::serialize(std::numeric_limits<T>::max());
}
}

TEST(jsjson, integers) {
  EXPECT_EQ(test::serializeMin<int32_t>(), "-2147483648");
  EXPECT_EQ(test::serializeMin<uint32_t>(), "0");
  EXPECT_EQ(test::serializeMin<int64_t>(), "-9223372036854775808");
  EXPECT_EQ(test::serializeMin<uint64_t>(), "0");

  EXPECT_EQ(test::serializeMax<int32_t>(), "2147483647");
  EXPECT_EQ(test::serializeMax<uint32_t>(), "4294967295");
  EXPECT_EQ(test::serializeMax<int64_t>(), "9223372036854775807");
  EXPECT_EQ(test::serializeMax<uint64_t>(), "18446744073709551615");
}

TEST(jsjson, floats) {
  EXPECT_EQ(test::serialize<float>(0), "0");
  EXPECT_EQ(test::serialize<float>(.1), "0.1");
  EXPECT_EQ(test::serialize<float>(-.1), "-0.1");
  EXPECT_EQ(test::serialize<float>(0.000001), "1e-06");
  EXPECT_EQ(test::serialize<float>(10000000), "1e+07");

  EXPECT_EQ(test::serialize<double>(0), "0");
  EXPECT_EQ(test::serialize<double>(.1), "0.1");
  EXPECT_EQ(test::serialize<double>(-.1), "-0.1");
  EXPECT_EQ(test::serialize<double>(0.000001), "1e-06");
  EXPECT_EQ(test::serialize<double>(10000000), "1e+07");
}

TEST(jsjson, strings) {
  using namespace std::string_literals;

  EXPECT_EQ(test::serialize(""), "\"\"");
  EXPECT_EQ(test::serialize(const_cast<char *>("")), "\"\"");
  EXPECT_EQ(test::serialize(""s), "\"\"");

  EXPECT_EQ(test::serialize("some string"), R"("some string")");
  EXPECT_EQ(test::serialize("some\\string"), R"("some\\string")");
  EXPECT_EQ(test::serialize("some\bstring"), R"("some\bstring")");
  EXPECT_EQ(test::serialize("some\fstring"), R"("some\fstring")");
  EXPECT_EQ(test::serialize("some\nstring"), R"("some\nstring")");
  EXPECT_EQ(test::serialize("some\rstring"), R"("some\rstring")");
  EXPECT_EQ(test::serialize("some\tstring"), R"("some\tstring")");

  EXPECT_EQ(test::serialize("some string"s), R"("some string")");
  EXPECT_EQ(test::serialize("some\\string"s), R"("some\\string")");
  EXPECT_EQ(test::serialize("some\bstring"s), R"("some\bstring")");
  EXPECT_EQ(test::serialize("some\fstring"s), R"("some\fstring")");
  EXPECT_EQ(test::serialize("some\nstring"s), R"("some\nstring")");
  EXPECT_EQ(test::serialize("some\rstring"s), R"("some\rstring")");
  EXPECT_EQ(test::serialize("some\tstring"s), R"("some\tstring")");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}
