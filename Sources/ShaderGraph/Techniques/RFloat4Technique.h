#pragma once

#include "RTechnique.h"
#include "../../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT Float4Technique : public Technique
	{
	public:
		/**
		 * @param uniformBlockName Optional name of the uniform block if you want to change the
		 * value during runtime
		 * @param uniformName Name of the uniform in the optional uniform block
		 */
		Float4Technique(Float4 defaultValue = Float4{ 1.0f, 1.0f, 1.0f, 1.0f },
			const std::string& uniformBlockName = "", const std::string& uniformName = "");

		std::string GetFunctionCalls() const override;

		void SetValue(Float4 value) { m_Value = std::move(value); }
		const Float4& GetValue() const { return m_Value; }

	private:
		Float4 m_Value;

		std::string m_UniformBlockName;
		std::string m_UniformName;
	};
}  // namespace At0::Ray
