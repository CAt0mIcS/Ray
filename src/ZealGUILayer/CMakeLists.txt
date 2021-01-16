set(SRC
    "src/glbase.h"
    "src/glpch.cpp"
    "src/glpch.h"
    "src/GUILayer.cpp"
    "src/GUILayer.h"
)

set(PUBLIC_INC
	""
)

set(PRIVATE_INC
    "${CMAKE_SOURCE_DIR}/src/Ray/RayDebug/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "Ray"
    "RayRender"
)

if(UNIX)
	list(APPEND PRIVATE_LIB "RayUtil")
endif()

add_library("ZealGUILayer" SHARED "${SRC}")
target_include_directories("ZealGUILayer"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("ZealGUILayer"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

set_target_properties("ZealGUILayer" PROPERTIES FOLDER "LayerExtensions")
target_precompile_headers("ZealGUILayer" PRIVATE "src/glpch.h")
AddRecursiveFilters("${SRC}")

SetOutputDirectory("ZealGUILayer" "Editors")
SetWorkingDirectory("ZealGUILayer" "")

add_custom_command(TARGET "ZealGUILayer" POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E make_directory 
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources/>"
)

add_custom_command(TARGET "ZealGUILayer" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/res/nanosuit.obj" 
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources/>"
)

add_custom_command(TARGET "ZealGUILayer" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/res/Cube.obj" 
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources/>"
)

add_custom_command(TARGET "ZealGUILayer" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/res/Cube.mtl" 
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources/>"
)

