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
		FileHandler() : m_Db(nullptr), m_IsTemporarySave(false) {};


		void SaveScene(const std::vector<GUI::Control*> controls, const std::vector<Line>& lines, int zoom);
		void LoadScene(GUI::MainWindow& win, std::vector<Line>& lines, int& zoom);
		void CreateDefaultTemplate();

		void CreateDatabase(const std::string& filepath);
		void CreateOrLoadSave();

		void ChangeScene(const std::string& filepath);

	private:
		QRD::Database* m_Db;
		bool m_IsTemporarySave;

		static constexpr const char* s_SaveFileName = "save.dbs";
		static constexpr const char* s_ConfigFilePath = "config.cfg";
	};
}



