set(SRC
	"include/RayECS/RayECS.h"
	"include/RayECS/RInternal.h"
	"include/RayECS/RRegistry.h"
	"include/RayECS/REntityStorage.h"
	"include/RayECS/RStorage.h"
	"include/RayECS/RView.h"
)

add_library("RayECS" INTERFACE)

if(MSVC)
    add_custom_target("IRayECS"
        SOURCES "${SRC}"
    )
endif()

AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayECS" "")