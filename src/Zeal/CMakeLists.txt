
# List of all source files
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
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "Reyal"
    "RlDebug"
    "RlRender"
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

# add_dependencies("Zeal" "Reyal" "ZealGUILayer")
add_dependencies("Zeal" "ZealGUILayer")
target_precompile_headers("Zeal" PRIVATE "src/pch.h")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("Zeal" "")




