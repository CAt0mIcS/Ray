#pragma once

#include "RRBase.h"
#include <string_view>


namespace At0::Ray
{
	class RR_API PixelShader
	{
	public:
		static Scope<PixelShader> CreateFromCompiled(std::string_view filepath);
		static Scope<PixelShader> CreateFromSource(std::string_view filepath);

		virtual void Bind() = 0;

		virtual ~PixelShader() = default;
	};
}