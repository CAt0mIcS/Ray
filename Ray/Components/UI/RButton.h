#pragma once

#include "../Base/RWidget.h"


namespace At0::Ray
{
	class Texture;
	class Window;

	class RAY_EXPORT Button : public Widget
	{
	public:
		Button(Entity entity, Window& window, std::string_view name, Float2 pos, float width,
			float height, const Float4& color = { 1.f, 1.f, 1.f, 1.f });
		Button(Entity entity, Window& window, std::string_view name, Float2 pos, float width,
			float height, Ref<Texture> texture);

		void SetColor(const Float4& color);
		void SetTexture(Ref<Texture> texture);

	private:
		/**
		 * True if the color material is in use.
		 * False if the texture material is in use.
		 */
		bool m_UseColorMaterial = true;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Button);
