#pragma once

#include "../pch.h"
#include "../Base.h"
#include "Field.h"
#include "Record.h"

#include "QRD/ExceptionHandlers/Exception.h"


namespace QRD
{
	/**
	* Stores all records and fields in the table and is used for adding/editing/deleting records or fields
	*/
	class QRD_API Table
	{
		friend class Database;
	public:
		/**
		* Adds a new field to the current table
		*
		* @param fieldName is the name of the field
		* @returns a reference to the new field
		* @warning if the vector which stores the field gets resized, all references will be invalid
		*/
		template<DbTypes FIELD_DATA_TYPE>
		Field& AddField(const std::string& fieldName);

		/**
		* Finds the field with specified name
		*
		* @param fieldName is the name of the searched field
		* @returns the found field, if it does not find anything it doesn't return
		*/
		Field& GetField(const std::string_view fieldName);

		/**
		* Checks if field exists in current table
		* 
		* @param fieldName is the name of the field
		* @returns true if the field exists, false otherwise
		*/
		bool FieldExists(const std::string_view fieldName);

		/**
		* Getter for all fields
		*
		* @returns all fields in this table
		*/
		const std::vector<Field>& GetFields() const { return m_Fields; }

		/**
		* Deletes the field and updates indices
		*
		* @param field is the field which will be deleted
		*/
		void DeleteField(const Field& field);

		/**
		* Deletes the field and updates indices
		*
		* @param fieldName is the name of the field which will be deleted
		*/
		void DeleteField(const std::string& fieldName);

		/**
		* Adds a record to the current table
		*
		* @param data is the data corresponding to the fields to insert
		* @returns a reference to the new record
		* @warning if the vector which stores the records gets resized, all references will be invalid
		*/
		template<typename... Args>
		Record& AddRecord(const Args&... data);

		/**
		* Inserts a record at given position
		*
		* @param pos is the position in m_Records where the record will be inserted
		* @param data is the data corresponding to the fields to insert
		* @returns a reference to the new record
		* @warning if the vector which stores the records gets resized, all references will be invalid
		*/
		template<typename... Args>
		Record& InsertRecord(unsigned int pos, const Args&... data);

		/**
		* Finds all records with specific values
		*
		* @param commandStrs are the search specifications, e.g. "field1:MyName", "field3:32"...
		* @returns a vector of all found records with specifications
		*/
		template<typename... Args>
		std::vector<Record*> GetRecordsByValues(const Args&... commandStrs);

		/**
		* Returns the record by id
		*
		* @param id is the id in m_Records
		* @returns the record in m_Records with id
		*/
		Record& GetRecordById(const unsigned int id);

		/**
		* Getter for all records
		*
		* @returns all records in m_Records
		*/
		const auto& GetRecords() const { return m_Records; }

		/**
		* Edits the record
		*
		* @param record is the record to be edited
		* @param newData is the new data to insert into the record
		*/
		template<typename... Args>
		void EditRecord(Record& record, const Args&... newData);

		/**
		* Deletes the record and updates indices
		*
		* @param record is the record to delete
		*/
		void DeleteRecord(const Record& record);

		/**
		* Deletes the found records with search specifications and updates indices
		*
		* @param commandStrs are the search specifications, e.g. "field1:MyName", "field3:32"...
		*/
		template<typename... Args>
		void DeleteRecord(const Args&... commandStrs);

		/**
		* Compares tables
		*
		* @param other is the other table object
		* @returns true if the tables hold the same data else false
		*/
		bool operator==(const Table& other) const;

		/**
		* Getter for table name
		*
		* @returns the table's name
		*/
		const std::string& GetTableName() const { return m_TableName; }

		/**
		* Getter for table id
		*
		* @returns the table's id
		*/
		unsigned short GetTableId() const { return m_TableId; }

		/**
		* Setter for table id, automatically called when deleting a table
		*
		* @param index is the new table id
		*/
		void SetTableId(unsigned int index) { m_TableId = index; }

		/**
		* Updates record id's after deleting a record
		*
		* @param indexOfElement is the index where the element was before its deletion/after its creation
		*/
		void UpdateRecordIds(unsigned int indexOfElement);

		/**
		* Updates field id's after deleting a field
		*
		* @param indexOfElement is the index where the element was before its deletion/after its creation
		*/
		void UpdateFieldIds(unsigned short indexOfElement);

		/**
		* Gives you a string with all information about this table object
		*
		* @returns debug string
		*/
		std::string ToString() const;

	private:
		/**
		* Constructor for Table object
		*
		* @param tableName is the name under which the table will be stored
		* @param id is the index of the table in Database::m_Tables
		*/
		Table(const std::string& tableName, const short id);

		/**
		* Function checks if commands passed as parameter to the FindRecordsByValues function are syntactically correct
		* 
		* @param commands is a list of all commands
		* @throws QRD::InvalidCommandException if one or more commands are invalid
		*/
		void ValidateCommands(const std::vector<std::string_view>& commands);

	private:
		/**
		* The table's name
		*/
		std::string m_TableName;

		/**
		* List of all records in this table, gets populated in Database::Read()
		*/
		std::vector<Record> m_Records;

		/**
		* List of all fields in this table, gets populated in Database::Read()
		*/
		std::vector<Field> m_Fields;

		/**
		* Represents the index of the table
		*/
		unsigned short m_TableId;
	};


	template<DbTypes FIELD_DATA_TYPE>
	inline Field& Table::AddField(const std::string& fieldName)
	{
		Field field(fieldName, FIELD_DATA_TYPE, (unsigned short)m_Fields.size());
		m_Fields.emplace_back(std::move(field));
		return m_Fields[m_Fields.size() - 1];
	}

	template<typename ...Args>
	inline Record& Table::AddRecord(const Args& ...data)
	{
		Record rec((unsigned int)m_Records.size());
		(rec.AddData(data), ...);
		return m_Records.emplace_back(std::move(rec));
	}

	template<typename... Args>
	inline Record& Table::InsertRecord(unsigned int pos, const Args&... data)
	{
		Record rec(pos);
		(rec.AddData(data), ...);
		m_Records.insert(m_Records.begin() + pos, std::move(rec));
		UpdateRecordIds(pos);
		return m_Records[pos];
	}

	template<typename... Args>
	inline std::vector<Record*> Table::GetRecordsByValues(const Args&... commandStrs)
	{
		std::vector<std::string_view> commands{ commandStrs... };

#ifdef _DEBUG
		ValidateCommands(commands);
#endif

		std::vector<Record*> recs{};
		
		std::string_view searchData;
		std::string_view fieldName;
		for (auto& cmd : commands)
		{
			size_t dataIdx;
			dataIdx = cmd.find(":") + 1;
			searchData = cmd.substr(dataIdx);
			fieldName = cmd.substr(0, dataIdx - 1);
		}

		Field& field = this->GetField(fieldName);
		for (size_t i = 0; i < m_Records.size() ; ++i)
		{
			for (auto& cmd : commands)
			{
				if (m_Records[i].GetRecordData()[field.GetFieldId()] == searchData)
				{
					if(std::find(recs.begin(), recs.end(), &m_Records[i]) == recs.end())
						recs.emplace_back(&m_Records[i]);
				}
			}
		}
		return recs;
	}

	template<typename... Args>
	void Table::EditRecord(Record& record, const Args&... newData)
	{
		unsigned int pos = record.GetRecordId();
		DeleteRecord(record);
		InsertRecord(pos, newData...);
	}

	template<typename... Args>
	void Table::DeleteRecord(const Args&... commandStrs)
	{
		std::vector<Record> recs = GetRecordsByValues(commandStrs...);

		for (const auto& rec : recs)
		{
			unsigned int recId = rec.GetRecordId();
			m_Records.erase(m_Records.begin() + recId);
			UpdateRecordIds(recId);
			return;
		}

#ifdef _DEBUG
		if (recs.size() == 0)
		{
			std::stringstream ss;
			ss << "Unable to find record";
			QRD_THROW(ObjectNotFoundException, ss.str());
		}

		std::stringstream ss;
		ss << "Unable to delete record";
		QRD_THROW(ObjectNotFoundException, ss.str());
#endif

	}
}
