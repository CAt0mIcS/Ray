#pragma once

#include "RlUBase.h"

#include <string>


namespace At0::Ray
{

	class RU_API ZMessageBox
	{
	public:
		enum class Button
		{
			Ok = 0,
			OkCancel = 1
		};

		enum class Icon
		{
			None = 0,
			Error,
			Warn,
			Info
		};

		enum class Flags
		{
			None = 0,
			DefaultDesktop = 1
		};

	public:
		ZMessageBox(const std::string_view title, const std::string_view text, Button buttons = Button::Ok, Icon icon = Icon::None, Flags flags = Flags::None);
	};
}