#include "Rpch.h"
#include "RTransform.h"


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

	Transform::Transform(Float3 translation, Float3 rotation, Float3 scale)
		: m_Translation{ translation }, m_Rotation{ rotation }, m_Scale{ scale }

	{
	}

	Transform::Transform(Float3 translation, Float3 rotation)
		: m_Translation{ translation }, m_Rotation{ rotation }
	{
	}

	Transform::Transform(Float3 translation)
		: m_Translation{ translation }, m_Rotation{ 0.0f, 0.0f, 0.0f }
	{
	}

	Transform::Transform() : m_Rotation{ 0.0f, 0.0f, 0.0f } {}

	Matrix Transform::AsMatrix()
	{
		if (m_Changed)
		{
			m_CachedMatrix = MatrixTranslation(m_Translation) * MatrixRotation(m_Rotation) *
							 MatrixScale(m_Scale);
			m_Changed = false;
		}
		return m_CachedMatrix;
	}

	Transform Transform::operator+(const Transform& other) const
	{
		Transform transform;
		transform.m_Translation = m_Translation + other.m_Translation;
		transform.m_Rotation = m_Rotation + other.m_Rotation;
		transform.m_Scale = m_Scale + other.m_Scale;

		return transform;
	}

	Transform Transform::operator-(const Transform& other) const
	{
		Transform transform;
		transform.m_Translation = m_Translation - other.m_Translation;
		transform.m_Rotation = m_Rotation - other.m_Rotation;
		transform.m_Scale = m_Scale - other.m_Scale;

		return transform;
	}

	Transform Transform::operator*(const Transform& other) const
	{
		Transform transform;
		transform.m_Translation = m_Translation * other.m_Translation;
		transform.m_Rotation = m_Rotation * other.m_Rotation;
		transform.m_Scale = m_Scale * other.m_Scale;

		return transform;
	}

	Transform Transform::operator/(const Transform& other) const
	{
		Transform transform;
		transform.m_Translation = m_Translation / other.m_Translation;
		transform.m_Rotation = m_Rotation / other.m_Rotation;
		transform.m_Scale = m_Scale / other.m_Scale;

		return transform;
	}

	Transform& Transform::operator+=(const Transform& other)
	{
		*this = *this + other;
		return *this;
	}

	Transform& Transform::operator-=(const Transform& other)
	{
		*this = *this - other;
		return *this;
	}

	Transform& Transform::operator*=(const Transform& other)
	{
		*this = *this * other;
		return *this;
	}

	Transform& Transform::operator/=(const Transform& other)
	{
		*this = *this / other;
		return *this;
	}
}  // namespace At0::Ray
