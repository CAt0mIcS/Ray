#pragma once

#include "../../RBase.h"
#include "RShader.h"
#include "../Buffers/RUniformBufferSynchronizer.h"
#include "../../Utils/RAssert.h"

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
			UniformData(uint32_t offset) : m_OffsetInUniformBlock(offset) {}
			UniformData() : m_OffsetInUniformBlock((uint32_t)-1) {}

			template<typename T>
			UniformData& Update(T&& data, uint32_t globalOffset)
			{
				RAY_MEXPECTS(m_OffsetInUniformBlock != (uint32_t)-1,
					"[UniformData] Uniform does not exist.");

				UniformBufferSynchronizer::Get().Update(
					data, globalOffset + m_OffsetInUniformBlock);
				return *this;
			}

		private:
			uint32_t m_OffsetInUniformBlock = 0;
		};


		class UniformDataAccess
		{
		public:
			UniformDataAccess(
				const Shader::Uniforms* uniforms, const Shader::UniformBlocks* uniformBlocks)
				: m_Uniforms(uniforms), m_UniformBlocks(uniformBlocks)
			{
			}

			UniformData operator[](std::string_view uniformPath);

		private:
			const Shader::Uniforms* m_Uniforms;
			const Shader::UniformBlocks* m_UniformBlocks;
		};

	public:
		UniformAccess(const Pipeline& pipeline);

		UniformDataAccess Resolve(Shader::Stage stageFlag);

	private:
		const Shader& m_Shader;
	};
}  // namespace At0::Ray
