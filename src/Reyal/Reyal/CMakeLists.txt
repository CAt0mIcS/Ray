set(SRC
	"include/Reyal/Application.cpp"
	"include/Reyal/Core/ThreadPool.cpp"
	"include/Reyal/Core/ZRect.cpp"
	"include/Reyal/Input/Keyboard.cpp"
	"include/Reyal/Input/Mouse.cpp"
	"include/Reyal/Layers/Layer.cpp"
	"include/Reyal/Widgets/ZWidget.cpp"
	"include/Reyal/rlpch.h"
	"include/Reyal/Window/WinAPIWindow.cpp"
	"include/Reyal/Window/OpenGLWindow.cpp"
	"include/Reyal/Window/Window.cpp"
	
	"include/Reyal/Window/WinAPIWindow.h"
	"include/Reyal/Window/OpenGLWindow.h"
	"include/Reyal/Window/Window.h"
	"include/Reyal/Core/ThreadPool.h"
	"include/Reyal/Core/Queue.h"
	"include/Reyal/Core/ZRect.h"
	"include/Reyal/Events/Event.h"
	"include/Reyal/Events/ApplicationEvent.h"
	"include/Reyal/Events/KeyboardEvent.h"
	"include/Reyal/Events/MouseEvent.h"
	"include/Reyal/Input/Keyboard.h"
	"include/Reyal/Input/Mouse.h"
	"include/Reyal/Layers/Layer.h"
	"include/Reyal/Widgets/ZWidget.h"
	"include/Reyal/Application.h"
	"include/Reyal/Reyal.h"
	"include/Reyal/RlBase.h"
	"include/Reyal/Core/Stack.h"
	"include/Reyal/EntryPoint.h"
)

set(PUBLIC_INC
	"include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extern/glm"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extern/glfw"
)

set(PRIVATE_INC
	"include/Reyal"
)

set(PUBLIC_LIB
	""
)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCCXX)
find_package("Threads" REQUIRED)
find_package("X11" REQUIRED)
list(APPEND PRIVATE_INC "${X11_INCLUDE_DIRS}")
set(PRIVATE_LIB
	"RlRender"
	"Threads::Threads"
	"GLFW"
	"Glad"
	"${X11_LIBRARIES}"
)
else()
set(PRIVATE_LIB
	"RlRender"
	"GLFW"
	"Glad"
)
endif()

add_library("Reyal" SHARED "${SRC}")

# Setting the include directory public will add the include directory to all Projects depending on it
target_include_directories("Reyal" 
	PUBLIC 
		"${PUBLIC_INC}" 
	PRIVATE 
		"${PRIVATE_INC}"
)

target_link_libraries("Reyal" 
	PRIVATE 
		"${PRIVATE_LIB}"
	PUBLIC
		"${PUBLIC_LIB}"
)

target_compile_definitions("Reyal" PRIVATE "RL_BUILD")
# add_dependencies("Reyal" "RlRender")

# Add precompiled headers and move the cmake-generated file into a already existing filter (TODO)
target_precompile_headers("Reyal" PRIVATE "include/Reyal/rlpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("Reyal" "")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND RL_BUILD_TESTS)
	add_subdirectory("tests")
endif()
