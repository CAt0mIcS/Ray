#pragma once

// class needs to have dll-interface to be used by clients of class
#pragma warning(disable : 4251)

#include "RlBase.h"
#ifdef _WIN32
	#include <Windows.h>
#endif

#include <deque>
#include <queue>
#include <exception>
#include <string>
#include <mutex>
#include <vector>
#include <sstream>
#include <chrono>
#include <functional>
#include <memory>
#include <fstream>
#include <bitset>
#include <condition_variable>
#include <thread>

#include <assert.h>


