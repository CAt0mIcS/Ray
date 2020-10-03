#include <sstream>
#include "Record.h"


namespace QRD
{
    bool Record::operator==(const Record& other) const
    {
        return m_RecordData == other.GetRecordData() && m_RecordId == other.GetRecordId();
    }

    void Record::DeleteData(unsigned short fieldId)
    {
        m_RecordData.erase(m_RecordData.begin() + fieldId);
    }

    std::string Record::ToString() const
    {
        std::stringstream ss2;
        for (unsigned int i = 0; i < m_RecordData.size(); ++i)
        {
            ss2 << "\n\t  [" << i << "]: " << m_RecordData[i];
        }

        std::stringstream ss;
        ss << "Record object: "
            << "\n\t[Record::Location]: " << this
            << "\n\t[Record::m_Data]: " << ss2.str()
            << "\n\t[Record::m_RecordId]: " << m_RecordId << '\n';

        return ss.str();
    }
}

