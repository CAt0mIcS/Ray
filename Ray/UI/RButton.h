#pragma once

#include "RWidget.h"


namespace At0::Ray
{
	class Texture2D;

	class RAY_EXPORT Button : public Widget
	{
	public:
		Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
			const Float3& color = { 1.0f, 1.0f, 1.0f });
		Button(Entity entity, std::string_view name, Float2 pos, float width, float height,
			Ref<Texture2D> texture);

		void SetColor(const Float3& color);
		void SetTexture(Ref<Texture2D> texture);

	private:
		/**
		 * True if the color material is in use.
		 * False if the texture material is in use.
		 */
		bool m_UseColorMaterial = true;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(Button);
