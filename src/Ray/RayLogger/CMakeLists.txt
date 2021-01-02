set(SRC
    "include/RayLogger/Formatters/RBracketFormatter.h"
    "include/RayLogger/Formatters/RDateTimeFormatter.h"
    "include/RayLogger/Formatters/RFormatter.h"
    "include/RayLogger/Formatters/RLogLevelFormatter.h"
    "include/RayLogger/Formatters/RNullTerminatorFormatter.h"
    "include/RayLogger/Loggers/RBaseLogger.h"
    "include/RayLogger/Loggers/RFileLogger.h"
    "include/RayLogger/RLogConfigure.h"
    "include/RayLogger/RLogLevel.h"

    "include/RayLogger/RLog.h"
)

# set(INC
#     "include"
# )


add_library("RayLogger" INTERFACE)

add_custom_target("IRayLogger"
    SOURCES "${SRC}"
)
set_target_properties("IRayLogger" PROPERTIES FOLDER "RayUtilities")

# target_include_directories("RayLogger" INTERFACE "${INC}")
target_link_libraries("RayLogger" INTERFACE "RayUtil")

AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayLogger" "")
