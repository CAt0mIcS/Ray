#ifndef RL_X11WINDOW_H
#define RL_X11WINDOW_H

#include "RlBase.h"
#include "Window.h"

struct _XDisplay;


namespace At0::Reyal
{
    class RL_API X11Window : public Window
    {
    public:
        /// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
        X11Window(const std::string_view name, Widget* parent = nullptr, bool isMainWindow = false);    

        /// <summary>
		/// Window Deconstructor
		/// </summary>
		~X11Window();

		/// <summary>
		/// Getter for the current Window Title
		/// </summary>
		/// <returns>The title of this Window</returns>
		std::string GetTitle() const override;

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		void SetTitle(const std::string_view title) override;

		/// <summary>
		/// Shows the Window
		/// </summary>
		void Show() const override;

		/// <summary>
		/// Hides the Window
		/// </summary>
		void Hide() const override;

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		void Maximize() const override;

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		void Minimize() const override;

		/// <summary>
		/// Closes the Window
		/// </summary>
		void Close() const override;

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		bool IsOpen() const override;

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <param name="exitCode">Will be filled with the exit code when the application quits, specify nullptr to ignore</param>
		/// <returns>True if the window was closed, false otherwise</returns>
		bool ShouldClose() override;

    private:
        _XDisplay* m_Display;
    };
}





#endif // RL_X11WINDOW_H