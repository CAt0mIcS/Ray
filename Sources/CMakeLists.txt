set(SRC
	"Rpch.h"
	"RBase.h"
	
	# ------------------------------------------------------------------
	# Core classes
	"Core/RApplication.h"
	"Core/RApplication.cpp"
	
	# ------------------------------------------------------------------
	# Input
	"Input/RMouse.h"
	"Input/RMouse.cpp"
	"Input/RKeyboard.h"
	"Input/RKeyboard.cpp"
	"Input/RKeyCodes.h"
	
	# ------------------------------------------------------------------
	# Graphics
	
	
	# ------------------------------------------------------------------
	# Utilities
	"Utils/RSerialize.h"
	"Utils/RString.h"
	"Utils/RString.cpp"
	"Utils/RLogger.h"
	"Utils/RLogger.cpp"
	"Utils/RAssert.h"
	"Utils/RTimestep.h"
	"Utils/RStack.h"
	"Utils/RQueue.h"
	"Utils/RThreadPool.h"
	"Utils/RThreadPool.cpp"
	"Utils/RMath.h"


	# Logging
	"Utils/Logger/Formatters/RBracketFormatter.h"
	"Utils/Logger/Formatters/RDateTimeFormatter.h"
	"Utils/Logger/Formatters/RFormatter.h"
	"Utils/Logger/Formatters/RLogLevelFormatter.h"
	"Utils/Logger/Formatters/RNullTerminatorFormatter.h"
	"Utils/Logger/Loggers/RBaseLogger.h"
	"Utils/Logger/Loggers/RFileLogger.h"
	"Utils/Logger/Loggers/RConsoleLogger.h"
	"Utils/Logger/RLogLevel.h"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
    ""
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/Extern/entt/src"
    "${CMAKE_SOURCE_DIR}/Extern/glfw/include"
)

set(PUBLIC_LIB
    ""
)

set(PRIVATE_LIB
    "glfw"
    "${Vulkan_LIBRARIES}"
)


# ------------------------------------------------------------------
# Setup
add_library("Ray" SHARED "${SRC}")
target_include_directories("Ray"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("Ray"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)
target_compile_definitions("Ray" PRIVATE "RAY_BUILD")
target_precompile_headers("Ray" PRIVATE "Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("Ray" "")
SetWorkingDirectory("Ray" "")


# ------------------------------------------------------------------
# Custom commands
add_custom_command(TARGET "Ray" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources"
    "$<$<CONFIG:DEBUG>:RAY_OUTPUTPATH_Debug>"
    "$<$<CONFIG:RELEASE>:RAY_OUTPUTPATH_Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:RAY_OUTPUTPATH_RelWithDebInfo>"
)

