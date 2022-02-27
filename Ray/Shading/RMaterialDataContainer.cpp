#include "RMaterialDataContainer.h"


namespace At0::Ray
{
	void* MaterialDataContainer::Get(const std::string& key) const
	{
		if (m_OffsetMap.find(key) == m_OffsetMap.end())
			return nullptr;

		return (void*)(m_Data.data() + m_OffsetMap.at(key).first);
	}

	ShaderDataType MaterialDataContainer::GetType(const std::string& key) const
	{
		if (m_OffsetMap.find(key) == m_OffsetMap.end())
			return (ShaderDataType)0;
		return m_OffsetMap.at(key).second;
	}

	void MaterialDataContainer::Set(const std::string& key, bool data)
	{
		Set<bool, GetType<bool>()>(key, (VkBool32)data);
	}

	void MaterialDataContainer::Set(const std::string& key, Bool2 data)
	{
		Set<Bool2, GetType<Bool2>()>(key, UIBool2{ data });
	}

	void MaterialDataContainer::Set(const std::string& key, Bool3 data)
	{
		Set<UIBool3, GetType<Bool3>()>(key, UIBool3{ data });
	}

	void MaterialDataContainer::Set(const std::string& key, Bool4 data)
	{
		Set<UIBool4, GetType<Bool4>()>(key, UIBool4{ data });
	}

	Ref<Texture> MaterialDataContainer::GetTexture(const std::string& key) const
	{
		if (m_Images.find(key) == m_Images.end())
			return nullptr;
		return m_Images.at(key);
	}

	bool MaterialDataContainer::HasUniformBlock(const std::string& name) const
	{
		for (const auto& [key, offset] : m_OffsetMap)
		{
			int pos = key.find('.');
			if (pos == std::string::npos)
				return false;
			std::string uBlockName = key.substr(0, pos);
			if (uBlockName == name)
				return true;
		}
		return false;
	}

	bool MaterialDataContainer::HasUniform(const std::string& name) const
	{
		if (m_Images.find(name) != m_Images.end())
			return true;

		for (const auto& [key, offset] : m_OffsetMap)
		{
			if (key == name)
				return true;

			int pos = key.find('.');
			std::string uniformName = key.substr(pos + 1);
			if (uniformName == name)
				return true;
		}
		return false;
	}

	bool MaterialDataContainer::HasKey(const std::string& key) const
	{
		return m_OffsetMap.find(key) != m_OffsetMap.end();
	}

	void MaterialDataContainer::ValidateSizeRequirements(ShaderDataType type, uint32_t size)
	{
		RAY_MEXPECTS(GetShaderDataTypeSize(type) == size,
			"[MaterialDataContainer] Trying to add item of size {0} byte(s) to ShaderDataType "
			"\"{1}\" with size {2} byte(s)",
			size, String::Construct(type), GetShaderDataTypeSize(type));
	}
}  // namespace At0::Ray
