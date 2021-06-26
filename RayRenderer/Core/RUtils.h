#pragma once

#include "RCore.h"


RR_EXTERN_C_BEG

RrError RrGetError(int code);

bool LogDebug(const char* message);
bool LogInfo(const char* message);
bool LogWarning(const char* message);
bool LogError(const char* message);

RR_EXTERN_C_END
