#pragma once

#include <sdkddkver.h>

#define NOATOM
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX

#include <Windows.h>
#include <dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include <wincodec.h>
#include <ShObjIdl.h>

