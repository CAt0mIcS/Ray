#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	class RL_API Layer
	{
	public:
		Layer(const std::string_view name);

		virtual void OnUpdate() = 0;

	private:
		std::string m_Name;
	};
}



