#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	class RL_API Application
	{
	public:

		Application();

		int Run();

		virtual ~Application();
	};

	// Defined by the client
	Application* CreateApplication();

}



