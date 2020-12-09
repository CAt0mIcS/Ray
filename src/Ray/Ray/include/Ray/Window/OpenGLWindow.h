#pragma once

#include "RBase.h"
#include "Window.h"

typedef struct GLFWwindow GLFWwindow;


namespace At0::Ray
{
	class Renderer3D;
	//class Renderer2D;

	class RAY_API OpenGLWindow : public Window
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
		virtual void Close() override;

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		virtual bool IsOpen() const override;

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void OnUpdate() override;

		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="path">Is the path to the .ico file</param>
		virtual void SetIcon(const std::string_view path);

		/// <summary>
		/// Getter for the Native Window
		/// </summary>
		/// <returns>The Native Window</returns>
		virtual void* GetNativeWindow() const override { return (void*)m_hWnd; }

		/// <summary>
		/// Initializes the 3D Renderer, checks if it has already been initialized
		/// </summary>
		virtual bool InitRenderer3D() override;

		/// <summary>
		/// Initializes the 2D Renderer, checks if it has already been initialized
		/// </summary>
		virtual bool InitRenderer2D() override;

		/// <summary>
		/// Getter for the Renderer of this Window
		/// </summary>
		/// <returns>The Window Renderer</returns>
		virtual Renderer3D* GetRenderer3D() const override;

	private:
		/// <summary>
		/// Sets all glfw event callbacks
		/// </summary>
		void SetUpEventCallbacks();

	private:
		static bool s_GLFWInitialized;
		
		bool m_IsOpen;

		/// <summary>
		/// Native window handle
		/// </summary>
		GLFWwindow* m_hWnd;

		/// <summary>
		/// Specifies the 3D renderer
		/// </summary>
		Scope<Renderer3D> m_Renderer3D;

		///// <summary>
		///// Specifies the 2D renderer
		///// </summary>
		//Scope<Renderer2D> m_Renderer2D;

		/// <summary>
		/// Specifies the old size of this window
		/// </summary>
		Size2 m_OldSize;

		/// <summary>
		/// Specifies the old position of this window
		/// </summary>
		Point2 m_OldPos;
	};
}


