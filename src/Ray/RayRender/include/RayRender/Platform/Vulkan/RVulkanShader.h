#pragma once

#include "../../RShader.h"
#include "Internal/RVulkanGraphicsResources.h"


namespace At0::Ray
{
	class RR_API VulkanShader : public Shader, VulkanGraphicsResources
	{
	public:
		VulkanShader(std::string_view vertexFilepath, std::string_view pixelFilepath);

		virtual void Bind() override;
	};
}