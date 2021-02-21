#pragma once

#include "../../RBase.h"
#include "RShader.h"
#include "../Buffers/RUniformBufferSynchronizer.h"

#include <unordered_map>
#include <vector>


namespace At0::Ray
{
	class Pipeline;

	class RAY_EXPORT UniformAccess
	{
	public:
		class UniformData
		{
		public:
			UniformData(uint32_t globalOffset) : m_GlobalOffset(globalOffset) {}
			UniformData() : m_GlobalOffset((uint32_t)-1) {}

			template<typename T>
			UniformData& operator=(T&& data)
			{
				RAY_MEXPECTS(m_GlobalOffset != (uint32_t)-1,
					"[UniformAccess::UniformData] Uniform does not exist.");

				UniformBufferSynchronizer::Get().Update(data, m_GlobalOffset);
				return *this;
			}

		private:
			uint32_t m_GlobalOffset;
		};

	private:
		using ValueType =
			std::unordered_map<std::string, std::unordered_map<std::string, UniformData>>;

	public:
		UniformAccess(const Pipeline& pipeline);

		ValueType& Resolve(Shader::Stage stageFlag);

	private:
		std::unordered_map<Shader::Stage, ValueType> m_Uniforms;
	};
}  // namespace At0::Ray
