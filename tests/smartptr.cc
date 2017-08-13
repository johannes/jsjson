#include <memory>
#include "common.h"
#include "gtest/gtest.h"
#include "jsjson/jsjson.h"

TEST(smartptr, shared_ptr) {
  auto shared_int = std::make_shared<int>(42);
  EXPECT_EQ(test::serialize(shared_int), "42");

  std::vector<std::shared_ptr<int>> vector_of_shared{shared_int};
  EXPECT_EQ(test::serialize(vector_of_shared), "[ 42 ]");
}
