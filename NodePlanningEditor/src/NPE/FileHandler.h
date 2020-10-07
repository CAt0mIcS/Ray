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

	/// <summary>
	/// FileHandler class, which handles most IO stuff
	/// </summary>
	class FileHandler
	{
	public:
		/// <summary>
		/// FileHandler Constructor
		/// </summary>
		FileHandler();

		/// <summary>
		/// Saves the scene to the file
		/// </summary>
		/// <param name="controls">Is a list of all Controls to save</param>
		/// <param name="lines">Is a list of all lines to save</param>
		/// <param name="zoom">Is the zoom level to save</param>
		/// <param name="saveToNewLocation">Is true if the SaveFileDialog should open to save to a new location</param>
		void SaveScene(const std::vector<GUI::Control*> controls, const std::vector<Line>& lines, int zoom, bool saveToNewLocation = false);
		
		/// <summary>
		/// Loads a scene from the file specified in "config.cfg" file
		/// </summary>
		/// <param name="win">Is the main window</param>
		/// <param name="lines">Is a reference to a list of lines which will be populated with the saved ones</param>
		/// <param name="zoom">Is a reference to the zoom level</param>
		void LoadScene(GUI::MainWindow& win, std::vector<Line>& lines, int& zoom);

		/// <summary>
		/// Creates a default databases template
		/// </summary>
		void CreateDefaultTemplate();

		/// <summary>
		/// Called once to load the save at the beginning of the program and check if the save file exists
		/// </summary>
		void CreateOrLoadSave();

		/// <summary>
		/// Loads a new save file
		/// </summary>
		/// <param name="filepath">Is the file which will be loaded</param>
		void ChangeScene(const std::string& filepath);

		/// <summary>
		/// Asks the user to choose a file to open
		/// </summary>
		/// <param name="win">Is the MainWindow where all the controls will be</param>
		/// <param name="lines">Is a vector where all line connections will be added to</param>
		/// <param name="m_Zoom">Is the current zoom of the scene</param>
		/// <returns>True if the user didn't abort the process, false otherwise</returns>
		bool OpenScene(GUI::MainWindow& win, std::vector<Line>& lines, int& m_Zoom);

		/// <summary>
		/// Getter for the current Save File name
		/// </summary>
		/// <returns>The name of the current Save File</returns>
		const std::string& GetFileName() const { return m_FileName; }

	private:
		/// <summary>
		/// Writes the file path to fonfig
		/// </summary>
		/// <param name="filePath">Is the path with file to write to config</param>
		void WriteConfig(std::string filePath);

	private:
		/// <summary>
		/// Is the database object
		/// </summary>
		QRD::Database* m_Db;

		/// <summary>
		/// Is true if we are saving at a temporary location
		/// </summary>
		bool m_IsTemporarySave;

		/// <summary>
		/// Specifies the name of the currently loaded file
		/// </summary>
		std::string m_FileName;

		/// <summary>
		/// Is the name of the default save file
		/// </summary>
		static constexpr const char* s_SaveFileName = "save.dbs";

		/// <summary>
		/// is the name of the default config file path
		/// </summary>
		static constexpr const char* s_ConfigFilePath = "config.cfg";
	};
}



