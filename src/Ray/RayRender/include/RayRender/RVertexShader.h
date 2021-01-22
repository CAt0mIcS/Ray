#pragma once

#include "RRBase.h"
#include <string_view>


namespace At0::Ray
{
	class RR_API VertexShader
	{
	public:
		static Scope<VertexShader> CreateFromCompiled(std::string_view filepath);
		static Scope<VertexShader> CreateFromSource(std::string_view filepath);

		virtual void Bind() = 0;

		virtual ~VertexShader() = default;

	protected:
		VertexShader() = default;
	};
}