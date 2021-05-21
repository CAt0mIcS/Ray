#pragma once

#include "RTechnique.h"
#include "../../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT Float4Technique : public Technique
	{
	public:
		Float4Technique(Float4 defaultValue = Float4{ 1.0f, 1.0f, 1.0f, 1.0f });

		std::string GetFunctionCalls() const override;

		void SetValue(Float4 value) { m_Value = std::move(value); }
		const Float4& GetValue() const { return m_Value; }

	private:
		Float4 m_Value;
	};
}  // namespace At0::Ray
