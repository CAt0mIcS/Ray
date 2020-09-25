#include "pch.h"
#include "FileHandler.h"

#include "Application.h"

#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/TextBox.h"

#include "GUI/Graphics/Renderer.h"

#include "Util/Exceptions.h"


namespace NPE
{
	void FileHandler::SaveScene(Application& app, int zoom)
	{
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

		tbSceneInfo.AddField<QRD::NUMBER>("zoom");

		for (auto* control : app.m_Window.GetControls())
		{
			const auto& pos = control->GetPos();
			const auto& size = control->GetSize();
			
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::string txt1 = converter.to_bytes(((GUI::TextBox*)control->GetChildren()[0])->GetText().text);
			std::string txt2 = converter.to_bytes(((GUI::TextBox*)control->GetChildren()[1])->GetText().text);

			tbNodeInfo.AddRecord(pos.x, pos.y, size.width, size.height, txt1, txt2);
		}

		for (std::pair<GUI::Button*, GUI::Button*>& line : app.m_Lines)
		{
			tbLines.AddRecord(line.first->GetId(), line.second->GetId());
		}

		tbSceneInfo.AddRecord(zoom);

		m_Db->WriteDb();
	}
	
	void FileHandler::LoadScene(Application& app, int& zoom)
	{
		//needs to be declared in release mode (error C4703: potentially uninitialized local pointer variable used)
		QRD::Table* tbNodeInfo = nullptr;
		QRD::Table* tbSceneInfo = nullptr;
		QRD::Table* tbLines = nullptr;

		tbNodeInfo = &m_Db->GetTable("NodeInfo");
		tbSceneInfo = &m_Db->GetTable("SceneInfo");
		tbLines = &m_Db->GetTable("Lines");

		zoom = std::stoi(tbSceneInfo->GetRecords()[0].GetRecordData()[0]);
		for (auto& record : tbNodeInfo->GetRecords())
		{
			auto& data = record.GetRecordData();
			GUI::Node* node = app.m_Window.AddControl<GUI::Node>(new GUI::Node(&app.m_Window));
			node->SetPos({ std::stof(data[0]), std::stof(data[1]) });
			node->SetSize({ std::stof(data[2]), std::stof(data[3]) });
			node->SetColor(GUI::g_DefaultNodeColor);
			node->Init();

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring txt1 = converter.from_bytes(data[4]);
			std::wstring txt2 = converter.from_bytes(data[5]);

			((GUI::TextBox*)(node->GetChildren()[0]))->SetText(txt1);
			((GUI::TextBox*)(node->GetChildren()[1]))->SetText(txt2);
		}

		for (auto& record : tbLines->GetRecords())
		{
			auto& data = record.GetRecordData();

			std::pair<GUI::Button*, GUI::Button*> line;

			for (auto* control : app.m_Window.GetControls())
			{
				for (auto* child : control->GetChildren())
				{
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
				app.m_Lines.emplace_back(line);
			}
		}
	}

	void FileHandler::CreateDefaultTemplate(int zoom)
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

		tbSceneInfo.AddField<QRD::NUMBER>("zoom");
		tbSceneInfo.AddRecord(zoom);
		m_Db->WriteDb();
	}

	void FileHandler::CreateDatabase(const std::string& filepath)
	{
		m_Db = new QRD::Database(filepath, 3);
	}
	
}


