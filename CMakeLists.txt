cmake_minimum_required(VERSION 3.18)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    if(NOT DEFINED IN_SOURCE_BUILD)
        message(FATAL_ERROR
          "CMake generation not allowed within the source directory!"
          "\n Remove \"${CMAKE_SOURCE_DIR}/CMakeCache.txt\" and try again from another folder, e.g.:"
          "\n "
          "\n rm CMakeCache.txt"
          "\n mkdir build"
          "\n cd build"
          "\n cmake ../"
          "\n "
          "\n Alternatively define IN_SOURCE_BUILD to force this option (not recommended!)"
        )
    endif()
endif()

# ------------------------------------------------------------------------------------------------
# Initialize the Solution Project
project("NodePlanningEditor")

# ------------------------------------------------------------------------------------------------
# Options
option(RL_ARCHITECTURE "Set the build architecture to either x64 or x86" "x64")

# Set architecture acording to option above
if(LINUX AND NOT APPLE)
    if("${RL_ARCHITECTURE}" STREQUAL "x64")
        set(CFLAGS "${CFLAGS} -m64")
    elseif("${RL_ARCHITECTURE}" STREQUAL "x86")
        set(CFLAGS "${CFLAGS} -m32")
    endif()
elseif(WIN32)
    # Should be automatically set using the -A switch in the .bat script
elseif(APPLE)
    if("${RL_ARCHITECTURE}" STREQUAL "x64")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    elseif("${RL_ARCHITECTURE}" STREQUAL "x86")
        set(CMAKE_OSX_ARCHITECTURES "i386")
    endif()
endif()

# ------------------------------------------------------------------------------------------------
# Required for Solution Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# ------------------------------------------------------------------------------------------------
# Set Debug AND Release for Multi-Configuration Generators
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
else()
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}")
endif()

# ------------------------------------------------------------------------------------------------
# Set the startup project for VS
if(MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "Zeal")
endif()

# ------------------------------------------------------------------------------------------------
# Set default string stuff to be unicode
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

# ------------------------------------------------------------------------------------------------
# Set C++ Version requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# ------------------------------------------------------------------------------------------------
# Helper Functions
include("BuildFiles/macros.cmake")


add_subdirectory("src")

# ------------------------------------------------------------------------------------------------
# Disable all warnings and add pthread (TODO, not a very clean way of doing it!)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCCXX)
    set(CMAKE_CXX_FLAGS "-pthread")
endif()






