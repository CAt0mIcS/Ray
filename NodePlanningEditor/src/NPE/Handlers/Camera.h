#pragma once

#include "NPE/Util/Props.h"



namespace NPE
{
	class Camera
	{
	public:
		Camera()
			: m_Scale(1.0f) {}

		float GetScale() const { return m_Scale; }

		void SetScale(float scale) { m_Scale = scale; }

	private:
		float m_Scale;
	};
}



