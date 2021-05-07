#pragma once

#include "../RBase.h"
#include "RComponent.h"

class Texture2D;

namespace At0::Ray
{
	class RAY_EXPORT Skybox : public Component
	{
	public:
		Skybox(Entity entity);

	private:
		Ref<Texture2D> m_Texture;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Skybox);
