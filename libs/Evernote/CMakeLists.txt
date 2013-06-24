ENABLE_QT()

# Header files.
FILE(GLOB HDRS *.h)

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

ADD_LIBRARY(evernote ${SRCS})

TARGET_LINK_LIBRARIES(evernote
  database
  evernote_api
  ${THRIFT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  ${ADD_LIB})