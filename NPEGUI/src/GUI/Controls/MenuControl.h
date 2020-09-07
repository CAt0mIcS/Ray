#pragma once

#include "Control.h"


namespace GUI
{
	class MenuItem;

	class GUI_API MenuControl : public Control
	{
	public:
		void AddItem(MenuItem* item);

	protected:
		MenuControl(Control* parent);


		virtual bool Render() override = 0;

	protected:
		Control* m_Parent;
	};

	class GUI_API MenuItem : public MenuControl
	{
	public:
		MenuItem(Control* parent, const std::string& text);

		virtual bool Render() override;

	private:
		std::string m_Text;
	};

	class GUI_API MenuBar : public MenuControl
	{
	public:
		MenuBar(Control* parent);

		virtual bool Render() override;
	};

}



