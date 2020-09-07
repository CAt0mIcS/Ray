#pragma once

#include <QRD/QRD.h>


namespace NPE
{
	class Application
	{
	public:
		Application();
		~Application();

		int Run();

	private:
		QRD::Database m_Database;
	};
}



