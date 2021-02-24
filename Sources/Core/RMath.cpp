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
}  // namespace At0::Ray
