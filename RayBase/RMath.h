#pragma once

#include "RBase.h"

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

	RAYBASE_API Matrix MatrixTranslation(Float3 translation);
	RAYBASE_API Matrix MatrixTranslation(float x, float y, float z);
	RAYBASE_API Matrix MatrixRotation(Float3 pitchYawRoll);
	RAYBASE_API Matrix MatrixRotation(float pitch, float yaw, float roll);
	RAYBASE_API Matrix MatrixScale(Float3 scale);
	RAYBASE_API Matrix MatrixScale(float x, float y, float z);
	RAYBASE_API Matrix MatrixIdentity();

	RAYBASE_API float Radians(float degrees);
	RAYBASE_API double Radians(double degrees);
	RAYBASE_API Float2 Normalize(Float2 vec);
	RAYBASE_API Float3 Normalize(Float3 vec);
	RAYBASE_API Float4 Normalize(Float4 vec);
	RAYBASE_API float DotProduct(Float2 x, Float2 y);
	RAYBASE_API float DotProduct(Float3 x, Float3 y);
	RAYBASE_API float DotProduct(Float4 x, Float4 y);
	RAYBASE_API Float3 CrossProduct(Float3 x, Float3 y);

	RAYBASE_API Float2 NDCSpaceToScreenSpace(Float2 coords, Float2 windowSize);
	RAYBASE_API Float2 ScreenSpaceToNDCSpace(Float2 coords, Float2 windowSize);

	RAYBASE_API float NDCSpaceToScreenSpaceX(float x, float windowSizeX);
	RAYBASE_API float ScreenSpaceToNDCSpaceX(float x, float windowSizeX);

	RAYBASE_API float NDCSpaceToScreenSpaceY(float y, float windowSizeY);
	RAYBASE_API float ScreenSpaceToNDCSpaceY(float y, float windowSizeY);

	/**
	 * Splits a integer (number) into an array of integers
	 * which are all almost the same size.
	 * Example:
	 *	SplitToIntegers(10, 4) --> [2, 3, 2, 3]		(2 + 3 + 2 + 3 == 10)
	 * @returns Vector with numbers split. Not guaranteed to have size splits
	 */
	RAYBASE_API std::vector<uint32_t> GenerateChunks(uint32_t number, uint32_t chunks);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const Int2& data);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const Int3& data);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const Int4& data);


	RAYBASE_API std::ostream& operator<<(std::ostream& os, const UInt2& data);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const UInt3& data);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const UInt4& data);


	RAYBASE_API std::ostream& operator<<(std::ostream& os, const Float2& data);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const Float3& data);

	RAYBASE_API std::ostream& operator<<(std::ostream& os, const Float4& data);
}  // namespace At0::Ray
