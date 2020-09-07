#pragma once

#include <string>
#include <QRD/QRD.h>


namespace GUI
{
	class MainWindow;
}


namespace NPE
{
	class FileHandler
	{
	public:
		FileHandler(const std::string& filePathToSaveFile);

		void SaveScene(const GUI::MainWindow& win);
		void LoadScene(GUI::MainWindow& win);

	private:
		QRD::Database m_Db;
	};
}



