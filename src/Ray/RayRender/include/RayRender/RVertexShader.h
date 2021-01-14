#pragma once

#include "RRBase.h"
#include <string_view>


namespace At0::Ray
{
	class RR_API VertexShader
	{
	public:
		static Ref<VertexShader> CreateFromCompiled(std::string_view filepath);
		static Ref<VertexShader> CreateFromSource(std::string_view filepath);

		virtual ~VertexShader() = default;
	};
}