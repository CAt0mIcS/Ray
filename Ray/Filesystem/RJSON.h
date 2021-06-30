#pragma once

#include "../RBase.h"

#include <../../Extern/rapidjson/include/rapidjson/document.h>


namespace At0::Ray
{
	class RAY_EXPORT JSON
	{
	public:
		/**
		 * Reads and parses document in filepath
		 */
		JSON(const std::string& filepath);
		JSON();

		const rapidjson::Document& GetDocument() const { return m_Document; }
		rapidjson::Document& GetDocument() { return m_Document; }

		/**
		 * Writes JSON to filepath
		 */
		void Write(const std::string& filepath);

	private:
		rapidjson::Document m_Document;
	};
}  // namespace At0::Ray
