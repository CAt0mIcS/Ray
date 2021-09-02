set(SRC
	"main.cpp"
)

add_executable("MaterialSystem" "${SRC}")
target_include_directories("MaterialSystem" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("MaterialSystem" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("MaterialSystem" PRIVATE "Ray")

set_target_properties("MaterialSystem" PROPERTIES FOLDER "Tests")

SetOutputDirectory("MaterialSystem" "")
SetWorkingDirectoryPlane("MaterialSystem" "${CMAKE_SOURCE_DIR}")
