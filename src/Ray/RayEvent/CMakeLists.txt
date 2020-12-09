set(SRC
	"include/RayEvent/RayEvent.h"
	"include/RayEvent/REvent.h"
	"include/RayEvent/RApplicationEvent.h"
	"include/RayEvent/RKeyboardEvent.h"
	"include/RayEvent/RMouseEvent.h"

	"include/RayEvent/REventDispatcher.h"
	"include/RayEvent/REventListener.h"
)

set(INC
    "include"
)


add_library("RayEvent" INTERFACE)
if(MSVC)
    add_custom_target("IRayEvent"
        SOURCES "${SRC}"
	)
endif()

target_include_directories("RayEvent" INTERFACE "${INC}")
AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayEvent" "")
