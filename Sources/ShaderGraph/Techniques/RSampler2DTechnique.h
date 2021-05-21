#pragma once

#include "RTechnique.h"


namespace At0::Ray
{
	class RAY_EXPORT Sampler2DTechnique : public Technique
	{
	public:
		enum Connections
		{
			Texture
		};

	public:
		Sampler2DTechnique();

		std::string GetFunctionCalls() const override;

	private:
		// Avoid naming conflicts by giving each sampler name and ID
		static uint16_t s_SamplerID;

		uint16_t m_SamplerID = 0;
	};
}  // namespace At0::Ray
