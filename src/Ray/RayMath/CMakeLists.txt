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
	"include/RayMath/RFloat4.h"
	"include/RayMath/RQuaternion.h"

	"RMathStatic.cpp"
)

# Cannot set CXX17 for INTERFACE libraries. Adding temporary cpp file and making it a static library
add_library("RayMath" STATIC "${SRC}")

set_target_properties("RayMath" PROPERTIES FOLDER "RayUtilities")
AddRecursiveFilters("${SRC}")
SetWorkingDirectory("RayMath" "")
SetOutputDirectory("RayUtil" "")