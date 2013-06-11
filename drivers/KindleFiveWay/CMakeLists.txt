ENABLE_QT()

INCLUDE_DIRECTORIES(.)

SET(FIVEWAY_PLUGIN_HDRS
    keycodes.h
    fivewayplugin.h
    kindlefiveway.h)
QT4_WRAP_CPP(MOC_SRCS ${FIVEWAY_PLUGIN_HDRS})

SET(FIVEWAY_PLUGIN_SRCS
  ${FIVEWAY_PLUGIN_HDRS}
  fivewayplugin.cpp
  kindlefiveway.cpp)

add_definitions(${QT_DEFINITIONS})

#IF (USE_BUILT_IN_DRIVER)

add_library(KindleFiveWay ${FIVEWAY_PLUGIN_SRCS} ${MOC_SRCS})
target_link_libraries(KindleFiveWay device ${QT_LIBRARIES} ${ADD_LIB})

#ELSE (USE_BUILT_IN_DRIVER)
#add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_SHARED)
#add_definitions(-DQT_NO_DEBUG)

#add_library(KindleFiveWay SHARED ${FIVEWAY_PLUGIN_SRCS})
#target_link_libraries(KindleFiveWay device ${QT_LIBRARIES} ${ADD_LIB})

# Install the plugin to QT_PLUGINS_OUTPUT_PATH
#SET_TARGET_PROPERTIES(KindleFiveWay PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${QT_PLUGINS_OUTPUT_PATH})

#ENDIF (USE_BUILT_IN_DRIVER)
