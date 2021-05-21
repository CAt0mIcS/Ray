#pragma once

#include "RTechnique.h"


namespace At0::Ray
{
	class RAY_EXPORT MultiplyTechnique : public Technique
	{
	public:
		enum Connection
		{
			Left,
			Right
		};

	public:
		MultiplyTechnique() = default;

		std::string GetFunctions() const override;
		std::string GetFunctionCalls() const override;
	};
}  // namespace At0::Ray
