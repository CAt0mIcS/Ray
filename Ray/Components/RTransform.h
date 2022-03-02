#pragma once

#include "RComponent.h"
#include "../Core/RMath.h"


namespace At0::Ray
{
	class RAY_EXPORT Transform : public Component
	{
	public:
		const Float3& Translation() const { return m_Translation; }
		const Float3& Rotation() const { return m_Rotation; }
		const Float3& Scale() const { return m_Scale; }

		Transform& SetTranslation(Float3 translation);
		Transform& SetRotation(Float3 rotation);
		Transform& SetScale(Float3 scale);

		Transform& Translate(Float3 translation);
		Transform& Rotate(Float3 rotation);
		Transform& Scale(Float3 scale);

		void UpdateMatrix();
		bool HasChanged() const;

		Transform(Entity entity, Float3 translation, Float3 rotation, Float3 scale);
		Transform(Entity entity, Float3 translation, Float3 rotation);
		Transform(Entity entity, Float3 translation);
		Transform(Entity entity);

		const Matrix& AsMatrix();
		const Matrix& GetCachedMatrix() const { return m_CachedMatrix; }
		void RecalculateCachedMatrix() { m_Changed = true; }

		Matrix3 GetNormalMatrix() const;

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
