#include "Rpch.h"
#include "RMath.h"


namespace At0::Ray
{
	Matrix MatrixTranslation(Float3 translation)
	{
		return glm::translate(glm::mat4(1.0f), translation);
	}

	Matrix MatrixTranslation(float x, float y, float z) { return MatrixTranslation({ x, y, z }); }
}  // namespace At0::Ray
