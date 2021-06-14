set(SRC
	"main.cpp"
)

add_executable("UI" "${SRC}")
target_include_directories("UI" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("UI" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("UI" PRIVATE "Ray")

set_target_properties("UI" PROPERTIES FOLDER "Tests")

SetOutputDirectory("UI" "")
SetWorkingDirectoryPlane("UI" "${CMAKE_SOURCE_DIR}")