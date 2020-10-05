#pragma once

#include "Control.h"


namespace GUI
{
	/// <summary>
	/// Button used as Line connection point
	/// </summary>
	class GUI_API Button : public Control
	{
	public:
		/// <summary>
		/// Button Constructor
		/// </summary>
		/// <param name="parent">Is the parent Control</param>
		Button(_In_opt_ Control* parent = nullptr);

		/// <summary>
		/// Setter for Button text
		/// </summary>
		/// <param name="text">Is the new text</param>
		void SetText(_In_ const std::wstring& text) { m_Text = text; }

		/// <summary>
		/// Uses the Renderer to draw the Button and all it's child controls, 
		/// but does not call BeginDraw() or EndDraw()
		/// </summary>
		/// <returns>True if the Control was rendered, false otherwise</returns>
		virtual bool Render() override;
		
		/// <summary>
		/// All events of the specific Control will be dispatched to this function
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, else false and the event will be dispatched to the client</returns>
		virtual bool OnEvent(_In_ Event& e) override;

		/// <summary>
		/// Getter for Button text
		/// </summary>
		/// <returns>The Button text</returns>
		const std::wstring& GetText() const { return m_Text; }

		/// <summary>
		/// Calculates the layout of a new Button
		/// </summary>
		/// <param name="parentPos">Is the position of the parent Control</param>
		/// <param name="parentSize">Is the size of the parent Control</param>
		/// <returns>The new position and size of the Control</returns>
		virtual std::optional<std::pair<Util::NPoint, Util::NSize>> CalculateLayout(_In_ const Util::NPoint& parentPos, _In_ const Util::NSize& parentSize) override;

	private:
		/// <summary>
		/// The Button text
		/// </summary>
		std::wstring m_Text;
	};
}

