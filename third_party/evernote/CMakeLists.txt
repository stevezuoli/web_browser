ENABLE_QT()

# Header files.
FILE(GLOB HDRS *.h)

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

ADD_LIBRARY(evernote_api ${SRCS})

TARGET_LINK_LIBRARIES(evernote_api
  ${THRIFT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  ${ADD_LIB})

ADD_EXECUTABLE(test_evernote test/test.cpp)
TARGET_LINK_LIBRARIES(test_evernote
 evernote_api)

