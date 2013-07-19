
#Header files.
FILE(GLOB HDRS *.h)

#Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS})

#Resouce.
ADD_LIBRARY(ime ${SRCS})
TARGET_LINK_LIBRARIES(ime
    fcitx
	pthread
    ${ADD_LIB})

add_subdirectory(ime_test)
