ENABLE_QT()

INCLUDE_DIRECTORIES(.)

SET(SCREEN_PLUGIN_HDRS
  duokan_screen.h)

SET(SCREEN_PLUGIN_SRCS
  ${SCREEN_PLUGIN_HDRS}
  duokan_screen.cpp
  duokan_screen_plugin.cpp)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

add_library(duokanscreen SHARED ${SCREEN_PLUGIN_SRCS})
target_link_libraries(duokanscreen ${QT_LIBRARIES} ${ADD_LIB})

# Install the plugin to QT_PLUGINS_OUTPUT_PATH
SET_TARGET_PROPERTIES(duokanscreen PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${QT_PLUGINS_OUTPUT_PATH})
