#pragma once

#include <Ray/Core/RMath.h>
#include <Ray/Scene/RScene.h>


namespace At0::Raditor
{
	class Gizmo
	{
	public:
		enum class Mode
		{
			Disabled = 0,
			Translate,
			Scale,
			Rotate
		};

	public:
		static Gizmo& Get();

		void SetMode(Mode mode) { m_Mode = mode; }
		Mode GetMode() const { return m_Mode; }

	private:
		Gizmo();

	private:
		struct Entities
		{
			Ray::Entity x;
			Ray::Entity y;
			Ray::Entity z;
		};

		Mode m_Mode = Mode::Disabled;
		Entities m_TransEntity;
		Entities m_ScaleEntity;
		Entities m_RotEntity;
	};
}  // namespace At0::Raditor
