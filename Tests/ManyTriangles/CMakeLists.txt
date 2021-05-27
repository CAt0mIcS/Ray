set(SRC
	"main.cpp"
)

add_executable("ManyTriangles" "${SRC}")
target_include_directories("ManyTriangles" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("ManyTriangles" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("ManyTriangles" PRIVATE "Ray")

set_target_properties("ManyTriangles" PROPERTIES FOLDER "Tests")

SetOutputDirectory("ManyTriangles" "")
SetWorkingDirectoryPlane("ManyTriangles" "${CMAKE_SOURCE_DIR}")
