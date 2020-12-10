set(SRC
	"include/RayEvent/RayEvent.h"

	"include/RayEvent/Events/REvent.h"
	"include/RayEvent/Events/RApplicationEvent.h"
	"include/RayEvent/Events/RKeyboardEvent.h"
	"include/RayEvent/Events/RMouseEvent.h"
	"include/RayEvent/Events/RApplicationEvent.cpp"
	"include/RayEvent/Events/RKeyboardEvent.cpp"
	"include/RayEvent/Events/RMouseEvent.cpp"

	"include/RayEvent/REventDispatcher.h"
	"include/RayEvent/REventListener.h"
)

set(INC
    "include"
)


add_library("RayEvent" STATIC "${SRC}")
target_include_directories("RayEvent" PUBLIC "${INC}")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayEvent" "")
SetWorkingDirectory("RayEvent" "")
