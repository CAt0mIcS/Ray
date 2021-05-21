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
		 * @returns All functions which may be called by this technique
		 */
		virtual std::string GetFunctions() const = 0;
		virtual std::string GetFunctionCalls() const = 0;

	protected:
		Technique() = default;
	};
}  // namespace At0::Ray
