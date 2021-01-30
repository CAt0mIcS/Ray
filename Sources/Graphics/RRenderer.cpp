#include "Rpch.h"
#include "RRenderer.h"

#include "RRenderStage.h"


namespace At0::Ray
{
	RenderStage* Renderer::GetRenderStage(uint32_t index) const
	{
		if (m_RenderStages.empty() || m_RenderStages.size() < index)
			return nullptr;

		return m_RenderStages[index].get();
	}

	void Renderer::AddRenderStage(Scope<RenderStage>&& renderStage)
	{
		m_RenderStages.emplace_back(std::move(renderStage));
	}

}  // namespace At0::Ray
