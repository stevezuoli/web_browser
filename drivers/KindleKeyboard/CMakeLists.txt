ENABLE_QT()

INCLUDE_DIRECTORIES(.)

SET(KEYBOARD_PLUGIN_HDRS
    kindlekeyboard.h
    keycodes.h
    keyboardplugin.h)
QT4_WRAP_CPP(MOC_SRCS ${KEYBOARD_PLUGIN_HDRS})

SET(KEYBOARD_PLUGIN_SRCS
  ${KEYBOARD_PLUGIN_HDRS}
  kindlekeyboard.cpp
  keyboardplugin.cpp)

#IF (USE_BUILT_IN_DRIVER)

add_library(KindleKeyboard ${KEYBOARD_PLUGIN_SRCS} ${MOC_SRCS})
target_link_libraries(KindleKeyboard screenshot keymap ${QT_LIBRARIES} ${ADD_LIB})

#ELSE(USE_BUILT_IN_DRIVER)
#add_definitions(${QT_DEFINITIONS})
#add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_SHARED)
#add_definitions(-DQT_NO_DEBUG)
#add_library(KindleKeyboard SHARED ${KEYBOARD_PLUGIN_SRCS})
#target_link_libraries(KindleKeyboard screenshot keymap ${QT_LIBRARIES} ${ADD_LIB})
# Install the plugin to QT_PLUGINS_OUTPUT_PATH
#SET_TARGET_PROPERTIES(KindleKeyboard PROPERTIES  RUNTIME_OUTPUT_DIRECTORY ${QT_PLUGINS_OUTPUT_PATH})
#ENDIF(USE_BUILT_IN_DRIVER)

add_subdirectory(screenshot)
add_subdirectory(keymap)