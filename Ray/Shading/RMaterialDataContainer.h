#pragma once

#include "../RBase.h"
#include "../Graphics/Pipelines/Shader/RShaderTypes.h"
#include "../Core/RMath.h"
#include "../Utils/RTypeTraits.h"

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
		void Set(const std::string& key, Ref<Texture2D> image) { m_Images[key] = std::move(image); }
		void Set(const std::string& key, bool data);
		void Set(const std::string& key, Bool2 data);
		void Set(const std::string& key, Bool3 data);
		void Set(const std::string& key, Bool4 data);

		template<typename T>
		void Set(const std::string& key, T&& data)
		{
			Set(key, std::forward<T>(data), GetType<T>());
		}

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

		template<typename T>
		static constexpr ShaderDataType GetType()
		{
			if constexpr (IsSameType<T, float>)
				return ShaderDataType::Float;
			else if constexpr (IsSameType<T, Float2>)
				return ShaderDataType::Vec2;
			else if constexpr (IsSameType<T, Float3>)
				return ShaderDataType::Vec3;
			else if constexpr (IsSameType<T, Float4>)
				return ShaderDataType::Vec4;

			else if constexpr (IsSameType<T, int32_t>)
				return ShaderDataType::Int;
			else if constexpr (IsSameType<T, Int2>)
				return ShaderDataType::IVec2;
			else if constexpr (IsSameType<T, Int3>)
				return ShaderDataType::IVec3;
			else if constexpr (IsSameType<T, Int4>)
				return ShaderDataType::IVec4;

			else if constexpr (IsSameType<T, uint32_t>)
				return ShaderDataType::UInt;
			else if constexpr (IsSameType<T, UInt2>)
				return ShaderDataType::UVec2;
			else if constexpr (IsSameType<T, UInt3>)
				return ShaderDataType::UVec3;
			else if constexpr (IsSameType<T, UInt4>)
				return ShaderDataType::UVec4;

			else if constexpr (IsSameType<T, bool>)
				return ShaderDataType::Bool;
			else if constexpr (IsSameType<T, Bool2>)
				return ShaderDataType::BVec2;
			else if constexpr (IsSameType<T, Bool3>)
				return ShaderDataType::BVec3;
			else if constexpr (IsSameType<T, Bool4>)
				return ShaderDataType::BVec4;

			else if constexpr (IsSameType<T, double>)
				return ShaderDataType::Double;
			else if constexpr (IsSameType<T, Double2>)
				return ShaderDataType::DVec2;
			else if constexpr (IsSameType<T, Double3>)
				return ShaderDataType::DVec3;
			else if constexpr (IsSameType<T, Double4>)
				return ShaderDataType::DVec4;

			else if constexpr (IsSameType<T, Matrix2>)
				return ShaderDataType::Mat2;
			else if constexpr (IsSameType<T, Matrix2x3>)
				return ShaderDataType::Mat2x3;
			else if constexpr (IsSameType<T, Matrix2x4>)
				return ShaderDataType::Mat2x4;

			else if constexpr (IsSameType<T, Matrix3>)
				return ShaderDataType::Mat3;
			else if constexpr (IsSameType<T, Matrix3x2>)
				return ShaderDataType::Mat3x2;
			else if constexpr (IsSameType<T, Matrix3x4>)
				return ShaderDataType::Mat3x4;

			else if constexpr (IsSameType<T, Matrix4>)
				return ShaderDataType::Mat4;
			else if constexpr (IsSameType<T, Matrix4x2>)
				return ShaderDataType::Mat4x2;
			else if constexpr (IsSameType<T, Matrix4x3>)
				return ShaderDataType::Mat4x3;

			else if constexpr (IsSameType<T, DMatrix2>)
				return ShaderDataType::DMat2;
			else if constexpr (IsSameType<T, DMatrix2x3>)
				return ShaderDataType::DMat2x3;
			else if constexpr (IsSameType<T, DMatrix2x4>)
				return ShaderDataType::DMat2x4;

			else if constexpr (IsSameType<T, DMatrix3>)
				return ShaderDataType::DMat3;
			else if constexpr (IsSameType<T, DMatrix3x2>)
				return ShaderDataType::DMat3x2;
			else if constexpr (IsSameType<T, DMatrix3x4>)
				return ShaderDataType::DMat3x4;

			else if constexpr (IsSameType<T, DMatrix4>)
				return ShaderDataType::DMat4;
			else if constexpr (IsSameType<T, DMatrix4x2>)
				return ShaderDataType::DMat4x2;
			else if constexpr (IsSameType<T, DMatrix4x3>)
				return ShaderDataType::DMat4x3;

			return (ShaderDataType)0;
		}

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Images;
		std::unordered_map<std::string, std::pair<int, ShaderDataType>> m_OffsetMap;

		std::vector<char*> m_Data;

		int m_LastOffset = 0;
	};
}  // namespace At0::Ray
