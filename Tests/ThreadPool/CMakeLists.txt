set(SRC
	"main.cpp"
)

add_executable("ThreadPool" "${SRC}")
target_include_directories("ThreadPool" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("ThreadPool" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("ThreadPool" PRIVATE "Ray")

set_target_properties("ThreadPool" PROPERTIES FOLDER "Tests")

SetOutputDirectory("ThreadPool" "")
SetWorkingDirectoryPlane("ThreadPool" "${CMAKE_SOURCE_DIR}")
