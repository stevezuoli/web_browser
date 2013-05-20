
ENABLE_QT()

INCLUDE_DIRECTORIES(inc)

# Header files.
FILE(GLOB HDRS inc/*.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS src/*.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

ADD_LIBRARY(system ${SRCS})
TARGET_LINK_LIBRARIES(system
 fblib
 screen
 ${QT_LIBRARIES}
 ${ADD_LIB} )


