#pragma once

#include "RTechnique.h"
#include "../../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT ColorTechnique : public Technique
	{
	public:
		ColorTechnique(Float4 defaultColor = Float4{ 1.0f, 1.0f, 1.0f, 1.0f });


		std::string GetFunctions() const override;
		std::string GetFunctionCalls() const override;

		void SetColor(Float4 color) { m_Color = std::move(color); }
		const Float4& GetColor() const { return m_Color; }

	private:
		Float4 m_Color;
	};
}  // namespace At0::Ray
