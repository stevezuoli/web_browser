ENABLE_QT()

INCLUDE_DIRECTORIES(.)

SET(TS_PLUGIN_HDRS
  kindlets.h
  tsplugin.h)

SET(TS_PLUGIN_SRCS
  ${TS_PLUGIN_HDRS}
  kindlets.cpp
  tsplugin.cpp)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

add_library(KindleTS SHARED ${TS_PLUGIN_SRCS})
target_link_libraries(KindleTS device ${QT_LIBRARIES} ${ADD_LIB})

# Install the plugin to QT_PLUGINS_OUTPUT_PATH
SET_TARGET_PROPERTIES(KindleTS PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${QT_PLUGINS_OUTPUT_PATH})
