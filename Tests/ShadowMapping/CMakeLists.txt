set(SHADERS
	"Shaders/DebugQuad.vert"
	"Shaders/DebugQuad.frag"
)

set(SRC
	"main.cpp"

	${SHADERS}
)

find_package("Vulkan" REQUIRED FATAL_ERROR)

add_executable("ShadowMapping" "${SRC}")
target_include_directories("ShadowMapping" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("ShadowMapping" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("ShadowMapping" PRIVATE "Ray" "${Vulkan_LIBRARIES}")

set_target_properties("ShadowMapping" PROPERTIES FOLDER "Tests")

SetOutputDirectory("ShadowMapping" "")
SetWorkingDirectoryPlane("ShadowMapping" "${CMAKE_SOURCE_DIR}")

AddRecursiveFilters("${SHADERS}")

