ENABLE_QT()

# Header files.
FILE(GLOB HDRS inc/*.h)

# Source files.
FILE(GLOB SRCS src/*.cpp)
SET(SRCS ${HDRS} ${SRCS})

# Resources.

ADD_LIBRARY(DkReader ${SRCS})

TARGET_LINK_LIBRARIES(
  DkReader
  ${ADD_LIB})
