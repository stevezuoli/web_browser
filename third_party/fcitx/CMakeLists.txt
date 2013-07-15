#Header files.
FILE(GLOB HDRS *.h)

#Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

#Resouce.
ADD_LIBRARY(fcitx ${SRCS})

TARGET_LINK_LIBRARIES(fcitx
    ${ADD_LIB})
