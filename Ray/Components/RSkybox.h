// #pragma once
//
// #include "RComponent.h"
//
// #include "../Events/REventListener.h"
// #include "../Events/RCustomEvents.h"
//
//
// namespace At0::Ray
//{
//	class Texture;
//	class TextureCubemap;
//	class Image2D;
//
//	class RAY_EXPORT Skybox : public Component, EventListener<CameraChangedEvent>
//	{
//	public:
//		Skybox(Entity entity, Ref<Texture> texture);
//		Skybox(Entity entity, Ref<TextureCubemap> texture);
//
//	private:
//		virtual void OnEvent(CameraChangedEvent& e) override;
//	};
// }  // namespace At0::Ray
//
//
// RAY_EXPORT_COMPONENT(Skybox);
