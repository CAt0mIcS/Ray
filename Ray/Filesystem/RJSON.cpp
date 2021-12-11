#include "RJSON.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>


namespace At0::Ray
{
	JSON::JSON(const std::string& filepath)
	{
		std::ifstream ifs(filepath);

		RAY_MEXPECTS(ifs.is_open(), "[JSON] Failed to open file \"{0}\"", filepath);

		rapidjson::IStreamWrapper isw(ifs);
		m_Document.ParseStream(isw);

		if (m_Document.GetParseError() != rapidjson::kParseErrorNone)
			ThrowRuntime("[JSON] Failed to parse file \"{0}\" ({1})", filepath,
				(uint32_t)m_Document.GetParseError());
	}

	JSON::JSON() { m_Document.SetObject(); }

	void JSON::Write(const std::string& filepath)
	{
		std::ofstream ofs(filepath);
		rapidjson::OStreamWrapper osw(ofs);

		rapidjson::Writer writer(osw);
		m_Document.Accept(writer);
	}
}  // namespace At0::Ray
