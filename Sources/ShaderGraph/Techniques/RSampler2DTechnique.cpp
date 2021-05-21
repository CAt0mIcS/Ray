#include "Rpch.h"
#include "RSampler2DTechnique.h"

#include "Utils/RString.h"


namespace At0::Ray
{
	uint16_t Sampler2DTechnique::s_SamplerID = 0;

	Sampler2DTechnique::Sampler2DTechnique() : m_SamplerID(s_SamplerID++) {}

	std::string Sampler2DTechnique::GetInputAttributes() const { return ""; }

	std::string Sampler2DTechnique::GetSamplerUniforms() const { return ""; }

	std::string Sampler2DTechnique::GetFunctionCalls() const
	{
		return String::Serialize("texture(sampler2D_{0}, inUV)", m_SamplerID);
	}
}  // namespace At0::Ray
