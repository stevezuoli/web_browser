# Header files.
FILE(GLOB HDRS *.h)

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

# Resources.

ADD_LIBRARY(aes ${SRCS})

ADD_EXECUTABLE(test_aes test/main.cpp)
TARGET_LINK_LIBRARIES(test_aes aes)
