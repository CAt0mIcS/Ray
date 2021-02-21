#pragma once

#include "../../RBase.h"
#include "RShader.h"

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
			UniformData() {}

			template<typename T>
			UniformData& operator=(T&& data)
			{
				return *this;
			}

		private:
		};

	private:
		using ValueType =
			std::unordered_map<std::string, std::unordered_map<std::string, UniformData>>;

	public:
		UniformAccess(const Pipeline& pipeline);

		ValueType& Resolve(Shader::Stage stageFlag);

		std::unordered_map<Shader::Stage, ValueType> m_Uniforms;
	};
}  // namespace At0::Ray
