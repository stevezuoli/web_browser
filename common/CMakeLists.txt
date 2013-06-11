ENABLE_QT()

#Header files.
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

#Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

#Resouce.

ADD_LIBRARY(common ${SRCS})

TARGET_LINK_LIBRARIES(common
    device
    ${QT_LIBRARIES}
    ${ADD_LIB})
