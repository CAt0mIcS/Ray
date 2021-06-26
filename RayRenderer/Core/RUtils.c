#include "Rpch.h"
#include "RUtils.h"


bool LogDebug(const char* message)
{
	if (RrpfnValidationCallback)
		return RrpfnValidationCallback(RrLogMessageSeverityDebug, message);
	return false;
}

bool LogInfo(const char* message)
{
	if (RrpfnValidationCallback)
		return RrpfnValidationCallback(RrLogMessageSeverityInfo, message);
	return false;
}

bool LogWarning(const char* message)
{
	if (RrpfnValidationCallback)
		return RrpfnValidationCallback(RrLogMessageSeverityWarning, message);
	return false;
}

bool LogError(const char* message)
{
	if (RrpfnValidationCallback)
		return RrpfnValidationCallback(RrLogMessageSeverityError, message);
	return false;
}
