set(SRC
    "include/RayUtil/Rupch.h"
    "include/RayUtil/RCoreConfig.h"
    "include/RayUtil/RUBase.h"
    "include/RayUtil/RSerialize.h"
    "include/RayUtil/RKeyCodes.h"
    "include/RayUtil/RGlobalDefines.h"

    "include/RayUtil/RException.h"
    "include/RayUtil/RException.cpp"
    "include/RayUtil/RRandom.h"
    "include/RayUtil/RRandom.cpp"
    "include/RayUtil/RTypeConvert.h"
    "include/RayUtil/RTypeConvert.cpp"
    "include/RayUtil/RInfoManager.h"
    "include/RayUtil/RInfoManager.cpp"
    "include/RayUtil/RMessageBox.h"
    "include/RayUtil/RMessageBox.cpp"
    "include/RayUtil/RHelper.h"
    "include/RayUtil/RHelper.cpp"
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

target_precompile_headers("RayUtil" PRIVATE "include/RayUtil/Rupch.h")
set_target_properties("RayUtil" PROPERTIES FOLDER "RayUtilities")
target_compile_definitions("RayUtil" PRIVATE "RU_BUILD")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayUtil" "")
SetWorkingDirectory("RayUtil" "")

# ===================================================================================
# Dependencies
add_subdirectory("extern")
