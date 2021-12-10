#pragma once

#include "../RBase.h"
#include "../Core/RTime.h"
#include "../Scene/RCamera.h"

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
		virtual void OnEvent(CameraChangedEvent& e) override;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Skybox);
