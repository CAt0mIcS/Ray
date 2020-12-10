set(SRC
	"include/RayEvent/RayEvent.h"

	"include/RayEvent/REvent.h"
	"include/RayEvent/RApplicationEvent.h"
	"include/RayEvent/RKeyboardEvent.h"
	"include/RayEvent/RMouseEvent.h"
	# "include/RayEvent/Events/REvent.h"
	# "include/RayEvent/Events/RApplicationEvent.h"
	# "include/RayEvent/Events/RKeyboardEvent.h"
	# "include/RayEvent/Events/RMouseEvent.h"
	# "include/RayEvent/Events/RApplicationEvent.cpp"
	# "include/RayEvent/Events/RKeyboardEvent.cpp"
	# "include/RayEvent/Events/RMouseEvent.cpp"

	"include/RayEvent/REventDispatcher.h"
	"include/RayEvent/REventListener.h"

	# "include/RayEvent/REBase.h"
)

set(INC
    "include"
)


add_library("RayEvent" INTERFACE)
# add_library("RayEvent" SHARED "${SRC}")
target_include_directories("RayEvent" INTERFACE "${INC}")
AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayEvent" "")
