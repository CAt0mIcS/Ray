#pragma once

#include "../RBase.h"
#include "RWindow.h"

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
		OpenGLWindow(std::string_view name, const Point2& pos, const Size2& size, Widget* parent = nullptr);

		~OpenGLWindow();

		virtual std::string GetTitle() const override;
		virtual void SetTitle(std::string_view title) override;

		virtual void Show() const override;
		virtual void Hide() const override;
		virtual void Maximize() const override;
		virtual void Minimize() const override;

		/// <summary>
		/// Moves the window to screen coordinates in pos
		/// </summary>
		/// <param name="pos">Are the coordinates where the window will be moved to</param>
		virtual void Move(const Point2& pos) override;

		/// <summary>
		/// Resizes the window
		/// </summary>
		/// <param name="size">Is the new size of the window</param>
		virtual void Resize(const Size2& size) override;

		/// <summary>
		/// Getter for the current window position
		/// </summary>
		/// <returns>The window position</returns>
		virtual Point2 GetPos() const override;

		/// <summary>
		/// Getter for the current window size
		/// </summary>
		/// <returns>The current window size</returns>
		virtual Size2 GetSize() const override;

		virtual void Close() override;
		virtual bool IsOpen() const override;

		/// <summary>
		/// Called every frame to read from the internal message queue
		/// </summary>
		virtual void Update() override;

		/// <summary>
		/// Sets the window icon
		/// </summary>
		/// <param name="path">Is the path to the .ico file</param>
		virtual void SetIcon(std::string_view path);

	protected:
		virtual void* GetNativeWindow() const override { return (void*)m_hWnd; }

	private:
		/// <summary>
		/// Sets all glfw event callbacks
		/// </summary>
		void SetUpEventCallbacks();

	private:
		static bool s_GLFWInitialized;

		bool m_IsOpen;
		GLFWwindow* m_hWnd;
	};
}


