#include "FileHandler.h"


namespace NPE
{
	FileHandler::FileHandler(const std::string& filePathToSaveFile)
		:m_Db(filePathToSaveFile, 3)
	{
	}
	
	void FileHandler::SaveScene(const GUI::MainWindow& win)
	{
	}
	
	void FileHandler::LoadScene(GUI::MainWindow& win)
	{
	}
}


