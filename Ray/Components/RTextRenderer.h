#pragma once

#include "RComponent.h"
#include "Base/RRenderer.h"


namespace At0::Ray
{
	class FlatTextMaterial;
	class CommandBuffer;

	class RAY_EXPORT TextRenderer : public Component, public Renderer
	{
	public:
		TextRenderer(Entity entity, Ref<FlatTextMaterial> material);

		void Update();
		void Render(const CommandBuffer& cmdBuff) const;

		// std::string_view GetText() const;
		// void SetText(std::string_view txt);
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(TextRenderer)
