set(SRC
	"include/ShaderConductor/ShaderConductor.hpp"
)

set(INC
	"include"
)

if(WIN32)
	set(LIB
		"ShaderConductor.lib"
	)
else()
	set(LIB
		"libShaderConductor.so"
	)
endif()

add_library("ShaderConductor" INTERFACE)

add_custom_target("IShaderConductor" SOURCES "${SRC}")
set_target_properties("IShaderConductor" PROPERTIES FOLDER "Extern")

target_link_directories("ShaderConductor" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/")
target_link_libraries("ShaderConductor" INTERFACE "${LIB}")
target_include_directories("ShaderConductor" INTERFACE "${INC}")
AddRecursiveFilters("${SRC}")