#include "Rpch.h"
#include "RRenderer.h"


namespace At0::Ray
{
	Renderer& Renderer::Get()
	{
		static Renderer renderer;
		return renderer;
	}

	void Renderer::Update(Delta ts)
	{
		for (auto& renderer : m_SubRenderers)
			renderer->Update(ts);
	}

	void Renderer::Bind(const CommandBuffer& cmdBuff) const
	{
		for (auto& renderer : m_SubRenderers)
			renderer->Bind(cmdBuff);
	}
}  // namespace At0::Ray
