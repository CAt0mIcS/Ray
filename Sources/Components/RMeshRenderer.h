#pragma once

#include "../RBase.h"
#include "RComponent.h"


namespace At0::Ray
{
	class CommandBuffer;

	class RAY_EXPORT MeshRenderer : public Component
	{
	public:
		MeshRenderer(Entity entity);

		void Render(const CommandBuffer& cmdBuff) const;
	};
}  // namespace At0::Ray
