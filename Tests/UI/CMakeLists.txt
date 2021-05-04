set(SRC
	"main.cpp"
)

add_executable("UITest" "${SRC}")
target_include_directories("UITest" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("UITest" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("UITest" PRIVATE "Ray")

SetOutputDirectory("UITest" "")
SetWorkingDirectoryPlane("UITest" "${CMAKE_SOURCE_DIR}")