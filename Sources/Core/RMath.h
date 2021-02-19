#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <type_traits>


namespace At0::Ray
{
	using Int2 = glm::vec<2, int32_t, glm::defaultp>;
	using Int3 = glm::vec<3, int32_t, glm::defaultp>;
	using Int4 = glm::vec<4, int32_t, glm::defaultp>;

	using UInt2 = glm::vec<2, uint32_t, glm::defaultp>;
	using UInt3 = glm::vec<3, uint32_t, glm::defaultp>;
	using UInt4 = glm::vec<4, uint32_t, glm::defaultp>;

	using Float2 = glm::vec<2, float, glm::defaultp>;
	using Float3 = glm::vec<3, float, glm::defaultp>;
	using Float4 = glm::vec<4, float, glm::defaultp>;

	struct Matrix
	{
	public:
		Matrix(glm::mat4 mat) : m_Matrix(std::move(mat)) {}

		static glm::mat4 RotationRollPitchYaw(float pitch, float roll, float yaw);
		static glm::mat4 RotationRollPitchYaw(const Float3& vec);

		static glm::mat4 Translation(const Float3& translation)
		{
			return glm::translate(glm::mat4(1.0f), translation);
		}

		static glm::mat4 Translation(float x, float y, float z) { return Translation({ x, y, z }); }

		static glm::mat4 Scale(const Float3& scaling)
		{
			return glm::scale(glm::mat4(1.0f), scaling);
		}

		static glm::mat4 Scale(float x, float y, float z) { return Scale({ x, y, z }); }

		operator glm::mat4 &() { return m_Matrix; }
		operator const glm::mat4 &() const { return m_Matrix; }

	private:
		glm::mat4 m_Matrix;
	};
}  // namespace At0::Ray