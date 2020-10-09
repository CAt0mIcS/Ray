#pragma once


#include "../Base.h"

#include <vector>
#include <string>
#include <sstream>


namespace QRD
{
	/**
	* Class which holds the data of one record in a table
	*/
	class QRD_API Record
	{
		friend class Table;
	public:
		/**
		* Constructor for Record
		*
		* @param recordId is the id of the new record
		*/
		Record(unsigned int recordId) : m_RecordId(recordId) {}

		/**
		* Adds data to existing record
		*
		* @param data is the data to add to the record
		*/
		template<typename T/*, class Enable = std::enable_if_t<std::is_convertible_v<T, std::string>>*/>
		void AddData(const T& data);

		/**
		* Getter for record data
		*
		* @returns all the data in this record
		*/
		const std::vector<std::string>& GetRecordData() const { return m_RecordData; }

		/**
		* Compares records
		*
		* @param other is the other Record
		* @returns true if the records hold the same data else false
		*/
		bool operator==(const Record& other) const;

		/**
		* Getter for record id
		*
		* @returns the record's id
		*/
		unsigned int GetRecordId() const { return m_RecordId; }

		/**
		* Deletes the data at fieldId
		*
		* @param fieldId is the id of the field where the data is stored
		*/
		void DeleteData(unsigned short fieldId);

		/**
		* Gives you a string with all information about this record object
		* 
		* @returns debug string
		*/
		std::string ToString() const;

	private:
		/**
		* Default constructor for Record
		*/
		Record() = default;

	private:
		/**
		* Datastructure for data in Record
		*/
		std::vector<std::string> m_RecordData;

		/**
		* Represents id in Table::m_Records
		*/
		unsigned int m_RecordId;
	};


	template<typename T>
	inline void Record::AddData(const T& data)
	{
		std::ostringstream ss;
		ss << data;
		m_RecordData.emplace_back(ss.str());
	}
}



