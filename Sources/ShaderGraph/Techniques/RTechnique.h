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
		virtual std::string GetInputAttributes() const;
		virtual std::string GetFunctions() const;
		virtual std::string GetFunctionCalls() const;
		virtual std::string GetBufferUniforms() const;
		virtual std::string GetSamplerUniforms() const;

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

		bool HasAttribute(const std::string& attribName) const;
		bool HasUniformInBlock(
			const std::string& uniformBlockName, const std::string& uniformName) const;
		bool HasSampler2DUniform(const std::string& uniformName) const;

		bool HasVertexInputAttribute(const std::string& attribName) const;
		bool HasVertexOutputAttribute(const std::string& attribName) const;
		bool HasVertexAsignment(
			const std::string& outAttribName, const std::string& inAttribName) const;

		void RequiresAttribute(std::string_view attribType, const std::string& attribName);
		void RequiresBufferUniform(const std::string& uniformBlockName,
			std::string_view uniformType, const std::string& uniformName);
		void RequiresSampler2DUniform(std::string_view uniformName);

		void RequiresVertexInputAttribute(
			std::string_view attribType, const std::string& attribName);
		void RequiresVertexOutputAttribute(
			std::string_view attribType, const std::string& attribName);
		void RequiresVertexAssignment(
			const std::string& outAttribName, const std::string& inAttribName);

		std::string MergeAttributes(uint32_t& location) const;
		std::string MergeUniforms(uint32_t& binding) const;

		std::string MergeVertexAttributes(
			uint32_t& vertexInputLocation, uint32_t& vertexOutputLocation) const;
		std::string MergeVertexAssignments() const;

	protected:
		Technique() = default;

	protected:
		// Maps connection point id to technique connected to that point
		std::unordered_map<uint32_t, Scope<Technique>> m_ChildTechniques;

		// Attribute name --> attribute type
		std::unordered_map<std::string, std::string> m_Attributes;

		// Uniform block name --> [uniform name in block --> type]
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
			m_BufferUniforms;

		// Sampler names
		std::vector<std::string> m_Sampler2DUniforms;

		// (VertexShader): Attribute name --> attribute type
		std::unordered_map<std::string, std::string> m_VertexInputAttributes;
		std::unordered_map<std::string, std::string> m_VertexOutputAttributes;

		// outAttribute --> inAttribute
		std::unordered_map<std::string, std::string> m_VertexAssignments;
	};
}  // namespace At0::Ray
