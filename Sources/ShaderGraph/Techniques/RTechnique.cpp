#include "Rpch.h"
#include "RTechnique.h"

namespace At0::Ray
{
	void Technique::Connect(uint32_t pointID, Scope<Technique> tech)
	{
		m_ChildTechniques[pointID] = std::move(tech);
	}
}  // namespace At0::Ray
