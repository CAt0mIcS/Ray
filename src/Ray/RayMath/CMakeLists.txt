set(SRC
	"include/RayMath/RFloat2.h"
	"include/RayMath/RVector.h"
	"include/RayMath/RUInt3.h"
	"include/RayMath/RUInt2.h"
	"include/RayMath/RMatrix.h"
	"include/RayMath/RMathInternals.h"
	"include/RayMath/RInt3.h"
	"include/RayMath/RInt2.h"
	"include/RayMath/RFloat4x4.h"
	"include/RayMath/RFloat4x3.h"
	"include/RayMath/RFloat3x4.h"
	"include/RayMath/RFloat3x3.h"
	"include/RayMath/RFloat3.h"
)

add_library("RayMath" INTERFACE)

add_custom_target("IRayMath"
    SOURCES "${SRC}"
)

set_target_properties("IRayMath" PROPERTIES FOLDER "RayUtilities")
AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayMath" "")