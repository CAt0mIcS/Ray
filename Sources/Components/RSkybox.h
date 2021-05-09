#pragma once

#include "../RBase.h"
#include "../Core/RTime.h"

#include "RComponent.h"


namespace At0::Ray
{
	class Texture2D;
	class Image;
	class RAY_EXPORT Skybox : public Component
	{
	public:
		Skybox(Entity entity, Ref<Texture2D> texture);

		void Update(Delta dt);

	private:
		Ref<Image> m_Texture;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Skybox);
