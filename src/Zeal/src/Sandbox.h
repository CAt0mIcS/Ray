#pragma once

#include <Ray/RApplication.h>
#include <Ray/Window/RWindow.h>

#include "ExtensionLoader/ExtensionLoader.h"

namespace At0::Zeal
{

	class Sandbox : public Ray::Application
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

}


