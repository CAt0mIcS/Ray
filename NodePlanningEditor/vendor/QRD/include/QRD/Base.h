#pragma once

#define QRD_DYNAMIC_LIBRARY 0

#if QRD_DYNAMIC_LIBRARY
	#ifdef DBMS_BUILD
		#define QRD_API __declspec(dllexport)
	#else
		#define QRD_API __declspec(dllimport)
	#endif
#else
	#define QRD_API
#endif

/**
* Disable STL warning: class needs to have dll-interface to be used by client class
*/
#pragma warning(disable : 4251)


/**
* Set to 0 to disable logging or timing
*/
#define QRD_LOG_ACTIVE 0
#define QRD_TIMER_ACTIVE 0

#ifdef _DEBUG
	#define QRD_ASSERT(x) if(!(x)) __debugbreak();
#else
	#define QRD_ASSERT(x)
#endif

namespace QRD
{
	/**
	* SQL DbTypes
	*/
	enum DbTypes
	{
		NONE = 0, TEXT, NUMBER
	};

}

