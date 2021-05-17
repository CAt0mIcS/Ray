#include "Rpch.h"
#include "RTransform.h"

#include "Utils/RLogger.h"


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
			m_CachedMatrix = MatrixTranslation(m_Translation) * MatrixRotation(m_Rotation) *
							 MatrixScale(m_Scale);
			m_Changed = false;
		}
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
