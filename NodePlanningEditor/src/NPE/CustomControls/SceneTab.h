#pragma once

#include "GUI/Controls/Control.h"


namespace NPE
{
	class SceneTab : public GUI::Control
	{
	public:
		/// <summary>
		/// SceneTab constructor
		/// </summary>
		/// <param name="parent">Is the parent of the tab</param>
		SceneTab(GUI::Control* parent = nullptr);

		/// <summary>
		/// Setter for the Save File path
		/// </summary>
		/// <param name="filepath">Is the full path to the new Save File</param>
		void SetFile(const std::string& filepath) { m_SaveFilePath = filepath; }

		/// <summary>
		/// Getter for the path to the Save File
		/// </summary>
		/// <returns>The full path to the Save File</returns>
		const std::string& GetFilePath() const { return m_SaveFilePath; }

		/// <summary>
		/// Renders Control and all it's children
		/// </summary>
		/// <returns>True if the Control was rendered(was in window), false otherwise</returns>
		virtual bool Render() override;

		/// <summary>
		/// All events of the specific Control will be dispatched to this function
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, else false and the event will be dispatched to the client</returns>
		virtual bool OnEvent(GUI::Event& e) override;

	private:
		/// <summary>
		/// Specifies the full path to the Save File
		/// </summary>
		std::string m_SaveFilePath;
	};
}



