#include "Rpch.h"
#include "RTransform.h"

#include "RayBase/RLogger.h"


namespace At0::Ray
{
	void Transform::SetTranslation(Float3 translation)
	{
		m_Translation = translation;
		m_Changed = true;
	}

	void Transform::SetRotation(Float3 rotation)
	{
		m_Rotation = rotation;
		m_Changed = true;
	}

	void Transform::SetScale(Float3 scale)
	{
		m_Scale = scale;
		m_Changed = true;
	}

	void Transform::Translate(Float3 translation)
	{
		m_Translation += translation;
		m_Changed = true;
	}

	void Transform::Rotate(Float3 rotation)
	{
		m_Rotation += rotation;
		m_Changed = true;
	}

	void Transform::Scale(Float3 scale)
	{
		m_Scale += scale;
		m_Changed = true;
	}

	void Transform::UpdateMatrix()
	{
		if (m_Changed)
		{
			const float c3 = cos(m_Rotation.z);
			const float s3 = sin(m_Rotation.z);
			const float c2 = cos(m_Rotation.x);
			const float s2 = sin(m_Rotation.x);
			const float c1 = cos(m_Rotation.y);
			const float s1 = sin(m_Rotation.y);
			// clang-format off
			 m_CachedMatrix = Matrix{
				{
					m_Scale.x * (c1 * c3 + s1 * s2 * s3),
					m_Scale.x * (c2 * s3),
					m_Scale.x * (c1 * s2 * s3 - c3 * s1),
					0.0f,
				},
				{
					m_Scale.y * (c3 * s1 * s2 - c1 * s3),
					m_Scale.y * (c2 * c3),
					m_Scale.y * (c1 * c3 * s2 + s1 * s3),
					0.0f,
				},
				{
					m_Scale.z * (c2 * s1),
					m_Scale.z * (-s2),
					m_Scale.z * (c1 * c2),
					0.0f,
				},
				{ m_Translation.x, m_Translation.y, m_Translation.z, 1.0f } };
			// clang-format on

			m_Changed = false;
			m_HasChanged = true;
		}
		else
			m_HasChanged = false;
	}

	bool Transform::HasChanged() const
	{
		bool parentChanged = false;
		if (GetEntity().HasParent())
			parentChanged = GetEntity().GetParent().Get<Transform>().HasChanged();

		return m_HasChanged || parentChanged;
	}

	Transform::Transform(Entity entity, Float3 translation, Float3 rotation, Float3 scale)
		: Component(entity), m_Translation{ translation }, m_Rotation{ rotation }, m_Scale{ scale }

	{
	}

	Transform::Transform(Entity entity, Float3 translation, Float3 rotation)
		: Component(entity), m_Translation{ translation }, m_Rotation{ rotation }
	{
	}

	Transform::Transform(Entity entity, Float3 translation)
		: Component(entity), m_Translation{ translation }, m_Rotation{ 0.0f, 0.0f, 0.0f }
	{
	}

	Transform::Transform(Entity entity) : Component(entity), m_Rotation{ 0.0f, 0.0f, 0.0f } {}

	const Matrix& Transform::AsMatrix()
	{
		//#ifndef NDEBUG
		if (m_Changed)
		{
			UpdateMatrix();
			// Log::Error("[Transform] Matrix should've already been recalculated");
		}
		//#endif
		return m_CachedMatrix;
	}
}  // namespace At0::Ray
