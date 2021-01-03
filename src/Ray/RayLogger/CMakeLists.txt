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

    "RayLoggerStatic.cpp"
)

# set(INC
#     "include"
# )


add_library("RayLogger" STATIC "${SRC}")
set_target_properties("RayLogger" PROPERTIES FOLDER "RayUtilities")

# target_include_directories("RayLogger" PRIVATE "${INC}")
target_link_libraries("RayLogger" PRIVATE "RayUtil")

AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayLogger" "")
SetOutputDirectory("RayLogger" "")