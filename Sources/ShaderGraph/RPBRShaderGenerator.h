#pragma once

#include "RShaderGenerator.h"


namespace At0::Ray
{
	class RAY_EXPORT PBRShaderGenerator : public ShaderGenerator
	{
	public:
		enum Connection
		{
			Albedo,
			Normal,
			Emission,
			Metallic,
			Smoothness,
			Occlusion,
			Alpha,
			AlphaClipThreshold
		};
	};
}  // namespace At0::Ray
