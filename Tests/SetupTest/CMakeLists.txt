set(SRC
	"main.cpp"
)

add_executable("Setup" "${SRC}")
target_include_directories("Setup" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("Setup" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Setup" PRIVATE "Ray")

set_target_properties("Setup" PROPERTIES FOLDER "Tests")

SetOutputDirectory("Setup" "")
SetWorkingDirectoryPlane("Setup" "${CMAKE_SOURCE_DIR}")