#pragma once

#include <string>
#include <QRD/QRD.h>


namespace NPE
{
	class Application;

	class FileHandler
	{
	public:
		FileHandler(const std::string& filePathToSaveFile);

		void SaveScene(Application& app);
		void LoadScene(Application& app);

	private:
		QRD::Database m_Db;
	};
}



