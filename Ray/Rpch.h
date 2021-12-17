#pragma once


#include <mutex>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <exception>
#include <sstream>
#include <chrono>
#include <functional>
#include <memory>
#include <iomanip>
#include <fstream>
#include <limits>
#include <bitset>
#include <array>
#include <condition_variable>
#include <thread>
#include <utility>
#include <map>
#include <optional>
#include <numeric>
#include <filesystem>
#include <assert.h>
#include <concepts>
#include <future>
#include <type_traits>

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include "RBase.h"
#include "Core/RKeyCodes.h"
#include "Utils/RNonCopyable.h"
#include <vulkan/vulkan_core.h>
#include <../../Extern/Violent/include/Violent/Utils/VSerialize.h>

#ifdef _MSC_VER
	#include <source_location>
#else
	#include <experimental/source_location>
#endif
#include <string_view>
#include <../../Extern/Violent/include/Violent/Violent.h>

#include <assert.h>

#include "Utils/RString.h"
#include "Utils/RLogger.h"
#include "Utils/RAssert.h"
#include "Utils/RException.h"
