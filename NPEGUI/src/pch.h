#pragma once

/**
* class needs to have dll-interface to be used by clients of class
*/
#pragma warning(disable : 4251)

#include "Win.h"

#include <string>
#include <vector>
#include <optional>
#include <sstream>

#include <wrl.h>
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

//TODO: Sort out necessary
#pragma comment(lib,"comctl32.lib") 
#pragma comment(lib,"propsys.lib")
#pragma comment(lib,"shlwapi.lib") 