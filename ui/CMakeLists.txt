ENABLE_QT()

#Header files.
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

#Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

#Resouce.

ADD_LIBRARY(ui ${SRCS})

IF(UNIX OR BUILD_FOR_ARM)
TARGET_LINK_LIBRARIES(ui
    common
    device
	ime
	system
    ${QT_LIBRARIES}
    ${ADD_LIB})
ELSE (UNIX OR BUILD_FOR_ARM)
TARGET_LINK_LIBRARIES(ui
    common
    device
	system
    ${QT_LIBRARIES}
    ${ADD_LIB})
ENDIF(UNIX OR BUILD_FOR_ARM)