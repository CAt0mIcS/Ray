add_subdirectory("glfw")
add_subdirectory("assimp")

SetWorkingDirectory("glfw" "")
SetOutputDirectory("glfw" "")

set_target_properties("glfw" PROPERTIES FOLDER "Extern")