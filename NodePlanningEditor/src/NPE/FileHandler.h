#pragma once

#include <string>
#include <unordered_map>
#include <QRD/QRD.h>

#include "Util/Util.h"

namespace GUI
{
	class MainWindow;
	class Control;
	class Button;
	class SceneTab;
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
		/// <param name="filepath">Is the path to save to</param>
		/// <param name="controls">Is a list of all Controls to save</param>
		/// <param name="lines">Is a list of all lines to save</param>
		/// <param name="saveToNewLocation">Is true if the SaveFileDialog should open to save to a new location</param>
		/// <returns>The new path</returns>
		std::string SaveScene(const std::string& filepath, const std::vector<GUI::Control*> controls, const std::vector<Line>& lines, bool saveToNewLocation = false);
		
		/// <summary>
		/// Loads a scene from the file specified in "config.cfg" file
		/// </summary>
		/// <param name="win">Is the main window</param>
		/// <param name="lines">Is a reference to a list of lines which will be populated with the saved ones</param>
		void LoadScene(GUI::MainWindow& win, std::vector<Line>& lines);

		/// <summary>
		/// Creates a default databases template
		/// </summary>
		void CreateDefaultTemplate();

		/// <summary>
		/// Called once to load the save at the beginning of the program and check if the save file exists
		/// </summary>
		/// <param name="tabs">Is the structure which will be filled with all tabs</param>
		void CreateOrLoadSave(GUI::MainWindow& win, std::vector<GUI::SceneTab*>& tabs);

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
		/// <returns>The new path</returns>
		std::string OpenScene(GUI::MainWindow& win, std::vector<Line>& lines);

		/// <summary>
		/// Removes the active tab from the config file
		/// </summary>
		/// <param name="active">Is the active scene tab</param>
		void RemoveTabFromConfig(GUI::SceneTab* active);

		/// <summary>
		/// Creates all scene tabs from config
		/// </summary>
		void CreateSceneTabs(GUI::MainWindow& win);

	private:
		/// <summary>
		/// Writes the file path to fonfig
		/// </summary>
		void WriteConfig();

		/// <summary>
		/// Checks if the config file exists and creates it if it doesn't
		/// </summary>
		/// <returns>True if the file exists, false otherwise</returns>
		bool CreateConfigFile();

		/// <summary>
		/// Reads the config file and fills m_Configs
		/// </summary>
		void ReadConfig();

		/// <summary>
		/// Recursively creates the save file at filepath
		/// </summary>
		/// <param name="filepath">Is the path to the save file</param>
		/// <returns>True if the file exists, false otherwise</returns>
		bool CreateSaveFile(const std::string& filepath);

		/// <summary>
		/// Checks if directory exists
		/// </summary>
		/// <param name="dirNameIn">Is the directory to check</param>
		/// <returns>True if the directory exists, false otherwise</returns>
		bool DirectoryExists(const std::string& dir);

		/// <summary>
		/// Checks if file exists
		/// </summary>
		/// <param name="filename">Is the file to search for</param>
		/// <returns>True if the file exists, false otherwise</returns>
		bool FileExists(const std::string& filepath);

		/// <summary>
		/// Creates the database
		/// </summary>
		/// <param name="path">Is the path to the save file</param>
		void CreateDatabase(const std::string& path);

	private:
		/// <summary>
		/// Is the database object
		/// </summary>
		QRD::Database* m_Db;

		/// <summary>
		/// Specifies all loaded file paths
		/// </summary>
		std::vector<std::pair<std::string, std::string>> m_Configs;

		/// <summary>
		/// Is the name of the default save file
		/// </summary>
		inline static const std::string s_SaveFileName = "save.dbs";

		/// <summary>
		/// Is the name of the default config file path
		/// </summary>
		inline static const std::string s_ConfigFilePath = "config.cfg";

		/// <summary>
		/// Is the default save directory
		/// </summary>
		inline static const std::string s_DefaultSaveFileDir = "saves\\";

		/// <summary>
		/// Is the default name of a save file
		/// </summary>
		inline static const std::string s_DefaultSaveFileName = "save.dbs";
	};
}



