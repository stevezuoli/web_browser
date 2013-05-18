
ENABLE_QT()

INCLUDE_DIRECTORIES(inc)

# Header files.
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

ADD_EXECUTABLE(system_manager ${SRCS})
TARGET_LINK_LIBRARIES(system_manager
 system
 ${QT_LIBRARIES}
 ${ADD_LIB} )


