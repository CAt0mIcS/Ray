#include "pch.h"
#include "Application.h"



namespace NPE
{
	Application::Application()
		: m_Database("saves\\save.dbs", 3)
	{
		//LoadFile();
	}

	int Application::Run()
	{
		return 0;
	}

	Application::~Application()
	{
		//SaveFile();
	}

}


