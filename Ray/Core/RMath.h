#pragma once

#include "../RBase.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <type_traits>


namespace At0::Ray
{
	namespace Math
	{
		template<typename T = float>
		static constexpr T PI = (T)3.1415926535897932384626433832795L;
	}  // namespace Math

	using Float2 = glm::vec<2, float, glm::defaultp>;
	using Float3 = glm::vec<3, float, glm::defaultp>;
	using Float4 = glm::vec<4, float, glm::defaultp>;

	using Int2 = glm::vec<2, int32_t, glm::defaultp>;
	using Int3 = glm::vec<3, int32_t, glm::defaultp>;
	using Int4 = glm::vec<4, int32_t, glm::defaultp>;

	using UInt2 = glm::vec<2, uint32_t, glm::defaultp>;
	using UInt3 = glm::vec<3, uint32_t, glm::defaultp>;
	using UInt4 = glm::vec<4, uint32_t, glm::defaultp>;

	using Bool2 = glm::vec<2, bool, glm::defaultp>;
	using Bool3 = glm::vec<3, bool, glm::defaultp>;
	using Bool4 = glm::vec<4, bool, glm::defaultp>;

	using UIBool2 = glm::vec<2, uint32_t, glm::defaultp>;
	using UIBool3 = glm::vec<3, uint32_t, glm::defaultp>;
	using UIBool4 = glm::vec<4, uint32_t, glm::defaultp>;

	using Double2 = glm::vec<2, double, glm::defaultp>;
	using Double3 = glm::vec<3, double, glm::defaultp>;
	using Double4 = glm::vec<4, double, glm::defaultp>;

	using Matrix2 = glm::mat2;
	using Matrix2x3 = glm::mat2x3;
	using Matrix2x4 = glm::mat2x4;

	using Matrix3 = glm::mat3;
	using Matrix3x2 = glm::mat3x2;
	using Matrix3x4 = glm::mat3x4;

	using Matrix4 = glm::mat4;
	using Matrix4x2 = glm::mat4x2;
	using Matrix4x3 = glm::mat4x3;

	using Matrix = Matrix4;

	using DMatrix2 = glm::dmat2;
	using DMatrix2x3 = glm::dmat2x3;
	using DMatrix2x4 = glm::dmat2x4;

	using DMatrix3 = glm::dmat3;
	using DMatrix3x2 = glm::dmat3x2;
	using DMatrix3x4 = glm::dmat3x4;

	using DMatrix4 = glm::dmat4;
	using DMatrix4x2 = glm::dmat4x2;
	using DMatrix4x3 = glm::dmat4x3;

	using DMatrix = DMatrix4;

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

	RAY_EXPORT Float2 NDCSpaceToScreenSpace(Float2 coords);
	RAY_EXPORT Float2 ScreenSpaceToNDCSpace(Float2 coords);

	RAY_EXPORT float NDCSpaceToScreenSpaceX(float x);
	RAY_EXPORT float ScreenSpaceToNDCSpaceX(float x);

	RAY_EXPORT float NDCSpaceToScreenSpaceY(float y);
	RAY_EXPORT float ScreenSpaceToNDCSpaceY(float y);

	/**
	 * Splits a integer (number) into an array of integers
	 * which are all almost the same size.
	 * Example:
	 *	SplitToIntegers(10, 4) --> [2, 3, 2, 3]		(2 + 3 + 2 + 3 == 10)
	 * @returns Vector with numbers split. Not guaranteed to have size splits
	 */
	RAY_EXPORT std::vector<uint32_t> GenerateChunks(uint32_t number, uint32_t chunks);

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
