#pragma once

#include "Ray/RBase.h"
#include "RayBase/RTime.h"
#include "Ray/Scene/RCamera.h"

#include "RComponent.h"


namespace At0::Ray
{
	class Texture2D;
	class TextureCubemap;
	class Image2D;

	class RAY_EXPORT Skybox : public Component, EventListener<CameraChangedEvent>
	{
	public:
		Skybox(Entity entity, Ref<Texture2D> texture);
		Skybox(Entity entity, Ref<TextureCubemap> texture);

	private:
		void OnEvent(CameraChangedEvent& e);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Skybox);
