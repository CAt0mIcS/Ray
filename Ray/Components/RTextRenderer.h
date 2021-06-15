#pragma once

#include "RComponent.h"
#include "Base/RRenderer.h"


namespace At0::Ray
{
	class Material;

	class RAY_EXPORT TextRenderer : public Component, public Renderer
	{
	public:
		TextRenderer(Entity entity, Ref<Material> material);

		std::string_view GetText() const { return m_Text; }
		void SetText(std::string_view txt) { m_Text = txt; }

	private:
		std::string m_Text;
		Ref<Material> m_Material;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(TextRenderer)
