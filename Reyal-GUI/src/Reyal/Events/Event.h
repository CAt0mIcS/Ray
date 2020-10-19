#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
	enum class EventType
	{
		INVALID = 0
	};

	class RL_API Event
	{
	public:
		Event()
			: m_Type(EventType::INVALID) {}

	protected:
		EventType m_Type;
	};
}
