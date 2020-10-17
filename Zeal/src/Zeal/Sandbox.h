#pragma once

#include <Reyal/Reyal.h>


namespace Zeal
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
	};


	Reyal::Application* Reyal::CreateApplication()
	{
		return new Sandbox();
	}
}



