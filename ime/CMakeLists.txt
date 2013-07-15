
#Header files.
FILE(GLOB HDRS *.h)

#Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

#Resouce.
ADD_LIBRARY(ime ${SRCS})
TARGET_LINK_LIBRARIES(ime
    fcitx
    ${ADD_LIB})

ADD_EXECUTABLE(ime_test main.cpp)
TARGET_LINK_LIBRARIES(ime_test
    ime
    ${ADD_LIB})
