#pragma once

#include "../../RBase.h"
#include "RShader.h"
#include "../Buffers/RBufferSynchronizer.h"
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
			UniformData(uint32_t offset, uint32_t globalOffset) : m_Offset(offset + globalOffset) {}
			UniformData() : m_Offset((uint32_t)-1) {}

			template<typename T>
			UniformData& Update(T&& data)
			{
				RAY_MEXPECTS(m_Offset != (uint32_t)-1, "[UniformData] Uniform does not exist.");

				BufferSynchronizer::Get().Update(data, m_Offset);
				return *this;
			}

		private:
			uint32_t m_Offset = 0;
		};


		class UniformDataAccess
		{
		public:
			UniformDataAccess(const Shader::Uniforms* uniforms,
				const Shader::UniformBlocks* uniformBlocks,
				std::pair<std::string, const Shader::UniformBlocks::UniformBlockData*>& blockCache,
				uint32_t globalOffset)
				: m_Uniforms(uniforms), m_UniformBlocks(uniformBlocks), m_BlockCache(blockCache),
				  m_GlobalOffset(globalOffset)
			{
			}

			UniformData operator()(std::string_view uniformBlockName, std::string_view uniformName);
			UniformData operator()(std::string_view uniformName);

		private:
			const Shader::Uniforms* m_Uniforms;
			const Shader::UniformBlocks* m_UniformBlocks;
			uint32_t m_GlobalOffset = (uint32_t)-1;
			std::pair<std::string, const Shader::UniformBlocks::UniformBlockData*>& m_BlockCache;
		};

	public:
		UniformAccess(const Pipeline& pipeline, uint32_t globalOffset = 0);
		UniformAccess() = default;

		UniformDataAccess Resolve(Shader::Stage stage);

		template<Shader::Stage stage>
		UniformData Resolve(std::string_view uniformBlockName, std::string_view uniformName)
		{
			RAY_MEXPECTS(m_Shader, "[UniformAccess] Not created using pipeline.");
			return Resolve(stage)(uniformBlockName, uniformName);
		}

		template<Shader::Stage stage>
		UniformData Resolve(std::string_view uniformName)
		{
			RAY_MEXPECTS(m_Shader, "[UniformAccess] Not created using pipeline.");
			return Resolve(stage)(uniformName);
		}

	private:
		const Shader* m_Shader = nullptr;
		uint32_t m_GlobalOffset = (uint32_t)-1;
		std::pair<std::string, const Shader::UniformBlocks::UniformBlockData*> m_BlockCache{};
	};
}  // namespace At0::Ray
