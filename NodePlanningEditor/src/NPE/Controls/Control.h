#pragma once

#include "NPE/Util/Props.h"


namespace NPE
{
	class Control
	{
	public:
		enum class Type
		{
			INVALID = 0,
			Node,
		};

	public:
		unsigned short GetId() const { return m_Id; }
		Type GetType() const { return m_Type; }

	protected:
		Control(const Type type, const NPoint& pos, const NSize& size, float scale)
			: m_Type(type), m_Id(m_NextId), m_Pos(pos), m_Size(size), m_Scale(scale) { ++m_NextId; }

	protected:
		HWND m_hWnd;
		NPoint m_Pos;
		NSize m_Size;
		float m_Scale;

	private:
		static unsigned short m_NextId;
		unsigned short m_Id;
		Type m_Type;
	};

	inline unsigned short Control::m_NextId = 0;
}



