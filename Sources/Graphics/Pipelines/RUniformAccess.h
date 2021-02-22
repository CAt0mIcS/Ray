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
			uint32_t m_GlobalOffset = 0;
		};

	public:
		UniformAccess(const Pipeline& pipeline);

		UniformData Resolve(Shader::Stage stageFlag, std::string_view uniformName,
			std::string_view uniformBlockName = "");

	private:
		const Shader& m_Shader;
	};
}  // namespace At0::Ray
