#ifndef RLR_TYPEDEFINES_H
#define RLR_TYPEDEFINES_H

#include <../../RlUtilities/include/RlUtil/PlatformDetection.h>

#ifdef RL_PLATFORM_WINDOWS
	#include <DirectXMath.h>
	namespace At0::Reyal
	{
		using WindowHandle = HWND;
		using Matrix = DirectX::XMMATRIX;
	}
#endif

#endif // RLR_TYPEDEFINES_H
