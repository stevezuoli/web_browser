
#Header files.
FILE(GLOB HDRS *.h)

#Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

ADD_EXECUTABLE(ime_test main.cpp)
TARGET_LINK_LIBRARIES(ime_test
    ime
    ${ADD_LIB})
