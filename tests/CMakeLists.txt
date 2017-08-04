file(GLOB SRCS *.cc)

ADD_EXECUTABLE(testjsjson ${SRCS})

TARGET_LINK_LIBRARIES(testjsjson libgtest libgmock)

add_test(NAME testjsjson COMMAND testjsjson)
