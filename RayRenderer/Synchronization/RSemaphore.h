#pragma once

#include "../Core/RCore.h"

RR_DEFINE_HANDLE(RrSemaphore);
RR_DEFINE_HANDLE(RrLogicalDevice);


RR_API RrError RrCreateSemaphore(RrLogicalDevice device, RrSemaphore* pSemaphore);

RR_API void RrDestroySemaphore(RrLogicalDevice device, RrSemaphore semaphore);
