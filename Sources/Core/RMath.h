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
		static constexpr double PI_D = 3.1415926535897932384626433832795;
		static constexpr float PI = 3.141592653f;
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
