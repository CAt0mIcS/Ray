#pragma once

#include "RSubRenderer.h"
#include "../../Scene/RScene.h"


namespace At0::Ray
{
	class RAY_EXPORT MeshRenderer : public SubRenderer, EventListener<EntityCreatedEvent>
	{
	public:
		MeshRenderer();

		void Update(Delta ts) override;
		void Bind(const CommandBuffer& cmdBuff) const override;

	private:
		void OnEvent(EntityCreatedEvent& e) override;

	private:
		bool m_RerecordCommandBuffers = true;
	};
}  // namespace At0::Ray
