file(GLOB SRCS *.cc)


enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

ADD_EXECUTABLE(testjsjson ${SRCS})

TARGET_LINK_LIBRARIES(testjsjson libgtest libgmock)

# This needs to re-run cmake for each new test case, the beow command
# should fix this, but is not suppported on my system, or something
gtest_add_tests(testjsjson "" ${SRCS})
#add_test_suite(testjsjson
#  LIST_TEST_FLAGS   "--gtest_list_tests"
#  SELECT_TEST_FLAGS "--gtest_filter=<SECTION><TEST>"
#  SECTION_REGEX     "^([A-Za-z_/0-9]+)\\$"
#  TEST_REGEX        "  ([A-Za-z_/0-9])"
#  )
