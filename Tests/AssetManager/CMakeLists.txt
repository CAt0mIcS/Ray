set(SRC
	"main.cpp"
)

add_executable("AssetManager" "${SRC}")
target_include_directories("AssetManager" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("AssetManager" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("AssetManager" PRIVATE "Ray")

set_target_properties("AssetManager" PROPERTIES FOLDER "Tests")

SetOutputDirectory("AssetManager" "")
SetWorkingDirectoryPlane("AssetManager" "${CMAKE_SOURCE_DIR}")
