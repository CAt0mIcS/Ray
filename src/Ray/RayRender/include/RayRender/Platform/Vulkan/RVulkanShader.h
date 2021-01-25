#pragma once

#include "../../RShader.h"


namespace At0::Ray
{
	class RR_API VulkanShader : public Shader
	{
	public:
		VulkanShader(std::string_view vertexFilepath, std::string_view pixelFilepath);

		virtual void Bind() override;
	};
}