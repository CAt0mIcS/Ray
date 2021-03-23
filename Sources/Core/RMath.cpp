#include "Rpch.h"
#include "RMath.h"


namespace At0::Ray
{
	Matrix MatrixTranslation(Float3 translation)
	{
		return glm::translate(glm::mat4(1.0f), translation);
	}
	Matrix MatrixTranslation(float x, float y, float z) { return MatrixTranslation({ x, y, z }); }
	Matrix MatrixRotation(Float3 yawPitchRoll)
	{
		return glm::rotate(
			glm::rotate(glm::rotate(glm::mat4(1.0f), yawPitchRoll.z, glm::vec3(0.0f, 0.0f, 1.0f)),
				yawPitchRoll.y, glm::vec3(0.0f, 1.0f, 0.0f)),
			yawPitchRoll.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	Matrix MatrixRotation(float pitch, float yaw, float roll)
	{
		return MatrixRotation({ pitch, yaw, roll });
	}
	Matrix MatrixScale(Float3 scale) { return glm::scale(glm::mat4(1.0f), scale); }
	Matrix MatrixScale(float x, float y, float z) { return MatrixScale({ x, y, z }); }
	Matrix MatrixIdentity() { return glm::identity<glm::mat4>(); }

	float Radians(float degrees) { return glm::radians(degrees); }
	double Radians(double degrees) { return glm::radians(degrees); }
	Float2 Normalize(Float2 vec) { return glm::normalize(vec); }
	Float3 Normalize(Float3 vec) { return glm::normalize(vec); }
	Float4 Normalize(Float4 vec) { return glm::normalize(vec); }
	float DotProduct(Float2 x, Float2 y) { return glm::dot(x, y); }
	float DotProduct(Float3 x, Float3 y) { return glm::dot(x, y); }
	float DotProduct(Float4 x, Float4 y) { return glm::dot(x, y); }
	Float3 CrossProduct(Float3 x, Float3 y) { return glm::cross(x, y); }

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
	void Transform::Translate(Float3 translation) { m_Translation += translation; }
	void Transform::Rotate(Float3 rotation) { m_Rotation += rotation; }
	void Transform::Scale(Float3 scale) { m_Scale += scale; }
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
			m_CachedMatrix = MatrixScale(m_Scale) * MatrixRotation(m_Rotation) *
							 MatrixTranslation(m_Translation);
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

	inline std::ostream& operator<<(std::ostream& os, const Int2& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Int3& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << ", z=" << data.z << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Int4& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << ", z=" << data.z << ", w=" << data.w << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const UInt2& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const UInt3& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << ", z=" << data.z << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const UInt4& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << ", z=" << data.z << ", w=" << data.w << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Float2& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Float3& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << ", z=" << data.z << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Float4& data)
	{
		os << "[x=" << data.x << ", y=" << data.y << ", z=" << data.z << ", w=" << data.w << "]";
		return os;
	}
}  // namespace At0::Ray
