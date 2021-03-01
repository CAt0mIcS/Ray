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


	Transform::Transform(Float3 translation, Float3 rotation, Float3 scale)
		: Translation(translation), Rotation(rotation), Scale(scale)
	{
	}

	Transform::Transform(Float3 translation, Float3 rotation)
		: Translation(translation), Rotation(rotation)
	{
	}

	Transform::Transform(Float3 translation)
		: Translation(translation), Rotation{ 0.0f, 0.0f, 0.0f }
	{
	}
	Transform::Transform() : Rotation{ 0.0f, 0.0f, 0.0f } {}
	Matrix Transform::AsMatrix() const
	{
		return MatrixScale(Scale) * MatrixRotation(Rotation) * MatrixTranslation(Translation);
	}
	Transform Transform::operator+(const Transform& other)
	{
		Transform transform;
		transform.Translation = Translation + other.Translation;
		transform.Rotation = Rotation + other.Rotation;
		transform.Scale = Scale + other.Scale;
		return transform;
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
