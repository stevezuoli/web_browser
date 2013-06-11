# Header files.
FILE(GLOB HDRS *.h)

# Source files.
FILE(GLOB SRCS *.c)
SET(SRCS ${HDRS} ${SRCS})

# Resources.

ADD_LIBRARY(keymap ${SRCS})
