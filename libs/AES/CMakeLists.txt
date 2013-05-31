# Header files.
FILE(GLOB HDRS *.h)

# Source files.
FILE(GLOB SRCS *.c)
SET(SRCS ${HDRS} ${SRCS})

# Resources.

ADD_LIBRARY(aes ${SRCS})

#ADD_EXECUTABLE(test_aes test/main.c)
#TARGET_LINK_LIBRARIES(test_aes aes)
