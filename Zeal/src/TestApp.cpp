#include "pch.h"

#include <Reyal/Reyal.h>


namespace Zeal
{
	class TestApp : public Reyal::Application
	{
	public:
		TestApp()
		{

		}
		
		~TestApp()
		{

		}

	};


	Reyal::Application* Reyal::CreateApplication()
	{
		return new TestApp();
	}
}