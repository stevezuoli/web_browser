ENABLE_QT()

# Header files.
FILE(GLOB HDRS src/*.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS src/*.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

ADD_LIBRARY(kqoauth ${SRCS})
TARGET_LINK_LIBRARIES(kqoauth
 ${QT_LIBRARIES}
 ${ADD_LIB} )


