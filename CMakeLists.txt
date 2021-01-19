cmake_minimum_required(VERSION 3.16)

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
# Options
option(RL_ARCHITECTURE "Set the build architecture to either x64 or x86" "x64")

# ------------------------------------------------------------------------------------------------
# Set architecture acording to RL_ARCHITECTURE option above
if(UNIX AND NOT APPLE)
    message(STATUS "Setting Build Architecture (Linux) to ${RL_ARCHITECTURE}")
    if("${RL_ARCHITECTURE}" STREQUAL "x64")
        set(CMAKE_C_FLAGS "-m64")
        set(CMAKE_CXX_FLAGS "-m64")
    elseif("${RL_ARCHITECTURE}" STREQUAL "x86")
        set(CMAKE_C_FLAGS "-m32")
        set(CMAKE_CXX_FLAGS "-m32")
    endif()
elseif(WIN32)
    # Should be automatically set using the -A switch in the .bat script
elseif(APPLE)
    message(STATUS "Setting Build Architecture (Apple) to ${RL_ARCHITECTURE}")
    if("${RL_ARCHITECTURE}" STREQUAL "x64")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    elseif("${RL_ARCHITECTURE}" STREQUAL "x86")
        set(CMAKE_OSX_ARCHITECTURES "i386")
    endif()
endif()

# TODO: Enable for gcc and clang only
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wno-attributes")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-attributes")
endif()

# ------------------------------------------------------------------------------------------------
# Set common options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# ------------------------------------------------------------------------------------------------
# Initialize the Solution Project
project("Ray")

# ------------------------------------------------------------------------------------------------
# Required for Solution Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# ------------------------------------------------------------------------------------------------
# Set Debug AND Release for Multi-Configuration Generators
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release" CACHE STRING "" FORCE)
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
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------------------------------------
# Helper Functions
include("BuildFiles/macros.cmake")

# ------------------------------------------------------------------------------------------------
# Create necessary directories
add_custom_target(CustomCommandManager ALL
     COMMAND ${CMAKE_COMMAND} -E make_directory 
     "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources/>"
     "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources/>"
     "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources/>"
)
set_target_properties(CustomCommandManager PROPERTIES FOLDER "CMakePredefinedTargets")


add_subdirectory("src")




