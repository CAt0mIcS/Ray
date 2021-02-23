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

			uint32_t GetOffset() const { return m_GlobalOffset; }

		private:
			uint32_t m_GlobalOffset = 0;
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
