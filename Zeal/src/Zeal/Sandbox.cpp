#include "pch.h"
#include "Sandbox.h"

#include <iostream>

namespace Zeal
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.Show();
	}
	
	Sandbox::~Sandbox()
	{
		ZL_PROFILE_FUNCTION();
	}
	
}
