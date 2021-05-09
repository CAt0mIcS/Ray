set(SRC
	"main.cpp"
)

add_executable("MultipleRenderpasses" "${SRC}")
target_include_directories("MultipleRenderpasses" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("MultipleRenderpasses" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("MultipleRenderpasses" PRIVATE "Ray")

set_target_properties("MultipleRenderpasses" PROPERTIES FOLDER "Tests")

SetOutputDirectory("MultipleRenderpasses" "")
SetWorkingDirectoryPlane("MultipleRenderpasses" "${CMAKE_SOURCE_DIR}")
