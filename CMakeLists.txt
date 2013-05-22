# Make sure you already have cmake 2.6 installed. For cross compiling.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
# Additional cmake modules
SET(CMAKE_FIND_STATIC FIRST) 
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# Boox project.
Project(Duokan CXX C)
ENABLE_TESTING()

# Global definitions
SET(ADD_LIB "")
IF(UNIX OR BUILD_FOR_ARM)
    ADD_DEFINITIONS(-DSHARE_ROOT="/usr/share")
ELSE (UNIX OR BUILD_FOR_ARM)
    ADD_DEFINITIONS(-DSHARE_ROOT="")
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF (UNIX OR BUILD_FOR_ARM)

# Check to use arm toolchain or not
IF(BUILD_FOR_ARM)
    INCLUDE(arm_toolchain)
    USE_ARM_TOOLCHAIN()
	include(FindTsLib)
	#include(FindDBus)
ELSE(BUILD_FOR_ARM)
link_directories("/usr/lib")
ENDIF(BUILD_FOR_ARM)

# Project include directories.
INCLUDE_DIRECTORIES(BEFORE
    ${CMAKE_SOURCE_DIR}/inc
	${CMAKE_SOURCE_DIR}/libs
	${CMAKE_SOURCE_DIR}/libs/DkReader/inc
)

# Find thread library for the current platform
include(enable_qt)
#include(misc)
include(strict_warning)
include(tc)

LINK_DIRECTORIES(${QT_LIBRARY_DIR})

# Output directories.
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)
SET(TEST_OUTPUT_PATH ${PROJECT_BINARY_DIR}/unittests)
SET(QT_PLUGINS_OUTPUT_PATH ${PROJECT_BINARY_DIR}/plugins)

add_subdirectory(system_manager)
add_subdirectory(web_browser)
add_subdirectory(libs)
add_subdirectory(drivers/screen)
add_subdirectory(drivers/kermit)
add_subdirectory(drivers/QKindleFb)

