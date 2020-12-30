#pragma once

#include "RUBase.h"

#include "RGlobalDefines.h"

#include <signal.h>
#include <string>


namespace At0::Ray::Util
{
	template<typename F>
	static void SetSignals(F&& func)
	{
		signal(SIGABRT, func);
		signal(SIGFPE, func);
		signal(SIGILL, func);
		signal(SIGINT, func);
		signal(SIGSEGV, func);
		signal(SIGTERM, func);
	}

	/// <summary>
	/// Allocates the console and opens std::cout, std::clog, std::cerr, std::cin, std::wcout, std::wclog, std::wcerr, std::wcin streams
	/// </summary>
	void RU_API AllocateConsole();

	/// <summary>
	/// Frees the console
	/// </summary>
	void RU_API DeallocateConsole();

	/// <summary>
	/// Checks if a string ends with another string
	/// </summary>
	/// <param name="str">Is the full string to check</param>
	/// <param name="end">Is the supposed end of the full string</param>
	/// <returns>True if str ends with end, false otherwise</returns>
	bool RU_API EndsWith(std::string_view str, std::string_view end);

	/// <summary>
	/// Transforms coordinates between pixel space(Default) and NDC Space(DirectX/OpenGL)
	/// </summary>
	template<typename Vertex>
	class CoordinateTransformer
	{
	public:
		CoordinateTransformer(const Size2& windowSize)
			: m_WindowSize(windowSize) {}

		Vertex ToNormalizedDeviceCoordinate(const Vertex& pixelCoord)
		{
			Vertex v{};
			v.x = (pixelCoord.x / m_WindowSize.x) * 2.0f - 1.0f;
			v.y = -((pixelCoord.y / m_WindowSize.y) * 2.0f - 1.0f);

			return v;
		}

		Vertex ToPixelCoordinate(const Vertex& ndcCoord)
		{
			Vertex v{};
			v.x = (ndcCoord.x + 1) * (m_WindowSize.x / 2.0f);
			v.y = -((ndcCoord.y - 1) * (m_WindowSize.y / 2.0f));

			return v;
		}

		void ToNormalizedDeviceCoordinate(const Vertex& pixelCoord, Vertex& ndcCoord)
		{
			ndcCoord = ToNormalizedDeviceCoordinate(pixelCoord);
		}

		void ToPixelCoordinate(const Vertex& ndcCoord, Vertex& pixelCoord)
		{
			pixelCoord = ToPixelCoordinate(ndcCoord);
		}

	private:
		const Size2& m_WindowSize;
	};

}


