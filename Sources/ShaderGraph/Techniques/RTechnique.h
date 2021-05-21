#pragma once

#include "../../RBase.h"
#include "../../Graphics/Pipelines/RShader.h"


namespace At0::Ray
{
	/**
	 * Base class for all shading techniques for runtime shader generation/shader graph
	 */
	class RAY_EXPORT Technique
	{
	public:
		virtual ~Technique() = default;

		/**
		 * @returns Shader code which needs to be in the main shader function
		 */
		virtual std::string GetMainShaderCode() const = 0;


	protected:
		Technique() = default;
	};
}  // namespace At0::Ray
