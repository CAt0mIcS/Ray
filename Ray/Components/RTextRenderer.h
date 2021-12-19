#pragma once

#include "RComponent.h"
#include "Base/RRenderer.h"

#include "RMesh.h"


namespace At0::Ray
{
	class Material;
	class CommandBuffer;

	class RAY_EXPORT TextRenderer : public Component, public Renderer
	{
	public:
		TextRenderer(Entity entity, Ref<Material> material);

		void Update();
		void Render(const CommandBuffer& cmdBuff) const;

		std::string_view GetText() const;
		void SetText(std::string_view txt);

	private:
		Mesh m_Mesh;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(TextRenderer)
