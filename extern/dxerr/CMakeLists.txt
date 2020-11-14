set(SRC
    "include/DxErr/dxerr.h"
    "include/DxErr/dxerr.cpp"
    "include/DxErr/DXGetErrorDescription.inl"
    "include/DxErr/DXGetErrorString.inl"
    "include/DxErr/DXTrace.inl"
)

add_library("DxErr" STATIC "${SRC}")
AddRecursiveFilters("${SRC}")
set_target_properties("DxErr" PROPERTIES FOLDER "extern")