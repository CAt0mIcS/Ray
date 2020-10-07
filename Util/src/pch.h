#pragma once

/**
* class needs to have dll-interface to be used by clients of class 
*/
#pragma warning(disable : 4251)


#include "Win.h"

#include <sstream>
#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <fstream>

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"

