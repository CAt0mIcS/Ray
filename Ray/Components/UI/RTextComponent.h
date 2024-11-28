#pragma once

#include "../RComponent.h"

#include <string>


namespace At0::Ray
{
	class Font;
	class Window;

	class RAY_EXPORT TextComponent : public Component
	{
	public:
		TextComponent(Entity entity, const Window& window, std::string text, Ref<Font> font);

		Ref<Font> GetSharedFont() const { return m_Font; }
		const Font& GetFont() const { return *m_Font; }
		Font& GetFont() { return *m_Font; }

		std::string_view GetText() const { return m_Text; }
		void SetText(std::string text);

	private:
		void LoadMeshes();

	private:
		Ref<Font> m_Font;
		std::string m_Text;

		const Window* m_Window;
	};
}  // namespace At0::Ray


RAY_EXPORT_COMPONENT(TextComponent)
