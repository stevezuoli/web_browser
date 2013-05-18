ENABLE_QT()

# Header files.
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

# Resources.

ADD_LIBRARY(screen ${SRCS})

TARGET_LINK_LIBRARIES(screen
  duokan_base
  drivers
  ${QT_LIBRARIES}
  ${ADD_LIB})