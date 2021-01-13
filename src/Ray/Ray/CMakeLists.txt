set(SRC
	"include/Ray/RApplication.h"
	"include/Ray/RApplication.cpp"
	"include/Ray/Rpch.h"
	"include/Ray/Ray.h"
	"include/Ray/RBase.h"
	"include/Ray/REntryPoint.h"
	
	"include/Ray/Core/RThreadPool.h"
	"include/Ray/Core/RThreadPool.cpp"
	"include/Ray/Core/RRect.h"
	"include/Ray/Core/RQueue.h"
	"include/Ray/Core/RRect.cpp"	
	"include/Ray/Core/RStack.h"
	"include/Ray/Core/RTimestep.h"
	"include/Ray/Core/RComponents.h"
	
	"include/Ray/Input/RKeyboard.h"
	"include/Ray/Input/RKeyboard.cpp"
	"include/Ray/Input/RMouse.h"
	"include/Ray/Input/RMouse.cpp"
	
	"include/Ray/Layers/RLayer.h"
	"include/Ray/Layers/RLayer.cpp"
	
	"include/Ray/Widgets/RWidget.h"
	"include/Ray/Widgets/RWidget.cpp"
	"include/Ray/Widgets/RButton.h"
	"include/Ray/Widgets/RButton.cpp"

	"include/Ray/Window/RWinAPIWindow.h"
	"include/Ray/Window/RWinAPIWindow.cpp"
	"include/Ray/Window/ROpenGLWindow.h"
	"include/Ray/Window/ROpenGLWindow.cpp"
	"include/Ray/Window/RWindow.h"
	"include/Ray/Window/RWindow.cpp"

	"Ray.def"
)

set(PUBLIC_INC
	"include"
)

set(PRIVATE_INC
	"${CMAKE_CURRENT_SOURCE_DIR}/../extern/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extern/glfw"
	"${CMAKE_CURRENT_SOURCE_DIR}/extern/stb_image/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"RayRender"
	"RayEvent"
	"GLFW"
	"Glad"
	"RayDebug"
	"RayUtil"
)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCCXX)
find_package("Threads" REQUIRED)
find_package("X11" REQUIRED)
list(APPEND PRIVATE_INC "${X11_INCLUDE_DIRS}")
list(APPEND PRIVATE_LIB "${X11_LIBRARIES}" "Threads::Threads")
endif()


add_library("Ray" SHARED "${SRC}")

# Setting the include directory public will add the include directory to all Projects depending on it
target_include_directories("Ray" 
	PUBLIC 
		"${PUBLIC_INC}" 
	PRIVATE 
		"${PRIVATE_INC}"
)

target_link_libraries("Ray" 
	PRIVATE 
		"${PRIVATE_LIB}"
	PUBLIC
		"${PUBLIC_LIB}"
)

target_compile_definitions("Ray" PRIVATE "RAY_BUILD")
# add_dependencies("Ray" "RayRender")

target_precompile_headers("Ray" PRIVATE "include/Ray/Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("Ray" "")
SetWorkingDirectory("Ray" "")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND RL_BUILD_TESTS)
	add_subdirectory("tests")
endif()
