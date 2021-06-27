#include "Rpch.h"
#include "RCore.h"


bool (*RrpfnValidationCallback)(RrLogMessageSeverity, const char*) = NULL;
