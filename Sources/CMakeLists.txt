set(SRC
	"Rpch.h"
	"RBase.h"
	
	# ------------------------------------------------------------------
	# Core classes
	"Core/RApplication.h"
	"Core/RApplication.cpp"
	
	# ------------------------------------------------------------------
	# Devices
	"Devices/RWindow.h"
	"Devices/RWindow.cpp"
	"Devices/RMouse.h"
	"Devices/RMouse.cpp"
	"Devices/RKeyboard.h"
	"Devices/RKeyboard.cpp"
	"Devices/RKeyCodes.h"


	# ------------------------------------------------------------------
	# Graphics
	

	# ------------------------------------------------------------------
	# GUI


	# ------------------------------------------------------------------
	# Utilities
	"Utils/RSerialize.h"
	"Utils/RString.h"
	"Utils/RString.cpp"
	"Utils/RTimestep.h"
	"Utils/RStack.h"
	"Utils/RQueue.h"
	"Utils/RThreadPool.h"
	"Utils/RThreadPool.cpp"
	"Utils/RMath.h"
	"Utils/RNonCopyable.h"

	# ------------------------------------------------------------------
	# Debug Utilities
	"Debug/Logger/Formatters/RBracketFormatter.h"
	"Debug/Logger/Formatters/RDateTimeFormatter.h"
	"Debug/Logger/Formatters/RFormatter.h"
	"Debug/Logger/Formatters/RLogLevelFormatter.h"
	"Debug/Logger/Formatters/RNullTerminatorFormatter.h"
	"Debug/Logger/Loggers/RBaseLogger.h"
	"Debug/Logger/Loggers/RFileLogger.h"
	"Debug/Logger/Loggers/RConsoleLogger.h"
	"Debug/Logger/RLogLevel.h"
	"Debug/RLogger.h"
	"Debug/RLogger.cpp"
	"Debug/RAssert.h"
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
    "ShaderConductor"
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
add_custom_command(TARGET "Ray" PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Resources/"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo>"
)

