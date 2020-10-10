#include "pch.h"
#include "FileHandler.h"

#include "Application.h"

#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/TextBox.h"

#include "GUI/Graphics/Renderer.h"

#include "Util/Exceptions.h"

#include "GUI/Window/FileWindow.h"


namespace NPE
{
	FileHandler::FileHandler()
		: m_Db(nullptr), m_IsTemporarySave(false), m_FileName("")
	{

		//m_SaveFolder += "{{";
		//int len = 20;
		//static constexpr const char alphanum[] =
		//	"0123456789"
		//	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		//	"abcdefghijklmnopqrstuvwxyz";
		//
		////srand((unsigned)time(nullptr) * getpid());
		//
		//for (int i = 0; i < len; ++i)
		//	m_SaveFolder += alphanum[rand() % (sizeof(alphanum) - 1)];
		//
		//m_SaveFolder += "}}\\";
		////TODO: Random string here
		////m_SaveFolder = "saves\\";
	}

	void FileHandler::SaveScene(const std::vector<GUI::Control*> controls, const std::vector<Line>& lines, const Util::NSize& scale, bool saveToNewLocation)
	{
		HCURSOR prevCursor = GetCursor();
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		if (saveToNewLocation)
		{
			GUI::FileWindow win;

			GUI::FileWindow::FilterSpecs filterSpecs[] =
			{
				{ L"Database File", L"*.dbs;*.txt" },
				{ L"Any File" , L"*.*" }
			};

			auto result = win.ShowSaveDialog(L"Select a Save File", filterSpecs, (unsigned int)std::size(filterSpecs));
			
			if (result == L"")
				return;

			std::ofstream writer2(result);
			writer2.close();

			m_Db->Clear();
			m_Db->SetFilePath(Util::WideCharToMultiByte(result));
			CreateDefaultTemplate();
			m_IsTemporarySave = false;

			WriteConfig(Util::WideCharToMultiByte(result));
		}

		//clear save file
		m_Db->DeleteTable("NodeInfo");
		m_Db->DeleteTable("SceneInfo");
		m_Db->DeleteTable("Lines");

		//Table creation and setup
		QRD::Table& tbNodeInfo = m_Db->CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Db->CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Db->CreateTable("Lines");

		tbNodeInfo.AddField<QRD::NUMBER>("x");
		tbNodeInfo.AddField<QRD::NUMBER>("y");
		tbNodeInfo.AddField<QRD::NUMBER>("width");
		tbNodeInfo.AddField<QRD::NUMBER>("height");
		tbNodeInfo.AddField<QRD::TEXT>("txt1");
		tbNodeInfo.AddField<QRD::TEXT>("txt2");

		tbLines.AddField<QRD::NUMBER>("ID2");
		tbLines.AddField<QRD::NUMBER>("ID1");

		tbSceneInfo.AddField<QRD::NUMBER>("scaleX");
		tbSceneInfo.AddField<QRD::NUMBER>("scaleY");

		for (auto* control : controls)
		{
			const auto& pos = control->GetPos();
			const auto& size = control->GetSize();
			
			std::string txt1 = Util::WideCharToMultiByte(((GUI::TextBox*)control->GetChildren()[0])->GetText().text);
			std::string txt2 = Util::WideCharToMultiByte(((GUI::TextBox*)control->GetChildren()[1])->GetText().text);

			if (txt1[txt1.size() - 1] == '\0')
				txt1.erase(txt1.cend() - 1);

			if (txt2[txt2.size() - 1] == '\0')
				txt2.erase(txt2.cend() - 1);

			tbNodeInfo.AddRecord(pos.x, pos.y, size.width, size.height, txt1, txt2);
			NPE_LOG("Saved Node: \nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}\nTitle:\t{4}\nInfo:\t{5}\n", pos.x, pos.y, size.width, size.height, txt1, txt2);
		}

		NPE_LOG("\n");

		for (const Line& line : lines)
		{
			if (line.first && line.second)
			{
				tbLines.AddRecord(line.first->GetId(), line.second->GetId());
				NPE_LOG("Saved Line: \nStart:\tx={0} y={1}\nEnd:\tx={2} y={3}\n", line.first->GetPos().x, line.first->GetPos().y, line.second->GetPos().x, line.second->GetPos().y);
			}
			else
				NPE_LOG("Didn't save line because it didn't have a valid end point");
		}
		NPE_LOG("\n\n");
		tbSceneInfo.AddRecord(scale.width, scale.height);

		m_Db->WriteDb();
		SetCursor(prevCursor);
	}
	
	void FileHandler::LoadScene(GUI::MainWindow& win, std::vector<Line>& lines, Util::NSize& scale)
	{
		GUI::Control::ResetIdCounter(1);
		HCURSOR prevCursor = GetCursor();
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		for (auto* control : win.GetControls())
		{
			delete control;
		}

		win.GetControls().clear();
		lines.clear();

		//needs to be declared in release mode (error C4703: potentially uninitialized local pointer variable used)
		QRD::Table* tbNodeInfo = nullptr;
		QRD::Table* tbSceneInfo = nullptr;
		QRD::Table* tbLines = nullptr;

		tbNodeInfo = &m_Db->GetTable("NodeInfo");
		tbSceneInfo = &m_Db->GetTable("SceneInfo");
		tbLines = &m_Db->GetTable("Lines");

		try
		{
			scale.width = std::stof(tbSceneInfo->GetRecords().at(0).GetRecordData()[0]);
			scale.height = std::stof(tbSceneInfo->GetRecords().at(0).GetRecordData()[1]);
		}
		catch (std::out_of_range&)
		{
			scale = { 1.0f, 1.0f };
		}

		for (auto& record : tbNodeInfo->GetRecords())
		{
			auto& data = record.GetRecordData();

			GUI::Node* node = win.AddControl<GUI::Node>(new GUI::Node(&win));
			node->SetPos({ std::stof(data[0]), std::stof(data[1]) });
			node->SetSize({ std::stof(data[2]), std::stof(data[3]) });
			node->SetColor(Constants::g_DefaultNodeColor);
			Actions::CreateNodeTemplate(node);

			std::wstring txt1 = Util::MultiByteToWideChar(data[4]);
			std::wstring txt2 = Util::MultiByteToWideChar(data[5]);

			((GUI::TextBox*)(node->GetChildren()[0]))->SetText(txt1);
			((GUI::TextBox*)(node->GetChildren()[1]))->SetText(txt2);

			NPE_LOG("Loaded Node: \nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}\nTitle:\t{4}\nInfo:\t{5}\n", node->GetPos().x, node->GetPos().y, node->GetSize().width, node->GetSize().height, data[4], data[5]);
		}

		NPE_LOG("\n");

		for (auto& record : tbLines->GetRecords())
		{
			auto& data = record.GetRecordData();

			std::pair<GUI::Button*, GUI::Button*> line;

			for (auto* control : win.GetControls())
			{
				for (auto* child : control->GetChildren())
				{
					if (child->GetType() != GUI::Control::Type::Button)
						continue;

					int id = child->GetId();
					if (child->GetId() == std::stoi(data[0]))
					{
						line.first = (GUI::Button*)child;
						break;
					}
					else if (child->GetId() == std::stoi(data[1]))
					{
						line.second = (GUI::Button*)child;
						break;
					}
				}
				if (line.first && line.second)
					break;
			}

			if (line.first && line.second)
			{
				lines.emplace_back(line);
				NPE_LOG("Loaded Line: \nStart:\tx={0} y={1}\nEnd:\tx={2} y={3}\n", line.first->GetPos().x, line.first->GetPos().y, line.second->GetPos().x, line.second->GetPos().y);
			}
		}
		NPE_LOG("\n\n");
		SetCursor(prevCursor);
	}

	void FileHandler::CreateDefaultTemplate()
	{
		QRD::Table& tbNodeInfo = m_Db->CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Db->CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Db->CreateTable("Lines");

		tbNodeInfo.AddField<QRD::NUMBER>("x");
		tbNodeInfo.AddField<QRD::NUMBER>("y");
		tbNodeInfo.AddField<QRD::NUMBER>("width");
		tbNodeInfo.AddField<QRD::NUMBER>("height");
		tbNodeInfo.AddField<QRD::TEXT>("txt1");
		tbNodeInfo.AddField<QRD::TEXT>("txt2");

		tbLines.AddField<QRD::NUMBER>("ID2");
		tbLines.AddField<QRD::NUMBER>("ID1");

		tbSceneInfo.AddField<QRD::NUMBER>("scaleX");
		tbSceneInfo.AddField<QRD::NUMBER>("scaleY");

		m_Db->WriteDb();
	}

	void FileHandler::CreateOrLoadSave()
	{
		std::string fileDir = "saves\\";

		/// <summary>
		/// Checks if directory exists
		/// </summary>
		/// <param name="dirNameIn">Is the directory to check</param>
		/// <returns>True if the directory exists, false otherwise</returns>
		auto dirExists = [](const std::string& dirNameIn)
		{
			NPE_LOG("Checking if directory {0} exists...", dirNameIn);

			DWORD ftyp = GetFileAttributesA(dirNameIn.c_str());
			if (ftyp == INVALID_FILE_ATTRIBUTES)
				return false;  //something is wrong with your path!

			if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
				return true;   // this is a directory!

			return false;    // this is not a directory!
		};

		/// <summary>
		/// Checks if file exists
		/// </summary>
		/// <param name="filename">Is the file to search for</param>
		/// <returns>True if the file exists, false otherwise</returns>
		auto fileExists = [](const std::string& fileName)
		{
			struct stat buffer;
			NPE_LOG("Checking if file {0} exists...", fileName);
			return (stat(fileName.c_str(), &buffer) == 0);
		};

		/**
		* Creating config directory and file
		*/
		bool configFileExists = true;
		if (!fileExists(s_ConfigFilePath))
		{
			NPE_LOG("Config file config.cfg doesn't exist, creating it...");

			/**
			* Write temporary path to config file and create it
			*/
			std::ofstream writer(s_ConfigFilePath);
			writer << fileDir + s_SaveFileName + '\n';
			writer << fileDir;
			writer.close();
			configFileExists = false;
		}
		else
			NPE_LOG("Config file exists");

		/**
		* Read configs
		*/
		std::ifstream reader(s_ConfigFilePath);
		std::string line;
		std::unordered_map<std::string, std::string> configs;
		configs["SaveFile"] = "";
		configs["SaveDir"] = "";

		for (auto& config : configs)
		{
			std::getline(reader, line);
			if (line.empty())
			{
				configs["SaveFile"] = fileDir + s_SaveFileName;
				configs["SaveDir"] = fileDir;
				break;
			}
			else
			{
				config.second = line;
			}
		}
		reader.close();

		if (configs["SaveDir"] == fileDir)
			m_IsTemporarySave = true;

		bool saveFileExist = true;
		if (!dirExists(configs["SaveDir"]))
		{
			NPE_LOG("Directory {0} doesn't exist, using temporary storage location ({1})", configs["SaveFile"], fileDir);
			m_IsTemporarySave = true;
			if (!CreateDirectoryA(fileDir.c_str(), nullptr))
			{
				NPE_THROW_LAST_WND_EXCEPT();
			}
			saveFileExist = false;
		}
		else
			NPE_LOG("Directory {0} exists", fileDir);

		if (!saveFileExist || !fileExists(configs["SaveFile"]))
		{
			configs["SaveDir"] = fileDir;
			configs["SaveFile"] = fileDir + s_SaveFileName;
			NPE_LOG("Save file in directory {0} doesn't exist, using temporary storage location", configs["SaveFile"]);
			
			if (!dirExists(configs["SaveDir"]))
			{
				if (!CreateDirectoryA(configs["SaveDir"].c_str(), nullptr))
				{
					NPE_THROW_LAST_WND_EXCEPT();
				}
			}

			std::ofstream writer(fileDir + s_SaveFileName);
			writer << "";
			writer.close();

			std::ofstream writer2(s_ConfigFilePath);
			writer2 << configs["SaveFile"] << '\n';
			writer2 << configs["SaveDir"];
			writer2.close();

			saveFileExist = false;
		}
		else
			NPE_LOG("File exists");

		m_Db = new QRD::Database(configs["SaveFile"], 3, 6);

		if (!saveFileExist)
		{
			CreateDefaultTemplate();
		}

		auto fileNameBegin = configs["SaveFile"].find_last_of('\\');
		m_FileName = configs["SaveFile"].substr(fileNameBegin + 1);
	}

	void FileHandler::ChangeScene(const std::string& filepath)
	{
		m_Db->Clear();

		/**
		* Checking if file size is 0, if so the we need to create a default template
		*/
		{
			std::ifstream in(filepath, std::ifstream::ate | std::ifstream::binary);
			size_t size = (size_t)in.tellg();
			in.close();
		
			if (size == 0)
			{
				CreateDefaultTemplate();
			}
		}

		/**
		* Get filename of new scene
		*/
		auto fileNameBegin = filepath.find_last_of('\\');
		m_FileName = filepath.substr(fileNameBegin + 1);

		m_Db->SetFilePath(filepath);
		m_Db->ReadDb();
		WriteConfig(filepath);
	}

	bool FileHandler::OpenScene(GUI::MainWindow& win, std::vector<Line>& lines, Util::NSize& scale)
	{
		GUI::FileWindow fileWin;

		GUI::FileWindow::FilterSpecs filterSpecs[] =
		{
			{ L"Database File", L"*.dbs;*.txt" },
			{ L"Any File" , L"*.*" }
		};

		auto result = fileWin.ShowOpenDialog(L"Select file to open", filterSpecs, (unsigned int)std::size(filterSpecs));

		if (result == L"")
			return false;

		ChangeScene(Util::WideCharToMultiByte(result));
		LoadScene(win, lines , scale);
		return true;
	}
	
	void FileHandler::WriteConfig(std::string filePath)
	{
		//Remove null-termination character because std::ofstream also writes it
		if(filePath[filePath.size() - 1] == '\0')
			filePath.erase(filePath.cend() - 1);
		
		auto replaceBegin = filePath.find_last_of('\\');

		std::ofstream writer(s_ConfigFilePath);
		writer << filePath << '\n';
		writer << filePath.replace(filePath.begin() + replaceBegin, filePath.end(), "");
		writer.close();
	}
}


