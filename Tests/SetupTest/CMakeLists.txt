set(SRC
	"main.cpp"
)

add_executable("SetupTest" "${SRC}")
target_include_directories("SetupTest" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("SetupTest" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("SetupTest" PRIVATE "Ray")

SetOutputDirectory("SetupTest" "")
SetWorkingDirectoryPlane("SetupTest" "${CMAKE_SOURCE_DIR}")