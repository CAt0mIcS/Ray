#pragma once

#include "RRBase.h"
#include <string_view>


namespace At0::Ray
{
	class RR_API PixelShader
	{
	public:
		static Ref<PixelShader> CreateFromCompiled(std::string_view filepath);
		static Ref<PixelShader> CreateFromSource(std::string_view filepath);

		virtual void Bind() = 0;

		virtual ~PixelShader() = default;
	};
}