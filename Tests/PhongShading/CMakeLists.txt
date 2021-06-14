set(SRC
	"main.cpp"
)

add_executable("PhongShading" "${SRC}")
target_include_directories("PhongShading" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("PhongShading" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("PhongShading" PRIVATE "Ray")

set_target_properties("PhongShading" PROPERTIES FOLDER "Tests")

SetOutputDirectory("PhongShading" "")
SetWorkingDirectoryPlane("PhongShading" "${CMAKE_SOURCE_DIR}")