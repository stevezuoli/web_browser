ENABLE_QT()

INCLUDE_DIRECTORIES(.)

SET(TS_PLUGIN_HDRS
  kindlets.h)
QT4_WRAP_CPP(MOC_SRCS ${TS_PLUGIN_HDRS})

SET(TS_PLUGIN_SRCS
  ${TS_PLUGIN_HDRS}
  kindlets.cpp
  tsplugin.cpp)

add_definitions(${QT_DEFINITIONS})

#IF (USE_BUILT_IN_DRIVER)

add_library(KindleTS ${TS_PLUGIN_SRCS} ${MOC_SRCS})
target_link_libraries(KindleTS duokan_base device ${QT_LIBRARIES} ${ADD_LIB})

#ELSE (USE_BUILT_IN_DRIVER)
#add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_SHARED)
#add_definitions(-DQT_NO_DEBUG)

#add_library(KindleTS SHARED ${TS_PLUGIN_SRCS})
#target_link_libraries(KindleTS device ${QT_LIBRARIES} ${ADD_LIB})

# Install the plugin to QT_PLUGINS_OUTPUT_PATH
#SET_TARGET_PROPERTIES(KindleTS PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${QT_PLUGINS_OUTPUT_PATH})

#ENDIF (USE_BUILT_IN_DRIVER)