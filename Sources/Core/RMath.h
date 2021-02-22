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
		Matrix() : m_Matrix(glm::identity<glm::mat4>()) {}

		static Matrix Perspective(float foV, float aspectRatio, float nearZ, float farZ)
		{
			glm::mat4 matrix = glm::perspective(glm::radians(foV), aspectRatio, nearZ, farZ);
			FlipY(matrix);
			return matrix;
		}

		static Matrix LookAt(
			const Float3& eyePosition, const Float3& center, const Float3& upDirection)
		{
			glm::mat4 matrix = glm::lookAt(eyePosition, center, upDirection);
			return matrix;
		}

		static Matrix RotationRollPitchYaw(float pitch, float roll, float yaw);
		static Matrix RotationRollPitchYaw(const Float3& vec);

		static Matrix Translation(const Float3& translation)
		{
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), translation);
			return matrix;
		}

		static Matrix Translation(float x, float y, float z) { return Translation({ x, y, z }); }

		static Matrix Scale(const Float3& scaling)
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaling);
			return scale;
		}

		static Matrix Scale(float x, float y, float z) { return Scale({ x, y, z }); }

		static Matrix Identity() { return glm::identity<glm::mat4>(); }

		operator glm::mat4 &() { return m_Matrix; }
		operator const glm::mat4 &() const { return m_Matrix; }

	private:
		static void FlipY(glm::mat4& matrix) { matrix[1][1] *= -1.0f; }

	private:
		glm::mat4 m_Matrix{};
	};
}  // namespace At0::Ray
