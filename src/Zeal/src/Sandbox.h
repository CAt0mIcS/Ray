#ifndef ZL_SANDBOX_H
#define ZL_SANDBOX_H

#include <Reyal/Application.h>
#include <Reyal/Window/Window.h>

namespace At0::Zeal
{

	class Sandbox : public Reyal::Application
	{
	public:
		/// <summary>
		/// Sandbox Constructor
		/// </summary>
		Sandbox();

		/// <summary>
		/// Sandbox Deconstructor
		/// </summary>
		~Sandbox();


		///TODO: Currently not used!

		/// <summary>
		/// Receives all window events (WindowResize, WindowClose, WindowPaint)
		/// </summary>
		/// <param name="receiver">Is the window which receives the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		virtual void OnEvent(Reyal::Widget* receiver, Reyal::Event& e);

	private:
		Reyal::Ref<Reyal::Window> m_Win;

	};
	
}


#include <RlRender/RendererAPI.h>

void At0::Reyal::CreateApplication()
{
	using namespace At0;

	Reyal::RendererAPI::SetAPI(Reyal::RendererAPI::API::D3D11);
	Reyal::Application::Create(new Zeal::Sandbox());
}

#endif // ZL_SANDBOX_H
