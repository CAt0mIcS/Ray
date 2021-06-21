#include "Rpch.h"
#include "RShaderGenerator.h"

#include "Components/RMeshRenderer.h"
#include "Nodes/RTexture2DNode.h"
#include "Nodes/RSampler2DNode.h"

#include "Utils/RString.h"
#include "Utils/RLogger.h"


namespace At0::Ray
{
	static std::string GetAttributes(const Node& rootNode, uint32_t& inputLocation,
		uint32_t& outputLocation, std::string_view attributes)
	{
		std::string newAttributes;

		auto attribs = rootNode.GetAttributes();
		for (const auto& [attribName, attribData] : attribs)
		{
			if (attributes.find(attribName) != std::string::npos)
				continue;

			if (attribData.inOut == "in")
				newAttributes += String::Serialize("layout(location = {0}) in {1} {2};\n",
					inputLocation++, attribData.type, attribName);
			else if (attribData.inOut == "out")
				newAttributes += String::Serialize("layout(location = {0}) out {1} {2};\n",
					outputLocation++, attribData.type, attribName);
		}

		return newAttributes;
	}

	static Ref<Texture2D> GetTexture(const Node& rootNode, std::string_view samplerName)
	{
		for (const auto& [connection, node] : rootNode)
		{
			if (Sampler2DNode* samplerNode = dynamic_cast<Sampler2DNode*>(node.node.get());
				samplerNode != nullptr && samplerNode->GetName() == samplerName)
			{
				return ((Texture2DNode*)samplerNode->GetChild(Sampler2DNode::Texture).node.get())
					->GetSharedTexture();
			}
			else if (Ref<Texture2D> tex = GetTexture(*node.node, samplerName))
			{
				return tex;
			}
		}

		Log::Warn("[ShaderGenerator] Failed to find texture for sampler \"{0}\"", samplerName);
		return nullptr;
	}

	std::string ShaderGenerator::GetUniforms(
		const Node& rootNode, std::string_view uniforms, ShaderStage stage)
	{
		std::string newUniforms;

		// Buffer uniforms
		{
			auto bufferUniforms = rootNode.GetBufferUniforms();
			for (const auto& [uBufferName, uniformData] : bufferUniforms)
			{
				if (uniforms.find(uBufferName) != std::string::npos)
					continue;

				std::string uniformsInBlock;

				for (const auto& [uniformType, uniformName] : uniformData)
					uniformsInBlock += '\n' + uniformType + ' ' + uniformName + ';';

				// Predefined set 0 for per scene data uniform
				if (uBufferName == "PerSceneData")
					newUniforms += String::Serialize("layout(set = 0, binding = 0) uniform "
													 "PerSceneData\n{{0}\n} uPerSceneData;\n",
						uniformsInBlock);
				else
				{
					newUniforms += String::Serialize(
						"layout(set = 1, binding = {0}) uniform {1}\n{{2}\n} u{1};\n",
						m_NextBinding++, uBufferName, uniformsInBlock);

					UniformData data{};
					data.stage = stage;
					data.name = uBufferName;
					data.type = UniformType::UniformBuffer;
					m_Uniforms.emplace_back(data);
				}
			}
		}

		// Sampler uniforms
		{
			auto samplerUniforms = rootNode.GetSamplerUniforms();
			for (const auto& [samplerName, samplerType] : samplerUniforms)
				if (uniforms.find(samplerName) == std::string::npos)
				{
					newUniforms +=
						String::Serialize("layout(set = 1, binding = {0}) uniform {1} {2};\n",
							m_NextBinding++, samplerType, samplerName);

					UniformData data{};
					data.stage = stage;
					data.name = samplerName;
					data.type = UniformType::CombinedImageSampler;
					data.texture = GetTexture(rootNode, samplerName);
					m_Uniforms.emplace_back(data);
				}
		}

		return newUniforms;
	}

	static std::string GetFunctions(const Node& rootNode, std::string_view functions)
	{
		std::string newFunctions;
		auto nodeFunctions = rootNode.GetFunctions();
		for (const std::string& fn : nodeFunctions)
			if (functions.find(fn) == std::string::npos)
				newFunctions += fn;
		return newFunctions;
	}

	std::string ShaderGenerator::GenerateVertexShader(std::vector<Ref<Node>> rootNodes)
	{
		std::string attributes;
		std::string uniforms;
		std::string functions;
		std::string mainCode;

		for (const Ref<Node>& rootNode : rootNodes)
		{
			attributes += GetAttributes(*rootNode, m_VSNextInputAttributeLocation,
				m_VSNextOutputAttributeLocation, attributes);
			uniforms += GetUniforms(*rootNode, uniforms, ShaderStage::Vertex);
			functions += GetFunctions(*rootNode, functions);
			mainCode += rootNode->GetFunctionCalls() + ";\n";
		}

		return String::Serialize(GenerateTemplate(), attributes, uniforms, functions, mainCode);
	}

	std::string ShaderGenerator::GenerateFragmentShader(std::vector<Ref<Node>> rootNodes)
	{
		std::string attributes;
		std::string uniforms;
		std::string functions;
		std::string mainCode;

		for (const Ref<Node>& rootNode : rootNodes)
		{
			attributes += GetAttributes(*rootNode, m_FSNextInputAttributeLocation,
				m_FSNextOutputAttributeLocation, attributes);
			uniforms += GetUniforms(*rootNode, uniforms, ShaderStage::Fragment);
			functions += GetFunctions(*rootNode, functions);
			mainCode += rootNode->GetFunctionCalls() + ";\n";
		}

		return String::Serialize(GenerateTemplate(), attributes, uniforms, functions, mainCode);
	}

	void ShaderGenerator::AddUniforms(MeshRenderer& renderer)
	{
		for (const UniformData& data : m_Uniforms)
		{
			if (data.type == UniformType::UniformBuffer)
			{
				if (data.name != UniformTag::PerObjectData)
					renderer.AddBufferUniform(data.name, data.stage);
			}
			else if (data.type == UniformType::CombinedImageSampler)
				renderer.AddSampler2DUniform(data.name, data.stage, data.texture);
		}
	}

	std::string ShaderGenerator::GenerateTemplate() const
	{
		return R"(#version 450 core
#extension GL_ARB_separate_shader_objects : enable

{0}

{1}

{2}

void main()
{
	{3}
}
)";
	}
}  // namespace At0::Ray
