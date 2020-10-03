#include <sstream>
#include "Field.h"


namespace QRD
{
    Field::Field(const std::string& fieldName, const DbTypes& dbType, const unsigned short fieldId)
        : m_FieldName(fieldName), m_FieldType(dbType), m_FieldId(fieldId)
    {
    }

    bool Field::operator==(const Field& other)
    {
        return m_FieldName == other.GetFieldName() && m_FieldType == other.GetFieldType() && m_FieldId == other.GetFieldId();
    }

    std::string Field::ToString() const
	{
		std::string fieldType = "Undefined type";
		switch (m_FieldType)
		{
		case TEXT: fieldType = "Text (" + std::to_string(TEXT) + ')'; break;
		case NUMBER: fieldType = "Number (" + std::to_string(NUMBER) + ')'; break;
		}

		std::stringstream ss;
		ss << "Field object: "
			<< "\n\t[Field::Location]: " << this
			<< "\n\t[Field::m_FieldName]: " << m_FieldName
			<< "\n\t[Field::m_FieldType]: " << fieldType
			<< "\n\t[Field::m_FieldId]: " << m_FieldId << '\n';

		return ss.str();
	}

}

