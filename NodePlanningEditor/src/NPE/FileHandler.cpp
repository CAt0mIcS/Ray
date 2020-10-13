#include "pch.h"
#include "FileHandler.h"

#include "Application.h"

#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/TextBox.h"
#include "GUI/Controls/SceneTab.h"

#include "GUI/Graphics/Renderer.h"

#include "Util/Exceptions.h"

#include "GUI/Window/FileWindow.h"


namespace NPE
{
	FileHandler::FileHandler()
		: m_Db(nullptr)
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

	std::string FileHandler::SaveScene(const std::string& filepath, const std::vector<GUI::Control*> controls, const std::vector<Line>& lines, bool saveToNewLocation)
	{
		HCURSOR prevCursor = GetCursor();
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		std::string newFile = "";
		if (saveToNewLocation)
		{
			GUI::FileWindow win;

			GUI::FileWindow::FilterSpecs filterSpecs[] =
			{
				{ L"Database File", L"*.dbs;*.txt" },
				{ L"Any File" , L"*.*" }
			};

			auto result = Util::WideCharToMultiByte(win.ShowSaveDialog(L"Select a Save File", filterSpecs, (unsigned int)std::size(filterSpecs)));
			newFile = result;

			if (result == "")
				return "";

			std::ofstream writer2(result);
			writer2.close();

			m_Db->Clear();
			m_Db->SetFilePath(result);
			CreateDefaultTemplate();

			m_Configs.emplace_back(result, "");
			size_t idx = result.find_last_of('\\');
			result.erase(result.begin() + idx, result.end());
			m_Configs[m_Configs.size() - 1].second = result;

			WriteConfig();
		}

		//clear save file
		m_Db->DeleteTable("VersionInfo");
		m_Db->DeleteTable("NodeInfo");
		m_Db->DeleteTable("SceneInfo");
		m_Db->DeleteTable("Lines");

		//Table creation and setup
		QRD::Table& tbVersionInfo = m_Db->CreateTable("VersionInfo");
		QRD::Table& tbNodeInfo = m_Db->CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Db->CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Db->CreateTable("Lines");

		tbVersionInfo.AddField<QRD::NUMBER>("SaveFileVersion");

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

		tbVersionInfo.AddRecord(Constants::g_SaveFileVersion);
		for (auto* control : controls)
		{
			if (control->GetType() == GUI::Control::Type::Tab)
				continue;

			const auto& pos = control->GetPos();
			const auto& size = control->GetSize();
			
			std::string txt1 = Util::WideCharToMultiByte(((GUI::TextBox*)control->GetChildren()[0])->GetText().text);
			std::string txt2 = Util::WideCharToMultiByte(((GUI::TextBox*)control->GetChildren()[1])->GetText().text);

			if (txt1[txt1.size() - 1] == '\0')
				txt1.erase(txt1.cend() - 1);

			if (txt2[txt2.size() - 1] == '\0')
				txt2.erase(txt2.cend() - 1);

			tbNodeInfo.AddRecord(pos.x, pos.y, size.width, size.height, txt1, txt2);
			NPE_LOG("Saved Node:\nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}\nTitle:\t\"{4}\"\nInfo:\t\"{5}\"\n", pos.x, pos.y, size.width, size.height, txt1, txt2);
		}

		NPE_LOG("\n");

		for (const Line& line : lines)
		{
			if (line.first && line.second)
			{
				tbLines.AddRecord(line.first->GetId(), line.second->GetId());
				NPE_LOG("Saved Line:\nStart:\tx={0} y={1}\nEnd:\tx={2} y={3}\n", line.first->GetPos().x, line.first->GetPos().y, line.second->GetPos().x, line.second->GetPos().y);
			}
			else
				NPE_LOG("Didn't save line because it didn't have a valid end point");
		}
		NPE_LOG("\n\n");
		tbSceneInfo.AddRecord(GUI::Renderer::Get().GetScale().width, GUI::Renderer::Get().GetScale().height);

		m_Db->WriteDb();
		SetCursor(prevCursor);
		return newFile;
	}
	
	void FileHandler::LoadScene(GUI::MainWindow& win, std::vector<Line>& lines)
	{
		GUI::Control::ResetIdCounter(1);
		HCURSOR prevCursor = GetCursor();
		SetCursor(LoadCursor(NULL, IDC_WAIT));

		//needs to be declared in release mode (error C4703: potentially uninitialized local pointer variable used)
		QRD::Table* tbVersionInfo = nullptr;
		QRD::Table* tbNodeInfo = nullptr;
		QRD::Table* tbSceneInfo = nullptr;
		QRD::Table* tbLines = nullptr;

		tbVersionInfo = &m_Db->GetTable("VersionInfo");
		if (std::stof(tbVersionInfo->GetRecordById(0).GetRecordData()[0]) != Constants::g_SaveFileVersion)
		{
			MessageBox(NULL, L"You are using an old save file version, expect unexpected behaviour", L"Invalid Save File", MB_OK);
		}

		tbNodeInfo = &m_Db->GetTable("NodeInfo");
		tbSceneInfo = &m_Db->GetTable("SceneInfo");
		tbLines = &m_Db->GetTable("Lines");

		try
		{
			GUI::Renderer::Get().SetScale(
				{ 
					std::stof(tbSceneInfo->GetRecords().at(0).GetRecordData()[0]), 
					std::stof(tbSceneInfo->GetRecords().at(0).GetRecordData()[1]) 
				}
			);
		}
		catch (std::out_of_range&)
		{
			GUI::Renderer::Get().SetScale({ 1.0f, 1.0f });
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

			NPE_LOG("Loaded Node:\nPos:\tx={0} y={1}\nSize:\twidth={2} height={3}\nTitle:\t\"{4}\"\nInfo:\t\"{5}\"\n", node->GetPos().x, node->GetPos().y, node->GetSize().width, node->GetSize().height, data[4], data[5]);
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
				NPE_LOG("Loaded Line:\nStart:\tx={0} y={1}\nEnd:\tx={2} y={3}\n", line.first->GetPos().x, line.first->GetPos().y, line.second->GetPos().x, line.second->GetPos().y);
			}
		}
		NPE_LOG("\n\n");
		SetCursor(prevCursor);
	}

	void FileHandler::CreateDefaultTemplate()
	{
		QRD::Table& tbVersionInfo = m_Db->CreateTable("VersionInfo");
		QRD::Table& tbNodeInfo = m_Db->CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Db->CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Db->CreateTable("Lines");

		tbVersionInfo.AddField<QRD::NUMBER>("SaveFileVersion");

		tbVersionInfo.AddRecord(Constants::g_SaveFileVersion);

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

	void FileHandler::CreateOrLoadSave(GUI::MainWindow& win, std::vector<GUI::SceneTab*>& tabs)
	{
		bool configFileExists = CreateConfigFile();
		ReadConfig();

		/// <summary>
		/// Delete all invalid paths in configs
		/// </summary>
		bool configChanged = false;
		for (unsigned int i = 0; i < m_Configs.size(); ++i)
		{
			if (!FileExists(m_Configs[i].first))
			{
				NPE_LOG("Save File in Directory {0} doesn't exist, removing it...", m_Configs[i].first);
				m_Configs.erase(m_Configs.begin() + i);
				--i;
				configChanged = true;
			}
		}
		
		/// <summary>
		/// Add default path if file no file in configs exists
		/// </summary>
		bool saveFileExists = true;
		if (m_Configs.size() == 0)
		{
			m_Configs.emplace_back(s_DefaultSaveFileDir + s_DefaultSaveFileName, s_DefaultSaveFileDir);
			saveFileExists = CreateSaveFile(m_Configs[0].first);
		}

		/// <summary>
		/// Update the config file if it contained an invalid path
		/// </summary>
		if (configChanged)
		{
			WriteConfig();
		}

		CreateDatabase(m_Configs[0].first);

		/// <summary>
		/// Create the default template if the file is empty
		/// </summary>
		std::ifstream in(m_Configs[0].first, std::ifstream::ate | std::ifstream::binary);
		size_t size = (size_t)in.tellg();
		in.close();

		if (size == 0)
			CreateDefaultTemplate();

		/// <summary>
		/// Create all the tabs
		/// </summary>
		for (auto& config : m_Configs)
		{
			auto* tab = win.AddControl<GUI::SceneTab>(new GUI::SceneTab(&win));

			Util::NPoint pos{};
			static constexpr Util::NSize size{ 100.0f, 25.0f };
			for (auto* tb : win.GetControls())
			{
				/// <summary>
				/// All Controls are SceneTabs at this stage of the program
				/// </summary>
				pos.x += tb->GetSize().width;
			}

			std::wstring path = Util::MultiByteToWideChar(config.first);
			tab->SetPos(pos);
			tab->SetSize(size);
			tab->SetFile(path);
			tab->SetFontSize(14);

			size_t idx = path.find_last_of('\\');
			path.erase(path.begin(), path.begin() + idx + 1);
			tab->SetText(path);
		}
		((GUI::SceneTab*)(win.GetControls()[0]))->SetActive(true);
	}

	void FileHandler::ChangeScene(const std::string& filepath)
	{
		m_Db->Clear();
		m_Db->SetFilePath(filepath);

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

		m_Db->ReadDb();
	}

	std::string FileHandler::OpenScene(GUI::MainWindow& win, std::vector<Line>& lines)
	{
		GUI::FileWindow fileWin;

		GUI::FileWindow::FilterSpecs filterSpecs[] =
		{
			{ L"Database File", L"*.dbs;*.txt" },
			{ L"Any File" , L"*.*" }
		};

		auto result = Util::WideCharToMultiByte(fileWin.ShowOpenDialog(L"Select file to open", filterSpecs, (unsigned int)std::size(filterSpecs)));

		if (result == "")
			return "";

		ChangeScene(result);

		for (unsigned int i = 0; i < win.GetControls().size(); ++i)
		{
			if (win.GetControls()[i]->GetType() != GUI::Control::Type::Tab)
			{
				delete win.GetControls()[i];
				win.GetControls().erase(win.GetControls().begin() + i);
				--i;
			}
		}
		lines.clear();
		LoadScene(win, lines);

		size_t idx = result.find_last_of('\\');
		m_Configs.emplace_back(result, result.substr(0, idx));
		WriteConfig();

		return result;
	}

	void FileHandler::RemoveTabFromConfig(GUI::SceneTab* active)
	{
		std::string filePath = Util::WideCharToMultiByte(active->GetFilePath());
		if (filePath[filePath.size() - 1] == '\0')
			filePath.erase(filePath.end() - 1);

		std::string fileDir = filePath;
		fileDir.erase(fileDir.begin() + fileDir.find_last_of('\\'), fileDir.end());

		std::pair<std::string, std::string> comp(filePath, fileDir);
		std::vector<std::pair<std::string, std::string>>::iterator activeInConfig = m_Configs.begin();
		for (int i = 0; i < m_Configs.size(); ++i)
		{
			if (m_Configs[i] == comp)
			{
				std::advance(activeInConfig, i);
				m_Configs.erase(activeInConfig);
				WriteConfig();
				break;
			}
		}
	}
	
	void FileHandler::WriteConfig()
	{
		std::ofstream writer(s_ConfigFilePath);
		writer.close();

		for (auto& [filePath, fileDir] : m_Configs)
		{
			//Remove null-termination character because std::ofstream also writes it
			if (filePath[filePath.size() - 1] == '\0')
				filePath.erase(filePath.cend() - 1);

			std::ofstream writer(s_ConfigFilePath, std::ios_base::app);
			writer << filePath << '\n';
			writer.close();
		}
		
	}

	bool FileHandler::CreateConfigFile()
	{
		if (!FileExists(s_ConfigFilePath))
		{
			NPE_LOG("Config file config.cfg doesn't exist, creating it...");

			/**
			* Write temporary path to config file and create it
			*/
			std::ofstream writer(s_ConfigFilePath);
			writer << s_DefaultSaveFileDir << s_DefaultSaveFileName << '\n';
			writer.close();
			return false;
		}
		else
		{
			NPE_LOG("Config file exists");
			return true;
		}
		return false;
	}

	void FileHandler::ReadConfig()
	{
		std::ifstream reader(s_ConfigFilePath);
		std::string line;

		while (std::getline(reader, line))
		{
			if (line.empty() && m_Configs.size() == 0)
			{
				m_Configs.emplace_back(s_DefaultSaveFileDir + s_DefaultSaveFileName, s_DefaultSaveFileDir);
				break;
			}
			else
			{
				size_t id = line.find_last_of('\\');
				m_Configs.emplace_back(line, "");
				line.erase(line.begin() + id, line.end());
				m_Configs[m_Configs.size() - 1].second = line;
			}
		}
		reader.close();
	}

	bool FileHandler::CreateSaveFile(const std::string& filepath)
	{
		size_t idx = filepath.find_last_of('\\');
		std::string filedir = filepath.substr(0, idx);
		bool dirExists = true;
		if (!DirectoryExists(filedir))
		{
			bool dirExists = false;
			if (!CreateDirectoryA(filedir.c_str(), nullptr))
			{
				NPE_THROW_LAST_WND_EXCEPT();
			}
		}

		if (!dirExists || !FileExists(filepath))
		{
			std::ofstream writer(filepath);
			writer << "";
			writer.close();
			return false;
		}
		return true;
	}
	
	bool FileHandler::DirectoryExists(const std::string& dir)
	{
		NPE_LOG("Checking if directory {0} exists...", dir);

		DWORD ftyp = GetFileAttributesA(dir.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;  //something is wrong with your path!

		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;   // this is a directory!

		return false;    // this is not a directory!
	}
	
	bool FileHandler::FileExists(const std::string& filepath)
	{
		struct stat buffer;
		NPE_LOG("Checking if file {0} exists...", filepath);
		return (stat(filepath.c_str(), &buffer) == 0);
	}
	
	void FileHandler::CreateDatabase(const std::string& path)
	{
		m_Db = new QRD::Database(path, 4, 6);
	}
}


