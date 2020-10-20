#pragma once

#include "RlBase.h"

#include <Util/ZPoint.h>


namespace Zeal::Reyal
{
	class RL_API Mouse
	{
	public:
		Mouse();

		bool IsLeftPressed() const;
		bool IsMiddlePressed() const;
		bool IsRightPressed() const;

		const Util::Point& GetMousePos() const;
		void SetMousePos(const Util::Point& mousePos);

	private:
		bool m_IsLeftPressed;
		bool m_IsMiddlePressed;
		bool m_IsRightPressed;

		Util::Point m_MousePos;
	};
}



