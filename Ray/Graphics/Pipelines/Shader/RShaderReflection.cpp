#include "Rpch.h"
#include "RShaderReflection.h"

#include "Filesystem/RJSON.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"

using namespace rapidjson;


namespace At0::Ray
{
	ShaderReflection::ShaderReflection(const std::string& filepath)
	{
		JSON json(filepath);
		Document& doc = json.GetDocument();

		RAY_MEXPECTS(doc.HasMember("Attributes"),
			"[ShaderReflection] Invalid shader reflection file \"{0}\"", filepath);
		for (Value::ConstMemberIterator it = doc["Attributes"].MemberBegin();
			 it != doc["Attributes"].MemberEnd(); ++it)
		{
			AttributeData data = LoadAttribute(it->value);
			data.name = it->name.GetString();
			AddAttribute(data);
		}

		RAY_MEXPECTS(doc.HasMember("UniformBlocks"),
			"[ShaderReflection] Invalid shader reflection file \"{0}\"", filepath);
		for (Value::ConstMemberIterator it = doc["UniformBlocks"].MemberBegin();
			 it != doc["UniformBlocks"].MemberEnd(); ++it)
		{
			UniformBlockData data = LoadUniformBlock(it->value);
			data.name = it->name.GetString();
			AddUniformBlock(data);
		}

		RAY_MEXPECTS(doc.HasMember("Uniforms"),
			"[ShaderReflection] Invalid shader reflection file \"{0}\"", filepath);
		for (Value::ConstMemberIterator it = doc["Uniforms"].MemberBegin();
			 it != doc["Uniforms"].MemberEnd(); ++it)
		{
			UniformData data = LoadUniform(it->value);
			data.name = it->name.GetString();
			AddUniform(data);
		}
	}

	void ShaderReflection::AddUniformBlock(UniformBlockData data)
	{
		m_UniformBlocks.emplace_back(std::move(data));
	}

	bool ShaderReflection::HasAttribute(std::string_view name) const
	{
		return std::find_if(m_Attributes.begin(), m_Attributes.end(),
				   [&name](const AttributeData& data) { return data.name == name; }) !=
			   m_Attributes.end();
	}

	bool ShaderReflection::HasUniform(std::string_view name, bool includeUniformBlocks) const
	{
		if (includeUniformBlocks)
			for (const auto& uBlockData : m_UniformBlocks)
				for (const auto& uData : uBlockData.uniforms)
					if (uData.name == name)
						return true;

		return std::find_if(m_Uniforms.begin(), m_Uniforms.end(), [&name](const UniformData& data) {
			return data.name == name;
		}) != m_Uniforms.end();
	}

	bool ShaderReflection::HasUniformBlock(std::string_view name) const
	{
		return std::find_if(m_UniformBlocks.begin(), m_UniformBlocks.end(),
				   [&name](const UniformBlockData& data) { return data.name == name; }) !=
			   m_UniformBlocks.end();
	}

	const ShaderReflection::AttributeData& ShaderReflection::GetAttribute(
		std::string_view name) const
	{
		for (const auto& attrib : m_Attributes)
			if (attrib.name == name)
				return attrib;

		ThrowRuntime("[ShaderReflection] Failed to get attribute with name \"{0}\"", name);
		return m_Attributes[0];
	}

	const ShaderReflection::UniformData& ShaderReflection::GetUniform(std::string_view name) const
	{
		for (const auto& uniform : m_Uniforms)
			if (uniform.name == name)
				return uniform;

		ThrowRuntime("[ShaderReflection] Failed to get uniform with name \"{0}\"", name);
		return m_Uniforms[0];
	}

	const ShaderReflection::UniformBlockData& ShaderReflection::GetUniformBlock(
		std::string_view name) const
	{
		for (const auto& uniformBlock : m_UniformBlocks)
			if (uniformBlock.name == name)
				return uniformBlock;

		ThrowRuntime("[ShaderReflection] Failed to get uniform block with name \"{0}\"", name);
		return m_UniformBlocks[0];
	}

	ShaderReflection::AttributeData& ShaderReflection::GetAttribute(std::string_view name)
	{
		return const_cast<AttributeData&>(std::as_const(*this).GetAttribute(name));
	}

	ShaderReflection::UniformData& ShaderReflection::GetUniform(std::string_view name)
	{
		return const_cast<UniformData&>(std::as_const(*this).GetUniform(name));
	}

	ShaderReflection::UniformBlockData& ShaderReflection::GetUniformBlock(std::string_view name)
	{
		return const_cast<UniformBlockData&>(std::as_const(*this).GetUniformBlock(name));
	}


	static Value SerializeAttribute(
		const ShaderReflection::AttributeData& data, MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		Value attrib{ kObjectType };
		attrib.AddMember("location", data.location, allocator);
		attrib.AddMember("size", data.size, allocator);
		attrib.AddMember("format", (uint32_t)data.format, allocator);
		return attrib;
	}

	static Value SerializeUniform(
		const ShaderReflection::UniformData& data, MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		Value uniform{ kObjectType };
		uniform.AddMember("binding", data.binding, allocator);
		uniform.AddMember("offset", data.offset, allocator);
		uniform.AddMember("size", data.size, allocator);
		uniform.AddMember("type", (uint32_t)data.type, allocator);
		uniform.AddMember("set", data.set, allocator);
		return uniform;
	}

	static Value SerializeUniformBlock(const ShaderReflection::UniformBlockData& data,
		MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		Value uniformBlock(kObjectType);
		uniformBlock.AddMember("binding", data.binding, allocator);
		uniformBlock.AddMember("size", data.size, allocator);

		Value uniforms(kObjectType);
		for (const auto& uData : data.uniforms)
			uniforms.AddMember(
				StringRef(uData.name.c_str()), SerializeUniform(uData, allocator), allocator);

		uniformBlock.AddMember("uniforms", uniforms, allocator);
		uniformBlock.AddMember("type", (uint32_t)data.type, allocator);
		uniformBlock.AddMember("set", data.set, allocator);
		return uniformBlock;
	}

	void ShaderReflection::WriteToFile(const std::string& filepath) const
	{
		using namespace rapidjson;

		JSON json;
		Document& doc = json.GetDocument();

		Value attributes{ kObjectType };
		for (const auto& attribute : m_Attributes)
		{
			attributes.AddMember(StringRef(attribute.name.c_str()),
				SerializeAttribute(attribute, doc.GetAllocator()), doc.GetAllocator());
		}
		doc.AddMember("Attributes", attributes, doc.GetAllocator());

		Value uniformBlocks{ kObjectType };
		for (const auto& uBlockData : m_UniformBlocks)
		{
			uniformBlocks.AddMember(StringRef(uBlockData.name.c_str()),
				SerializeUniformBlock(uBlockData, doc.GetAllocator()), doc.GetAllocator());
		}
		doc.AddMember("UniformBlocks", uniformBlocks, doc.GetAllocator());

		Value uniforms{ kObjectType };
		for (const auto& uData : m_Uniforms)
		{
			uniforms.AddMember(StringRef(uData.name.c_str()),
				SerializeUniform(uData, doc.GetAllocator()), doc.GetAllocator());
		}
		doc.AddMember("Uniforms", uniforms, doc.GetAllocator());

		json.Write(filepath);
	}

	ShaderReflection::AttributeData ShaderReflection::LoadAttribute(const rapidjson::Value& data)
	{
		AttributeData attribData{};
		attribData.location = data["location"].GetUint();
		attribData.size = data["size"].GetUint();
		attribData.format = (VkFormat)data["format"].GetUint();
		return attribData;
	}

	ShaderReflection::UniformBlockData ShaderReflection::LoadUniformBlock(
		const rapidjson::Value& data)
	{
		UniformBlockData uniformData{};
		uniformData.binding = data["binding"].GetUint();
		uniformData.set = data["set"].GetUint();
		uniformData.size = data["size"].GetUint();
		uniformData.type = (UniformType)data["type"].GetUint();

		RAY_MEXPECTS(data.HasMember("uniforms"),
			"[ShaderReflection] UniformBlocks::uniforms member missing in shader reflection");
		for (Value::ConstMemberIterator it = data["uniforms"].MemberBegin();
			 it != data["uniforms"].MemberEnd(); ++it)
		{
			UniformData uData = LoadUniform(it->value);
			uData.name = it->name.GetString();
			uniformData.uniforms.emplace_back(uData);
		}

		return uniformData;
	}

	ShaderReflection::UniformData ShaderReflection::LoadUniform(const rapidjson::Value& data)
	{
		UniformData uniformData{};
		uniformData.binding = data["binding"].GetUint();
		uniformData.offset = data["offset"].GetUint();
		uniformData.set = data["set"].GetUint();
		uniformData.size = data["size"].GetUint();
		uniformData.type = (UniformType)data["type"].GetUint();
		return uniformData;
	}

	const ShaderReflection::UniformData& ShaderReflection::UniformBlockData::GetUniform(
		std::string_view name) const
	{
		for (const auto& uniform : uniforms)
			if (uniform.name == name)
				return uniform;

		ThrowRuntime("[ShaderReflection] Failed to get uniform with name \"{0}\"", name);
		return uniforms[0];
	}

	ShaderReflection::UniformData& ShaderReflection::UniformBlockData::GetUniform(
		std::string_view name)
	{
		return const_cast<UniformData&>(std::as_const(*this).GetUniform(name));
	}
}  // namespace At0::Ray
