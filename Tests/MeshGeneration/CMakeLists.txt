set(SRC
	"main.cpp"
)

add_executable("MeshGeneration" "${SRC}")
target_include_directories("MeshGeneration" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("MeshGeneration" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("MeshGeneration" PRIVATE "Ray")

set_target_properties("MeshGeneration" PROPERTIES FOLDER "Tests")

SetOutputDirectory("MeshGeneration" "")
SetWorkingDirectoryPlane("MeshGeneration" "${CMAKE_SOURCE_DIR}")