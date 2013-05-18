ENABLE_QT()

# Header files.
SET(GLOB WEBBROWSER_HDRS
  *.h)
 
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

# Executable file.
ADD_EXECUTABLE(web_browser ${SRCS})
TARGET_LINK_LIBRARIES(web_browser
   system
   screen
   ${QT_LIBRARIES}
   ${ADD_LIB}
)

# Install the web_browser to $BUILD/bin
INSTALL(TARGETS web_browser RUNTIME DESTINATION bin)

