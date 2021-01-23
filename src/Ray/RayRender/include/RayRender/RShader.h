#pragma once

#include "RRBase.h"

#include <string_view>


namespace At0::Ray
{
	class RR_API Shader
	{
	public:
		static Scope<Shader> Create(std::string_view vertexFilepath, std::string_view pixelFilepath);

		virtual void Bind() = 0;

		virtual ~Shader() = default;

	protected:
		Shader() = default;
	};
}