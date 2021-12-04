#pragma once

#include "../RBase.h"
#include "../Graphics/Pipelines/Shader/RShaderDataTypes.h"
#include "../Core/RMath.h"

#include <unordered_map>
#include <utility>
#include <string>
#include <vector>


namespace At0::Ray
{
	class Texture2D;

	class RAY_EXPORT MaterialDataContainer
	{
	public:
		void Set(std::string key, Ref<Texture2D> image) { m_Images[key] = std::move(image); }

		void Set(const std::string& key, float data) { Set(key, data, ShaderDataType::Float); }
		void Set(const std::string& key, int32_t data) { Set(key, data, ShaderDataType::Int); }
		void Set(const std::string& key, uint32_t data) { Set(key, data, ShaderDataType::UInt); }
		void Set(const std::string& key, Float2 data) { Set(key, data, ShaderDataType::Vec2); }
		void Set(const std::string& key, Float3 data) { Set(key, data, ShaderDataType::Vec3); }
		void Set(const std::string& key, Float4 data) { Set(key, data, ShaderDataType::Vec4); }
		void Set(const std::string& key, Matrix3 data) { Set(key, data, ShaderDataType::Mat3); }
		void Set(const std::string& key, Matrix data) { Set(key, data, ShaderDataType::Mat4); }

		Ref<Texture2D> GetTexture(const std::string& key) const;
		void* Get(const std::string& key) const;
		ShaderDataType GetType(const std::string& key) const;

		bool HasUniformBlock(const std::string& name) const;
		bool HasUniform(const std::string& name) const;
		bool HasKey(const std::string& key) const;

	private:
		template<typename T>
		void Set(const std::string& key, T&& data, ShaderDataType type)
		{
			if (HasKey(key))
			{
				auto [offset, type] = m_OffsetMap[key];
				ValidateSizeRequirements(type, sizeof(data));
				memcpy(m_Data.data() + offset, &data, sizeof(data));
			}
			else
			{
				int prevOffset = m_LastOffset;
				m_OffsetMap[key] = std::make_pair(m_LastOffset, type);
				m_LastOffset += sizeof(data);
				m_Data.resize(m_LastOffset);
				memcpy(m_Data.data() + prevOffset, &data, sizeof(data));
			}
		}

		static void ValidateSizeRequirements(ShaderDataType type, uint32_t size);

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Images;
		std::unordered_map<std::string, std::pair<int, ShaderDataType>> m_OffsetMap;

		std::vector<char*> m_Data;

		int m_LastOffset = 0;
	};
}  // namespace At0::Ray
