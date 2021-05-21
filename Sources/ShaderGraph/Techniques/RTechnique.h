#pragma once

#include "../../RBase.h"
#include "../../Utils/RNonCopyable.h"
#include "../../Graphics/Pipelines/RShader.h"

#include <unordered_map>


namespace At0::Ray
{
	/**
	 * Base class for all shading techniques for runtime shader generation/shader graph
	 */
	class RAY_EXPORT Technique : NonCopyable
	{
	public:
		virtual ~Technique() = default;

		/**
		 * @returns All functions which may be called by this technique
		 */
		virtual std::string GetFunctions() const = 0;
		virtual std::string GetFunctionCalls() const = 0;

		void Connect(uint32_t pointID, Scope<Technique> tech);
		void Disconnect(uint32_t pointID) { m_ChildTechniques.erase(pointID); }

	protected:
		Technique() = default;

	protected:
		// Maps connection point id to technique connected to that point
		std::unordered_map<uint32_t, Scope<Technique>> m_ChildTechniques;
	};
}  // namespace At0::Ray
