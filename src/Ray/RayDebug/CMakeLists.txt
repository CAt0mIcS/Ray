set(SRC
    "include/RayDebug/RInstrumentor.h"
    "include/RayDebug/rdpch.h"

    "include/RayDebug/RLogger.h"
    "include/RayDebug/RLogger.cpp"
    "include/RayDebug/RAssert.h"
    "include/RayDebug/RDBase.h"
)

set(PUBLIC_INC
    "include"
)

set(PRIVATE_INC
    ""
)

set(PUBLIC_LIB
    "RayUtil"
    "RayLogger"
)

set(PRIVATE_LIB
    ""
)


add_library("RayDebug" SHARED "${SRC}")
target_include_directories("RayDebug"
    PUBLIC 
        "${PUBLIC_INC}"
    PRIVATE
        "${PRIVATE_INC}"
)
target_link_libraries("RayDebug" 
    PRIVATE 
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

set_target_properties("RayDebug" PROPERTIES FOLDER "RayUtilities")
target_compile_definitions("RayDebug" PRIVATE "RD_BUILD")
target_precompile_headers("RayDebug" PRIVATE "include/RayDebug/Rdpch.h")
# add_dependencies("RlDebug" "RlUtilities")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayDebug" "")
SetWorkingDirectory("RayDebug" "")
