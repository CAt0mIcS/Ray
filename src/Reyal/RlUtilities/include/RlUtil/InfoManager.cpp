#include "utpch.h"
#include "InfoManager.h"

#include "MessageBox.h"


#ifdef _WIN32
#include <DxErr/dxerr.h>
#endif

namespace At0::Util
{
	std::string InfoManager::GetDefaultString(uint16_t line, const std::string_view file)
	{
		std::stringstream oss;
		oss << "[File] " << file << '\n'
			<< "[Line] " << line;

		return oss.str();
	}

#ifdef _WIN32
	HRESULT InfoManager::m_Hr;

	void InfoManager::DxWarn(HRESULT hr, uint16_t line, const std::string_view file, const std::string_view type)
	{
		Reyal::ZMessageBox(type, GetDxString(type, line, file), Reyal::ZMessageBox::Button::Ok, Reyal::ZMessageBox::Icon::Warn, Reyal::ZMessageBox::Flags::DefaultDesktop);
	}

	void InfoManager::DxInfo(HRESULT hr, uint16_t line, const std::string_view file, const std::string_view type)
	{
		Reyal::ZMessageBox(type, GetDxString(type, line, file), Reyal::ZMessageBox::Button::Ok, Reyal::ZMessageBox::Icon::Info, Reyal::ZMessageBox::Flags::DefaultDesktop);
	}

	std::string InfoManager::GetDxErrorString()
	{
		return DXGetErrorStringA(m_Hr);
	}

	std::string InfoManager::GetDxErrorDescription()
	{
		char buff[512];
		DXGetErrorDescriptionA(m_Hr, buff, sizeof(buff));
		return buff;
	}

	std::string InfoManager::GetDxString(const std::string_view type, uint16_t line, std::string_view file)
	{
		std::ostringstream oss;
		oss << type << '\n'
			<< "[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
			<< "[Error String] " << GetDxErrorString() << '\n'
			<< "[Description] " << GetDxErrorDescription() << '\n'
			<< GetDefaultString(line, file);

		return oss.str();
	}
#endif 
	// _WIN32
}



