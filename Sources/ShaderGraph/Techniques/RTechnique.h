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
		virtual std::string GetInputAttributes() const { return ""; }
		virtual std::string GetFunctions() const { return ""; }
		virtual std::string GetFunctionCalls() const { return ""; }
		virtual std::string GetBufferUniforms() const { return ""; }
		virtual std::string GetSamplerUniforms() const { return ""; }

		void Connect(uint32_t pointID, Scope<Technique> tech);
		void Disconnect(uint32_t pointID) { m_ChildTechniques.erase(pointID); }

		auto& GetTechniques() const { return m_ChildTechniques; }
		auto& GetTechniques() { return m_ChildTechniques; }

		template<typename T = Technique>
		T& GetTechnique(uint32_t connectionPoint)
		{
			return *(T*)m_ChildTechniques.at(connectionPoint).get();
		}

		template<typename T = Technique>
		const T& GetTechnique(uint32_t connectionPoint) const
		{
			return *(T*)m_ChildTechniques.at(connectionPoint).get();
		}

	protected:
		Technique() = default;

	protected:
		// Maps connection point id to technique connected to that point
		std::unordered_map<uint32_t, Scope<Technique>> m_ChildTechniques;
	};
}  // namespace At0::Ray
