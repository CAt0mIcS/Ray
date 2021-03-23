set(SRC
	"imconfig.h"
	"imgui.cpp"
	"imgui.h"
	"imgui_demo.cpp"
	"imgui_draw.cpp"
	"imgui_internal.h"
	"imgui_tables.cpp"
	"imgui_widgets.cpp"
	"imstb_rectpack.h"
	"imstb_textedit.h"
	"imstb_truetype.h"
	"imgui_impl_glfw.cpp"
	"imgui_impl_glfw.h"
	"imgui_impl_vulkan.cpp"
	"imgui_impl_vulkan.h"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
    "${Vulkan_INCLUDE_DIRS}"
)

set(PRIVATE_INC
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
add_library("ImGui" STATIC "${SRC}")
target_include_directories("ImGui"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("ImGui"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)

AddRecursiveFilters("${SRC}")
SetOutputDirectory("ImGui" "")
SetWorkingDirectoryPlane("ImGui" "${CMAKE_SOURCE_DIR}")


