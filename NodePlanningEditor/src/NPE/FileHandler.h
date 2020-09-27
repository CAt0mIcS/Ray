#pragma once

#include <string>
#include <QRD/QRD.h>

namespace GUI
{
	class MainWindow;
	class Control;
	class Button;
}


namespace NPE
{
	using Line = std::pair<GUI::Button*, GUI::Button*>;

	class FileHandler
	{
	public:
		FileHandler() = default;


		void SaveScene(const std::vector<GUI::Control*> controls, const std::vector<Line>& lines, int zoom);
		void LoadScene(GUI::MainWindow& win, std::vector<Line>& lines, int& zoom);
		void CreateDefaultTemplate(int zoom);

		void CreateDatabase(const std::string& filepath);


	private:
		QRD::Database* m_Db;
	};
}



