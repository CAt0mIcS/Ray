#pragma once

#include "../RBase.h"
#include "RComponent.h"


namespace At0::Ray
{
	class Image;
	class RAY_EXPORT Skybox : public Component
	{
	public:
		Skybox(Entity entity);

	private:
		Ref<Image> m_Texture;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Skybox);
