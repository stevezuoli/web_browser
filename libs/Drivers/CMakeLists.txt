ENABLE_QT()

# Header files.
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

# Resources.

ADD_LIBRARY(drivers ${SRCS})

TARGET_LINK_LIBRARIES(drivers
  DkReader
  ${QT_LIBRARIES}
  ${ADD_LIB})