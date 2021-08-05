set(SRC
	"main.cpp"
)

add_executable("OpenGLRendering" "${SRC}")
target_include_directories("OpenGLRendering" PRIVATE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/Extern/glslang" "${CMAKE_SOURCE_DIR}/Extern")
target_compile_definitions("OpenGLRendering" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("OpenGLRendering" PRIVATE "Ray")

set_target_properties("OpenGLRendering" PROPERTIES FOLDER "Tests")

SetOutputDirectory("OpenGLRendering" "")
SetWorkingDirectoryPlane("OpenGLRendering" "${CMAKE_SOURCE_DIR}")
source_group("CSharp" FILES "${SCRIPT_SRC}")
