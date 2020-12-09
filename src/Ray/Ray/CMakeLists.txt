set(SRC
	"include/Ray/Application.cpp"
	"include/Ray/Core/ThreadPool.cpp"
	"include/Ray/Core/Rect.cpp"
	"include/Ray/Input/Keyboard.cpp"
	"include/Ray/Input/Mouse.cpp"
	"include/Ray/Layers/Layer.cpp"
	"include/Ray/Widgets/Widget.cpp"
	"include/Ray/rpch.h"
	"include/Ray/Window/WinAPIWindow.cpp"
	"include/Ray/Window/OpenGLWindow.cpp"
	"include/Ray/Window/Window.cpp"
	
	"include/Ray/Window/WinAPIWindow.h"
	"include/Ray/Window/OpenGLWindow.h"
	"include/Ray/Window/Window.h"
	"include/Ray/Core/ThreadPool.h"
	"include/Ray/Core/Queue.h"
	"include/Ray/Core/Rect.h"
	"include/Ray/Input/Keyboard.h"
	"include/Ray/Input/Mouse.h"
	"include/Ray/Layers/Layer.h"
	"include/Ray/Widgets/Widget.h"
	"include/Ray/Application.h"
	"include/Ray/Ray.h"
	"include/Ray/RBase.h"
	"include/Ray/Core/Stack.h"
	"include/Ray/EntryPoint.h"
	"include/Ray/Core/Timestep.h"

	"Ray.def"
)

set(PUBLIC_INC
	"include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extern/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extern/glfw"
)

set(PRIVATE_INC
	"include/Ray"
)

set(PUBLIC_LIB
	""
)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCCXX)
find_package("Threads" REQUIRED)
find_package("X11" REQUIRED)
list(APPEND PRIVATE_INC "${X11_INCLUDE_DIRS}")
set(PRIVATE_LIB
	"RayRender"
	"RayDebug"
	"RayEvent"
	"RayUtil"
	"Threads::Threads"
	"GLFW"
	"Glad"
	"${X11_LIBRARIES}"
)
else()
set(PRIVATE_LIB
	"RayRender"
	"RayEvent"
	"GLFW"
	"Glad"
	"RayDebug"
	"RayUtil"
)
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

target_compile_definitions("Ray" PRIVATE "RL_BUILD")
# add_dependencies("Ray" "RayRender")

# Add precompiled headers and move the cmake-generated file into a already existing filter (TODO)
target_precompile_headers("Ray" PRIVATE "include/Ray/rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("Ray" "")
SetWorkingDirectory("Ray" "")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND RL_BUILD_TESTS)
	add_subdirectory("tests")
endif()
