ENABLE_QT()

# Header files.
SET(GLOB EVERNOTE_EXPLORER_HDRS
  *.h)
 
FILE(GLOB HDRS *.h)
QT4_WRAP_CPP(MOC_SRCS ${HDRS})

# Source files.
FILE(GLOB SRCS *.cpp)
SET(SRCS ${HDRS} ${SRCS} ${MOC_SRCS})

# Executable file.
ADD_EXECUTABLE(evernote_explorer ${SRCS})
TARGET_LINK_LIBRARIES(evernote_explorer
   evernote
   ${QT_LIBRARIES}
   ${ADD_LIB}
)

# Install the evernote_explorer to $BUILD/bin
INSTALL(TARGETS evernote_explorer RUNTIME DESTINATION bin)
