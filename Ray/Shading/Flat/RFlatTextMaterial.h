#pragma once

#include "../RMaterial.h"
#include "Ray/Core/RMath.h"
#include "Ray/Graphics/Pipelines/RGraphicsPipeline.h"


namespace At0::Ray
{
	class Font;

	class RAY_EXPORT FlatTextMaterial : public Material
	{
	public:
		struct Layout
		{
			std::string text;
			Ref<Font> font;
			Float4 color;
		};

	public:
		FlatTextMaterial(Layout layout = {}, GraphicsPipeline::Layout pipelineLayout = {});

		std::string_view GetText() const { return m_Layout.text; }
		const Font& GetFont() const { return *m_Layout.font; }
		Ref<Font> GetSharedFont() const { return m_Layout.font; }
		Float4 GetColor() const override { return m_Layout.color; }

	private:
		Layout m_Layout;
	};
}  // namespace At0::Ray
