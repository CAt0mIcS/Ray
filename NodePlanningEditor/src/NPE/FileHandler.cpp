#include "pch.h"
#include "FileHandler.h"

#include "Application.h"

#include "GUI/Controls/Node.h"
#include "GUI/Controls/Button.h"

#include "GUI/Graphics/Renderer.h"

#include "Util/Exceptions.h"


namespace NPE
{
	void FileHandler::SaveScene(Application& app)
	{
		//clear save file
		m_Db->DeleteTable("NodeInfo");
		m_Db->DeleteTable("SceneInfo");
		m_Db->DeleteTable("Lines");
		m_Db->WriteDb();

		//Table creation and setup
		QRD::Table& tbNodeInfo = m_Db->CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Db->CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Db->CreateTable("Lines");

		tbNodeInfo.AddField<QRD::NUMBER>("x");
		tbNodeInfo.AddField<QRD::NUMBER>("y");
		tbNodeInfo.AddField<QRD::NUMBER>("width");
		tbNodeInfo.AddField<QRD::NUMBER>("height");

		tbLines.AddField<QRD::NUMBER>("ID2");
		tbLines.AddField<QRD::NUMBER>("ID1");

		tbSceneInfo.AddField<QRD::NUMBER>("zoom");

		for (auto* control : app.m_Window.GetControls())
		{
			const auto& pos = control->GetPos();
			const auto& size = control->GetSize();
			tbNodeInfo.AddRecord(pos.x, pos.y, size.width, size.height);
		}

		for (std::pair<GUI::Button*, GUI::Button*>& line : app.m_Lines)
		{
			tbLines.AddRecord(line.first->GetId(), line.second->GetId());
		}

		tbSceneInfo.AddRecord(app.m_Zoom);

		m_Db->ExitDb();
	}
	
	void FileHandler::LoadScene(Application& app)
	{
		//needs to be declared in release mode (error C4703: potentially uninitialized local pointer variable used)
		QRD::Table* tbNodeInfo = nullptr;
		QRD::Table* tbSceneInfo = nullptr;
		QRD::Table* tbLines = nullptr;

		tbNodeInfo = &m_Db->GetTable("NodeInfo");
		tbSceneInfo = &m_Db->GetTable("SceneInfo");
		tbLines = &m_Db->GetTable("Lines");

		app.m_Zoom = std::stoi(tbSceneInfo->GetRecords()[0].GetRecordData()[0]);
		for (auto& record : tbNodeInfo->GetRecords())
		{
			auto& data = record.GetRecordData();
			GUI::Node* node = app.m_Window.AddControl<GUI::Node>(new GUI::Node(&app.m_Window));
			node->SetPos({ std::stof(data[0]), std::stof(data[1]) });
			node->SetSize({ std::stof(data[2]), std::stof(data[3]) });
			node->SetColor(GUI::g_DefaultNodeColor);
			node->Init();
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

	void FileHandler::CreateDefaultTemplate(Application& app)
	{
		QRD::Table& tbNodeInfo = m_Db->CreateTable("NodeInfo");
		QRD::Table& tbSceneInfo = m_Db->CreateTable("SceneInfo");
		QRD::Table& tbLines = m_Db->CreateTable("Lines");

		tbNodeInfo.AddField<QRD::NUMBER>("x");
		tbNodeInfo.AddField<QRD::NUMBER>("y");
		tbNodeInfo.AddField<QRD::NUMBER>("width");
		tbNodeInfo.AddField<QRD::NUMBER>("height");

		tbLines.AddField<QRD::NUMBER>("ID2");
		tbLines.AddField<QRD::NUMBER>("ID1");

		tbSceneInfo.AddField<QRD::NUMBER>("zoom");
		tbSceneInfo.AddRecord(app.m_Zoom);
		m_Db->WriteDb();
	}

	void FileHandler::CreateDatabase(const std::string& filepath)
	{
		m_Db = new QRD::Database(filepath, 3);
	}
	
}


