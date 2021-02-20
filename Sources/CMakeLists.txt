set(SRC
	"Rpch.h"
	"RBase.h"
	

	# ------------------------------------------------------------------
	# Core classes
	"Core/REngine.h"
	"Core/REngine.cpp"
	"Core/RTime.h"
	"Core/RTime.cpp"
	"Core/RMath.h"
	"Core/RKeyCodes.h"
	

	# ------------------------------------------------------------------
	# Devices
	"Devices/RWindow.h"
	"Devices/RWindow.cpp"
	"Devices/RMouse.h"
	"Devices/RMouse.cpp"
	"Devices/RKeyboard.h"
	"Devices/RKeyboard.cpp"


	# ------------------------------------------------------------------
	# Graphics
	"Graphics/RGraphics.h"
	"Graphics/RGraphics.cpp"
	
	"Graphics/Core/RVulkanInstance.h"
	"Graphics/Core/RVulkanInstance.cpp"
	"Graphics/Core/RPhysicalDevice.h"
	"Graphics/Core/RPhysicalDevice.cpp"
	"Graphics/Core/RSurface.h"
	"Graphics/Core/RSurface.cpp"
	"Graphics/Core/RLogicalDevice.h"
	"Graphics/Core/RLogicalDevice.cpp"

	"Graphics/Buffers/RBuffer.h"
	"Graphics/Buffers/RBuffer.cpp"
	"Graphics/Buffers/RIndexBuffer.h"
	"Graphics/Buffers/RIndexBuffer.cpp"
	"Graphics/Buffers/RUniformBuffer.h"
	"Graphics/Buffers/RUniformBuffer.cpp"
	"Graphics/Buffers/RVertexBuffer.h"
	"Graphics/Buffers/RVertexBuffer.cpp"

	"Graphics/Images/RImage.h"
	"Graphics/Images/RImage.cpp"
	"Graphics/Images/RImage2D.h"
	"Graphics/Images/RImage2D.cpp"
	"Graphics/Images/RDepthImage.h"
	"Graphics/Images/RDepthImage.cpp"
	"Graphics/Images/RImageView.h"
	"Graphics/Images/RImageView.cpp"



	# ------------------------------------------------------------------
	# GUI


	# ------------------------------------------------------------------
	# Utilities
	"Utils/RSerialize.h"
	"Utils/RString.h"
	"Utils/RString.cpp"
	"Utils/RThreadPool.h"
	"Utils/RThreadPool.cpp"
	"Utils/RNonCopyable.h"
	"Utils/Logger/Formatters/RBracketFormatter.h"
	"Utils/Logger/Formatters/RDateTimeFormatter.h"
	"Utils/Logger/Formatters/RFormatter.h"
	"Utils/Logger/Formatters/RLogLevelFormatter.h"
	"Utils/Logger/Formatters/RNullTerminatorFormatter.h"
	"Utils/Logger/Loggers/RBaseLogger.h"
	"Utils/Logger/Loggers/RFileLogger.h"
	"Utils/Logger/Loggers/RConsoleLogger.h"
	"Utils/Logger/RLogLevel.h"
	"Utils/RLogger.h"
	"Utils/RLogger.cpp"
	"Utils/RAssert.h"
	"Utils/RQueue.h"
	"Utils/RException.h"
	"Utils/RException.cpp"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
    "${Vulkan_INCLUDE_DIRS}"
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Extern/entt/src"
    "${CMAKE_SOURCE_DIR}/Extern/glfw/include"
)

set(PUBLIC_LIB
    "glm"
)

set(PRIVATE_LIB
    "glfw"
    "${Vulkan_LIBRARIES}"
    "assimp"
    "glslang"
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

if(WIN32)
add_custom_command(TARGET "Ray" POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/Extern/assimp/lib/x64/assimp-vc142-mt.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo>"
)
endif()

