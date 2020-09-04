#pragma once

#include "../pch.h"
#include "../Base.h"


namespace QRD
{
	/**
	* Field class which stores the name and type of the field
	*/
	class QRD_API Field
	{
		friend class Table;
	public:
		/**
		* Constructor for field object
		*
		* @param fieldName is the name of the field
		* @param dbType is the type of data the field will store
		* @param fieldId is the id of the field;
		*/
		Field(const std::string& fieldName, const DbTypes& dbType, const unsigned short fieldId);

		/**
		* Getter for field name
		*
		* @returns the field's name
		*/
		const std::string& GetFieldName() const { return m_FieldName; }

		/**
		* Getter for field id
		*
		* @returns the field's id
		*/
		unsigned short GetFieldId() const { return m_FieldId; }

		/**
		* Getter for field type
		*
		* @returns the field's type
		*/
		const DbTypes GetFieldType() const { return m_FieldType; }
	
		/**
		* Comparison operator, compares all data in field
		* 
		* @param other is the other Field object to compare to
		* @returns true if both field objects hold the same data, else false
		*/
		bool operator==(const Field& other);

		/**
		* Gives you a string with all information about this field object
		*
		* @returns debug string
		*/
		std::string ToString() const;

	private:
		/**
		* Field class which stores the name and type of the field
		*/
		Field() = default;

	private:
		/**
		* The field's name
		*/
		std::string m_FieldName;

		/**
		* The field's type
		*/
		DbTypes m_FieldType;

		/**
		* Represents id in Table::m_Fields
		*/
		unsigned short m_FieldId;
	};

}

