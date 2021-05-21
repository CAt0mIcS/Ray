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

		bool HasAttribute(std::string_view attribName) const;
		bool HasUniformInBlock(
			std::string_view uniformBlockName, std::string_view uniformName) const;
		bool HasSampler2DUniform(std::string_view uniformName);

		void RequiresAttribute(std::string_view attribType, std::string_view attribName);
		void RequiresBufferUniform(std::string_view uniformBlockName, std::string_view uniformName);
		void RequiresSampler2DUniform(std::string_view uniformName);

		std::string MergeAttributes(uint32_t& location) const;
		std::string MergeUniforms(uint32_t& binding) const;

	protected:
		Technique() = default;

	protected:
		// Maps connection point id to technique connected to that point
		std::unordered_map<uint32_t, Scope<Technique>> m_ChildTechniques;

		// Attribute name --> attribute type
		std::unordered_map<std::string, std::string> m_Attributes;

		// Uniform block name --> uniform names in block
		std::unordered_map<std::string, std::vector<std::string>> m_BufferUniforms;

		// Sampler names
		std::vector<std::string> m_Sampler2DUniforms;
	};
}  // namespace At0::Ray
