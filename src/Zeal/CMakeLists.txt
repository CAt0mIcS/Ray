set(SRC
    "src/pch.cpp"
    "src/pch.h"
    "src/Sandbox.cpp"
    "src/Sandbox.h"

    "src/ExtensionLoader/WindowsExtensionLoader.cpp"
    "src/ExtensionLoader/LinuxExtensionLoader.cpp"
    "src/ExtensionLoader/ExtensionLoader.h"
)

set(PUBLIC_INC
	""
)

set(PRIVATE_INC
    "src"
    "${CMAKE_SOURCE_DIR}/src/Ray/RayUtil/include"
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
    list(APPEND PRIVATE_LIB "dl")
endif()


add_executable("Zeal" WIN32 "${SRC}")
target_include_directories("Zeal"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("Zeal"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

add_dependencies("Zeal" "ZealGUILayer")
target_precompile_headers("Zeal" PRIVATE "src/pch.h")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("Zeal" "")
SetWorkingDirectory("Zeal" "")


