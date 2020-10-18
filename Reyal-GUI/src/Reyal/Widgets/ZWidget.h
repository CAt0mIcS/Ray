#pragma once

#include "RlBase.h"
#include "RlWin.h"

#include <Util/ZRect.h>


namespace Zeal::Reyal
{
	class RL_API ZWidget
	{
	public:

		const Util::ZRect& GetRect() const { return m_Rect; }

		uint32_t GetID() const { return m_ID; }

		bool operator==(_In_ const ZWidget& other)
		{
			return GetID() == other.GetID();
		}

	protected:
		ZWidget(_In_opt_ ZWidget* parent = nullptr);

	protected:
		uint32_t m_ID;
		ZWidget* m_Parent;

		Util::ZRect m_Rect;

	private:
		static uint32_t s_NextID;
	};
}



