set(SRC
	"main.cpp"
)

SetMonoIncludesAndLibraries()

add_executable("Mono" "${SRC}")
target_include_directories("Mono" PRIVATE "${CMAKE_SOURCE_DIR}/Sources" "${MONO_INC}")
target_compile_definitions("Mono" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Mono" PRIVATE "Ray" "${MONO_LIB}")

set_target_properties("Mono" PROPERTIES FOLDER "Tests")

SetOutputDirectory("Mono" "")
SetWorkingDirectoryPlane("Mono" "${CMAKE_SOURCE_DIR}")