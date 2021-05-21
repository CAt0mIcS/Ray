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
		 * @retursn All input attributes needed by this technique
		 */
		virtual std::string GetInputAttributes() const = 0;

		/**
		 * @returns All uniforms needed by this technique
		 */
		virtual std::string GetUniforms() const = 0;

		/**
		 * @returns Global variables needed by this technique
		 */
		virtual std::string GetVariables() const = 0;

		/**
		 * @returns All functions which may be called by this technique
		 */
		virtual std::string GetFunctions() const = 0;

		/**
		 * @returns Shader code which needs to be in the main shader function
		 */
		virtual std::string GetMainShaderCode() const = 0;

	protected:
		Technique() = default;
	};
}  // namespace At0::Ray
