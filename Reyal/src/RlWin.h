#ifndef RL_RLWIN_H
#define RL_RLWIN_H


#include <sdkddkver.h>

#define NOATOM
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX

#ifdef _WIN32
	#include <Windows.h>
	#include <d2d1.h>

	#include <d3d11.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
#else
	#error "Only Windows is currently supported!"
#endif


#endif /* RL_RLWIN_H */