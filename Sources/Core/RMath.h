#pragma once

#include "../RBase.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <type_traits>


namespace At0::Ray
{
	namespace Math
	{
		template<typename T = float>
		static constexpr T PI = (T)3.1415926535897932384626433832795L;
	}  // namespace Math

	using Int2 = glm::vec<2, int32_t, glm::defaultp>;
	using Int3 = glm::vec<3, int32_t, glm::defaultp>;
	using Int4 = glm::vec<4, int32_t, glm::defaultp>;

	using UInt2 = glm::vec<2, uint32_t, glm::defaultp>;
	using UInt3 = glm::vec<3, uint32_t, glm::defaultp>;
	using UInt4 = glm::vec<4, uint32_t, glm::defaultp>;

	using Float2 = glm::vec<2, float, glm::defaultp>;
	using Float3 = glm::vec<3, float, glm::defaultp>;
	using Float4 = glm::vec<4, float, glm::defaultp>;

	using Matrix = glm::mat4;

	RAY_EXPORT Matrix MatrixTranslation(Float3 translation);
	RAY_EXPORT Matrix MatrixTranslation(float x, float y, float z);
	RAY_EXPORT Matrix MatrixRotation(Float3 pitchYawRoll);
	RAY_EXPORT Matrix MatrixRotation(float pitch, float yaw, float roll);
	RAY_EXPORT Matrix MatrixScale(Float3 scale);
	RAY_EXPORT Matrix MatrixScale(float x, float y, float z);
	RAY_EXPORT Matrix MatrixIdentity();

	RAY_EXPORT float Radians(float degrees);
	RAY_EXPORT double Radians(double degrees);
	RAY_EXPORT Float2 Normalize(Float2 vec);
	RAY_EXPORT Float3 Normalize(Float3 vec);
	RAY_EXPORT Float4 Normalize(Float4 vec);
	RAY_EXPORT float DotProduct(Float2 x, Float2 y);
	RAY_EXPORT float DotProduct(Float3 x, Float3 y);
	RAY_EXPORT float DotProduct(Float4 x, Float4 y);
	RAY_EXPORT Float3 CrossProduct(Float3 x, Float3 y);

	struct RAY_EXPORT Transform
	{
		const Float3& Translation() const { return m_Translation; }
		const Float3& Rotation() const { return m_Rotation; }
		const Float3& Scale() const { return m_Scale; }

		void SetTranslation(Float3 translation);
		void SetRotation(Float3 rotation);
		void SetScale(Float3 scale);

		void Translate(Float3 translation);
		void Rotate(Float3 rotation);
		void Scale(Float3 scale);

		Transform(Float3 translation, Float3 rotation, Float3 scale);
		Transform(Float3 translation, Float3 rotation);
		Transform(Float3 translation);
		Transform();

		Matrix AsMatrix();

		Transform operator+(const Transform& other) const;
		Transform operator-(const Transform& other) const;
		Transform operator*(const Transform& other) const;
		Transform operator/(const Transform& other) const;

		Transform& operator+=(const Transform& other);
		Transform& operator-=(const Transform& other);
		Transform& operator*=(const Transform& other);
		Transform& operator/=(const Transform& other);

		auto operator<=>(const Transform&) const = default;

	private:
		Matrix m_CachedMatrix = MatrixIdentity();
		Float3 m_Translation{};
		Float3 m_Rotation{};
		Float3 m_Scale{ 1.0f, 1.0f, 1.0f };
		bool m_Changed = true;
	};

	RAY_EXPORT Float2 NDCSpaceToScreenSpace(Float2 coords);
	RAY_EXPORT Float2 ScreenSpaceToNDCSpace(Float2 coords);

	RAY_EXPORT float NDCSpaceToScreenSpaceX(float x);
	RAY_EXPORT float ScreenSpaceToNDCSpaceX(float x);

	RAY_EXPORT float NDCSpaceToScreenSpaceY(float y);
	RAY_EXPORT float ScreenSpaceToNDCSpaceY(float y);


	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const Int2& data);

	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const Int3& data);

	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const Int4& data);


	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const UInt2& data);

	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const UInt3& data);

	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const UInt4& data);


	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const Float2& data);

	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const Float3& data);

	RAY_EXPORT std::ostream& operator<<(std::ostream& os, const Float4& data);
}  // namespace At0::Ray
