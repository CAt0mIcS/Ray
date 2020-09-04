#include "../pch.h"
#include "Table.h"


namespace QRD
{
    Table::Table(const std::string& tableName, const short id)
        : m_TableName(tableName), m_Records{}, m_Fields{}, m_TableId(id)
    {
    }

    void Table::ValidateCommands(const std::vector<std::string_view>& commands)
    {
        for (auto& command : commands)
        {
            auto it = std::find(command.begin(), command.end(), ':');
            unsigned int idCol = (unsigned int)std::distance(command.begin(), it);

            if (it == command.end() || idCol > command.size() - 2 || idCol < 1)
            {
                std::stringstream ss;
                ss << "Unable to find colon in string or colon position is invalid";
                QRD_THROW(InvalidCommandException, ss.str());
            }

            std::string_view fieldName = command.substr(0, idCol - 1);
            if (!FieldExists(fieldName))
            {
                std::stringstream ss;
                ss << "Unable to find field with name " << fieldName;
                QRD_THROW(ObjectNotFoundException, ss.str());
            }
        }
    }

    Field& Table::GetField(const std::string_view fieldName)
    {
        for (auto& field : m_Fields)
        {
            if (field.GetFieldName() == fieldName)
                return field;
        }

#ifdef _DEBUG
        std::stringstream ss;
        ss << "Unable to find field with name " << fieldName;
        QRD_THROW(ObjectNotFoundException, ss.str());
#endif
    }

    bool Table::FieldExists(const std::string_view fieldName)
    {
        for (auto& field : m_Fields)
        {
            if (field.GetFieldName() == fieldName)
                return true;
        }
        return false;
    }

    void Table::DeleteField(const Field& field)
    {
        unsigned int fieldId = field.GetFieldId();
        m_Fields.erase(m_Fields.begin() + fieldId);
        UpdateFieldIds(fieldId);

        for (auto& rec : m_Records)
        {
            rec.DeleteData(fieldId);
        }
    }

    void Table::DeleteField(const std::string& fieldName)
    {
        unsigned int fieldId = -1;
        for(const auto& field : m_Fields)
        {
            if (field.GetFieldName() == fieldName)
            {
                fieldId = field.GetFieldId();
                m_Fields.erase(m_Fields.begin() + fieldId);
                UpdateFieldIds(fieldId);
                return;
            }
        }

#ifdef _DEBUG
        std::stringstream ss;
        ss << "Unable to delete field with name " << fieldName;
        QRD_THROW(ObjectNotFoundException, ss.str());
#endif
    }

    Record& Table::GetRecordById(const unsigned int id)
    {

#ifdef _DEBUG
        if (id > m_Records.size())
            QRD_THROW(OutOfRangeException, "Vector subscription out of range");
#endif

        return m_Records[id];
    }

    void Table::DeleteRecord(const Record& record)
    {
        unsigned int recId = record.GetRecordId();
        m_Records.erase(m_Records.begin() + recId);
        UpdateRecordIds(recId);
    }

    bool Table::operator==(const Table& other) const
    {
        if(m_TableId == other.m_TableId)
            return true;
        return false;
    }

    void Table::UpdateRecordIds(unsigned int indexOfElement)
    {
        for (unsigned int i = indexOfElement; i < m_Records.size(); ++i)
        {
            m_Records[i].m_RecordId = i;
        }
    }

    void Table::UpdateFieldIds(unsigned short indexOfElement)
    {
        for (unsigned int i = indexOfElement; i < m_Fields.size(); ++i)
        {
            m_Records[i].m_RecordId = i;
        }
    }

    std::string Table::ToString() const
    {

        std::stringstream ssRec;
        std::stringstream ssFields;
        for (auto& rec : m_Records)
        {
            ssRec << rec.ToString();
        }

        for (auto& field : m_Fields)
        {
            ssFields << field.ToString();
        }

        std::stringstream ss;
        ss << "Table object: "
            << "\n\t[Table::Location]: " << this
            << "\n\t[Table::m_TableName]: " << m_TableName
            << "\n\t[Table::m_Records]: \n" << ssRec.str()
            << "\n\t[Table::m_Fields]: \n" << ssFields.str()
            << "\n\t[Table::m_TableId]: " << m_TableId << '\n';

        return ss.str();
    }

}
