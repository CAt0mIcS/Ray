#include "../RBase.h"

#include <sstream>
#include <string>


namespace At0::Ray
{
	class RAY_EXPORT OStringStream
	{
	public:
		template<typename T>
		OStringStream& operator<<(T&& value)
		{
			m_Oss << value;
			return *this;
		}

		std::string toStr() { return m_Oss.str(); }

	private:
		std::ostringstream m_Oss;
	};
}  // namespace At0::Ray