add_subdirectory("glfw")
add_subdirectory("glm")
add_subdirectory("glslang")
add_subdirectory("assimp")
add_subdirectory("Ion")
# add_subdirectory("ktx")

set(SKIP_INSTALL_ALL TRUE)
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE) # we use the font vectors directly; HarfBuzz’s hinting is not used
add_subdirectory("freetype2")

SetOutputDirectory("glfw" "")
SetOutputDirectory("freetype" "")


SetOutputDirectory("glslang" "")
SetOutputDirectory("OGLCompiler" "")
SetOutputDirectory("OSDependent" "")
SetOutputDirectory("SPIRV" "")
SetOutputDirectory("GenericCodeGen" "")
SetOutputDirectory("MachineIndependent" "")

set_target_properties("glfw" PROPERTIES FOLDER "Extern")
set_target_properties("freetype" PROPERTIES FOLDER "Extern")

set_target_properties("glslang" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("OGLCompiler" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("OSDependent" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("SPIRV" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("GenericCodeGen" PROPERTIES FOLDER "Extern/glslang")
set_target_properties("MachineIndependent" PROPERTIES FOLDER "Extern/glslang")


set_target_properties("Ion" PROPERTIES FOLDER "Extern")
