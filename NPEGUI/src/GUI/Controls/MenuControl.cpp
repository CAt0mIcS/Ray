#include "pch.h"
#include "MenuControl.h"

#include "GUI/Graphics/Renderer.h"


namespace GUI
{
	MenuControl::MenuControl(Control* parent)
		: m_Parent(parent)
	{

	}

	void MenuControl::AddItem(MenuItem* item)
	{
		m_Children.emplace_back(item);
	}


	MenuItem::MenuItem(Control* parent, const std::string& text)
		: MenuControl(parent), m_Text(text)
	{
		SetType(Type::MenuItem);
		SetPos({ 10, 10 });
		SetSize({ 50, 20 });
		SetColor({ 200, 200, 200 });
	}

	bool MenuItem::Render()
	{
		Renderer::Get().RenderRect(GetPos(), GetSize(), GetColor());

		for (auto* child : GetChildren())
		{
			child->Render();
		}


		return true;
	}

	MenuBar::MenuBar(Control* parent)
		: MenuControl(parent)
	{
		SetType(Type::MenuBar);
		SetPos({ 0, 0 });
		SetSize({ 1920, 50 });
		SetColor({ 255, 255, 255 });
	}

	bool MenuBar::Render()
	{
		Renderer::Get().RenderRect(GetPos(), GetSize(), GetColor());

		for (auto* child : GetChildren())
		{
			child->Render();
		}

		return true;
	}

}


