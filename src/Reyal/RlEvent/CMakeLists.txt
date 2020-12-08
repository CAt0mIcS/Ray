set(SRC
	"include/RlEvent/RlEvents.h"
	"include/RlEvent/Event.h"
	"include/RlEvent/ApplicationEvent.h"
	"include/RlEvent/KeyboardEvent.h"
	"include/RlEvent/MouseEvent.h"

	"include/RlEvent/EventDispatcher.h"
	"include/RlEvent/EventListener.h"
)

set(INC
    "include"
)


add_library("RlEvent" INTERFACE)
if(MSVC)
    add_custom_target("IRlEvent"
        SOURCES "${SRC}"
	)
endif()

target_include_directories("RlEvent" INTERFACE "${INC}")
AddRecursiveFilters("${SRC}")
