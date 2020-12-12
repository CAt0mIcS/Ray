set(SRC
    "include/RayUtil/utpch.h"
    "include/RayUtil/CoreConfig.h"
    "include/RayUtil/RlUBase.h"
    "include/RayUtil/Serialize.h"
    "include/RayUtil/KeyCodes.h"

    "include/RayUtil/Exception.h"
    "include/RayUtil/Exception.cpp"
    "include/RayUtil/Random.h"
    "include/RayUtil/Random.cpp"
    "include/RayUtil/TypeConvert.h"
    "include/RayUtil/TypeConvert.cpp"
    "include/RayUtil/InfoManager.h"
    "include/RayUtil/InfoManager.cpp"
    "include/RayUtil/MessageBox.h"
    "include/RayUtil/MessageBox.cpp"
    "include/RayUtil/Helper.h"
    "include/RayUtil/Helper.cpp"
)

set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/dxerr/include"
)

set(PUBLIC_LIB
	""
)

if(WIN32)
	set(PRIVATE_LIB
		"DxErr"
	)
else()
	set(PRIVATE_LIB
		""
	)
endif()

add_library("RayUtil" SHARED "${SRC}")
target_include_directories("RayUtil"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("RayUtil"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

target_precompile_headers("RayUtil" PRIVATE "include/RayUtil/utpch.h")
set_target_properties("RayUtil" PROPERTIES FOLDER "RayUtilities")
target_compile_definitions("RayUtil" PRIVATE "RU_BUILD")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayUtil" "")
SetWorkingDirectory("RayUtil" "")

# ===================================================================================
# Dependencies
add_subdirectory("extern")
