ENABLE_QT()

INCLUDE_DIRECTORIES(.)

SET(SCREEN_TRANSFORM_PLUGIN_HDRS
    qscreentransformed_qws.h
  )

SET(SCREEN_TRANSFORM_PLUGIN_SRCS
    ${SCREEN_TRANSFORM_PLUGIN_HDRS}
    qscreentransformed_qws.cpp
  )

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

add_library(qgfxtransformed SHARED ${SCREEN_TRANSFORM_PLUGIN_SRCS})
target_link_libraries(qgfxtransformed device ${QT_LIBRARIES} ${ADD_LIB})

add_library(gfxtransformed ${SCREEN_TRANSFORM_PLUGIN_SRCS})
target_link_libraries(gfxtransformed device ${QT_LIBRARIES} ${ADD_LIB})

# Install the plugin to QT_PLUGINS_OUTPUT_PATH
SET_TARGET_PROPERTIES(qgfxtransformed PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${QT_PLUGINS_OUTPUT_PATH})
