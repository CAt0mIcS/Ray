set(SRC
	"RBase.h"

	# ------------------------------------------------------------------
	# Core classes
	"RTime.h"
	"RTime.cpp"
	"RMath.h"
	"RMath.cpp"
	
	# ------------------------------------------------------------------
	# File system
	"Filesystem/RJSON.h"
	"Filesystem/RJSON.cpp"

	# ------------------------------------------------------------------
	# Utilities
	"RString.h"
	"RString.cpp"
	"RThreadPool.h"
	"RThreadPool.cpp"
	"RNonCopyable.h"
	"RLogger.h"
	"RLogger.cpp"
	"RAssert.h"
	"RException.h"
	"RException.cpp"
	"RKeyCodes.h"

)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
	"${Vulkan_INCLUDE_DIRS}"
	"${CMAKE_SOURCE_DIR}"
)

set(PRIVATE_INC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_SOURCE_DIR}/Extern/rapidjson/include"
	"${CMAKE_SOURCE_DIR}/Extern"
)

set(PUBLIC_LIB
	"glm"
)

set(PRIVATE_LIB
	"${Vulkan_LIBRARIES}"
)


# ------------------------------------------------------------------
# Setup
add_library("RayBase" SHARED "${SRC}")
target_include_directories("RayBase"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("RayBase"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)
target_compile_definitions("RayBase" PRIVATE "RAYBASE_BUILD" "ENTT_DLL_EXPORT")
target_precompile_headers("RayBase" PRIVATE "Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayBase" "")
SetWorkingDirectoryPlane("RayBase" "${CMAKE_SOURCE_DIR}")


