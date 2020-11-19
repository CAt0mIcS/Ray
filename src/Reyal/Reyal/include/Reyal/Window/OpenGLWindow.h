#ifndef RL_GLFWWINNDOW_H
#define RL_GLFWWINNDOW_H

#include "RlBase.h"
#include "Window.h"



namespace At0::Reyal
{
	class RL_API OpenGLWindow : public Window
	{
	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		OpenGLWindow(const std::string_view name, Widget* parent = nullptr, bool isMainWindow = false);
		
		/// <summary>
		/// Window Deconstructor
		/// </summary>
		~OpenGLWindow();

		/// <summary>
		/// Getter for the current Window Title
		/// </summary>
		/// <returns>The title of this Window</returns>
		virtual std::string GetTitle() const override;

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		virtual void SetTitle(const std::string_view title) override;

		/// <summary>
		/// Shows the Window
		/// </summary>
		virtual void Show() const override;

		/// <summary>
		/// Hides the Window
		/// </summary>
		virtual void Hide() const override;

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		virtual void Maximize() const override;

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		virtual void Minimize() const override;

		/// <summary>
		/// Closes the Window
		/// </summary>
		virtual void Close() const override;

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		virtual bool IsOpen() const override;

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <param name="exitCode">Will be filled with the exit code when the application quits, specify nullptr to ignore</param>
		/// <returns>True if the window was closed, false otherwise</returns>
		virtual bool ShouldClose() override;

		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="path">Is the path to the .ico file</param>
		virtual void SetIcon(const std::string_view path);

	private:
		/// <summary>
		/// Sets all glfw event callbacks
		/// </summary>
		void SetUpEventCallbacks();

	private:
		static bool s_GLFWInitialized;
	};
}



#endif // RL_GLFWWINNDOW_H