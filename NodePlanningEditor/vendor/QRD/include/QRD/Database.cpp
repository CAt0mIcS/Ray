#include "pch.h"
#include "Database.h"
#include "Debug/Timer.h"

#include "ExceptionHandlers/Exception.h"


namespace QRD
{
	Database::Database(const std::string& filePath, size_t tableAmnt, size_t fieldAmnt, size_t recordAmnt)
		: m_DBFilePath(filePath), m_Tables{}, m_FieldAllocCount(fieldAmnt), m_RecordAllocCount(recordAmnt)
	{
		bool(*FileExists)(const std::string& filePath) = [](const std::string& filePath)
		{
			struct stat buffer;
			return (stat(filePath.c_str(), &buffer) == 0);
		};

#ifdef _DEBUG
		if (!FileExists(filePath))
		{
			std::stringstream ss;
			ss << "The specified path to the .dbs file is either invalid or doesn't exist\n[Path]:\t\t " << filePath;
			QRD_THROW(FileNotFoundException, ss.str());
		}
#endif

		m_Tables.reserve(tableAmnt);
		ReadDb(fieldAmnt, recordAmnt);
	}

	Table& Database::CreateTable(const std::string& tableName)
	{
		Table table(tableName, (const short)m_Tables.size());

		table.m_Fields.reserve(m_FieldAllocCount);
		table.m_Records.reserve(m_RecordAllocCount);

		m_Tables.emplace_back(std::move(table));
		m_TablePosInVec.emplace_back(&m_Tables[m_Tables.size() - 1], (unsigned int)(m_Tables.size() - 1));

		return m_Tables[m_Tables.size() - 1];
	}

	Table& Database::GetTable(const std::string& tableName)
	{
		for (auto& table : m_Tables)
		{
			if (table.GetTableName() == tableName)
				return table;
		}

#ifdef _DEBUG
		std::stringstream ss;
		ss << "Unable to find table with name " << tableName;
		QRD_THROW(ObjectNotFoundException, ss.str());
#endif
	}

	bool Database::TableExists(const std::string& tableName)
	{
		for (auto& table : m_Tables)
		{
			if (table.GetTableName() == tableName)
				return true;
		}
		return false;
	}

	void Database::DeleteTable(Table& table)
	{
		unsigned int tableId;
		for (const Table& tab : m_Tables)
		{
			if (tab == table)
			{
				tableId = m_Tables[tab.GetTableId()].GetTableId();

				m_Tables.erase(m_Tables.begin() + tab.GetTableId());
				m_TablePosInVec.erase(m_TablePosInVec.begin() + tab.GetTableId());

				UpdateTableIds(tableId);
				return;
			}
		}

#ifdef _DEBUG
		std::stringstream ss;
		ss << "Unable to delete table with name " << table.GetTableName();
		QRD_THROW(ObjectNotFoundException, ss.str());
#endif
	}

	void Database::DeleteTable(const std::string& tableName)
	{
		for (const Table& tab : m_Tables)
		{
			unsigned int tableId;
			if (tab.GetTableName() == tableName)
			{
				tableId = m_Tables[tab.GetTableId()].GetTableId();

				m_Tables.erase(m_Tables.begin() + tab.GetTableId());
				m_TablePosInVec.erase(m_TablePosInVec.begin() + tab.GetTableId());
				UpdateTableIds(tableId);
				return;
			}
		}

#ifdef _DEBUG
		std::stringstream ss;
		ss << "Unable to delete table with name " << tableName;
		QRD_THROW(ObjectNotFoundException, ss.str());
#endif
	}

	void Database::UpdateTableIds(unsigned short indexOfDeletedElement)
	{
		for (unsigned int i = indexOfDeletedElement; i < m_Tables.size(); ++i)
		{
			m_Tables[i].SetTableId(i);
		}
	}

	void Database::ReadDb(size_t fieldAmnt, size_t recordAmnt)
	{
		std::ifstream reader(m_DBFilePath);

		std::string line;
		std::getline(reader, line);
		if (line == "")
			return;

		unsigned short tableNr = std::stoi(line.replace(0, 8, ""));
		if(m_Tables.capacity() < tableNr)
			m_Tables.reserve(tableNr);

		std::getline(reader, line);

		for (unsigned short i = 0; i < tableNr; ++i)
		{
			Table& table = this->CreateTable(line.replace(0, 7, ""));

			table.m_Fields.reserve(fieldAmnt);
			table.m_Records.reserve(recordAmnt);

			ReadFields(reader, line);
			ReadRecords(reader, line);
		}
	}

	void Database::WriteDb()
	{
		std::ofstream writer(m_DBFilePath);
		writer << "TABLES: " << m_Tables.size() << '\n';

		for (auto& table : m_Tables)
		{
			writer << "TABLE: " << table.GetTableName() << "\nFIELDS\n{\n";
			for (const auto& field : table.GetFields())
			{
				switch (field.GetFieldType())
				{
				case NUMBER:
					writer << "    " << field.GetFieldName() << ":INTEGER\n";
					break;
				case TEXT:
					writer << "    " << field.GetFieldName() << ":TEXT\n";
					break;
				}
			}
			
			writer << "}\nRECORDS: " << table.GetRecords().size() << '\n';
			for (const auto& record : table.GetRecords())
			{
				writer << "RECORD\n{\n";
				for (unsigned short j = 0; j < record.GetRecordData().size(); ++j)
				{
					writer << "    " << record.GetRecordData()[j] << '\n';
				}
				writer << "}\n";
			}
		}
		writer.close();
	}

	void Database::ExitDb()
	{
		WriteDb();
	}

	std::string Database::ToString() const
	{
		std::stringstream ssT;
		for (auto& table : m_Tables)
		{
			ssT << table.ToString();
		}

		std::stringstream ss;
		ss << "Database object: "
			<< "\n\t[Database::Location]: " << this
			<< "\n\t[Database::m_DBFilePath]: " << m_DBFilePath
			<< "\n\t[Database::m_Tables]: \n" << ssT.str()
			<< "\n\t[Database::m_FieldAllocCount]: " << m_FieldAllocCount
			<< "\n\t[Database::m_RecordAllocCount]: " << m_RecordAllocCount << '\n';

		return ss.str();
	}

	void Database::ReadFields(std::ifstream& reader, std::string& line)
	{
		Table& table = m_Tables[m_Tables.size() - 1];

		std::getline(reader, line);
		std::getline(reader, line);
		std::getline(reader, line);
		
		while (line != "}")
		{
			char typeIdx = (unsigned char)line.find(':') + 1;

#ifdef _DEBUG
			if (!typeIdx)
			{
				std::stringstream ss;
				ss << "Invalid index for type specifier, index was" << (int)typeIdx;
				QRD_THROW(FileReadException, ss.str());
			}
#endif

			if (line[typeIdx] == 'I')
				table.AddField<NUMBER>(line.replace(line.size() - 8, line.size() - 1, "").replace(0, 4, ""));
			else if (line[typeIdx] == 'T')
				table.AddField<TEXT>(line.replace(line.size() - 5, line.size() - 1, "").replace(0, 4, ""));

			std::getline(reader, line);
		}
	}

	void Database::ReadRecords(std::ifstream& reader, std::string& line)
	{
		Table& table = m_Tables[m_Tables.size() - 1];

		std::getline(reader, line);
		if (line == "")
			return;

		unsigned short recordNr = std::stoi(line.replace(0, 9, ""));

		if (recordNr == 0)
		{
			std::getline(reader, line);
			return;
		}

		std::getline(reader, line);
		std::getline(reader, line);
		std::getline(reader, line);

		for (unsigned short i = 0; i < recordNr; ++i)
		{
			Record& rec = table.AddRecord();
			while (line != "}")
			{
				rec.AddData(line.replace(0, 4, ""));
				std::getline(reader, line);
			}
			std::getline(reader, line);
			if (i < recordNr - 1)
			{
				std::getline(reader, line);
				std::getline(reader, line);
			}
		}
	}

	void Database::ReserveTables(unsigned int tableAmnt)
	{
		m_Tables.reserve(tableAmnt);
	}

	bool Database::operator==(const Database& other)
	{
		return (m_DBFilePath == other.m_DBFilePath && m_Tables == other.m_Tables);
	}

}


