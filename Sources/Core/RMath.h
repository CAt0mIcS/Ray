#pragma once

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
		inline constexpr double PI_D = 3.1415926535897932384626433832795;
		inline constexpr float PI = 3.141592653f;
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

	Matrix MatrixTranslation(Float3 translation);
	Matrix MatrixTranslation(float x, float y, float z);
	Matrix MatrixRotation(Float3 pitchYawRoll);
	Matrix MatrixRotation(float pitch, float yaw, float roll);
	Matrix MatrixScale(Float3 scale);
	Matrix MatrixScale(float x, float y, float z);
}  // namespace At0::Ray
