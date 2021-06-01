#pragma once

#include "../RBase.h"
#include "../Core/RMath.h"
#include "RComponent.h"


namespace At0::Ray
{
	class RAY_EXPORT Transform : public Component
	{
	public:
		const Float3& Translation() const { return m_Translation; }
		const Float3& Rotation() const { return m_Rotation; }
		const Float3& Scale() const { return m_Scale; }

		void SetTranslation(Float3 translation);
		void SetRotation(Float3 rotation);
		void SetScale(Float3 scale);

		void Translate(Float3 translation);
		void Rotate(Float3 rotation);
		void Scale(Float3 scale);

		void UpdateMatrix();
		bool HasChanged() const;

		Transform(Entity entity, Float3 translation, Float3 rotation, Float3 scale);
		Transform(Entity entity, Float3 translation, Float3 rotation);
		Transform(Entity entity, Float3 translation);
		Transform(Entity entity);

		const Matrix& AsMatrix();
		void RecalculateCachedMatrix() { m_Changed = true; }

		auto operator<=>(const Transform&) const = default;

	private:
		Matrix m_CachedMatrix = MatrixIdentity();
		Float3 m_Translation{};
		Float3 m_Rotation{};
		Float3 m_Scale{ 1.0f, 1.0f, 1.0f };
		bool m_Changed = true;
		bool m_HasChanged = true;
	};
}  // namespace At0::Ray
