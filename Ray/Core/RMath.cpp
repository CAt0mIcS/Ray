#include "RMath.h"
#include "Devices/RWindow.h"


namespace At0::Ray
{
	Matrix MatrixTranslation(Float3 translation)
	{
		return glm::translate(glm::mat4(1.0f), translation);
	}
	Matrix MatrixTranslation(float x, float y, float z)
	{
		return MatrixTranslation({ x, y, z });
	}
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
	Matrix MatrixScale(Float3 scale)
	{
		return glm::scale(glm::mat4(1.0f), scale);
	}
	Matrix MatrixScale(float x, float y, float z)
	{
		return MatrixScale({ x, y, z });
	}
	Matrix MatrixIdentity()
	{
		return glm::identity<glm::mat4>();
	}

	float Radians(float degrees)
	{
		return glm::radians(degrees);
	}
	double Radians(double degrees)
	{
		return glm::radians(degrees);
	}
	Float2 Normalize(Float2 vec)
	{
		return glm::normalize(vec);
	}
	Float3 Normalize(Float3 vec)
	{
		return glm::normalize(vec);
	}
	Float4 Normalize(Float4 vec)
	{
		return glm::normalize(vec);
	}
	float DotProduct(Float2 x, Float2 y)
	{
		return glm::dot(x, y);
	}
	float DotProduct(Float3 x, Float3 y)
	{
		return glm::dot(x, y);
	}
	float DotProduct(Float4 x, Float4 y)
	{
		return glm::dot(x, y);
	}
	Float3 CrossProduct(Float3 x, Float3 y)
	{
		return glm::cross(x, y);
	}

	RAY_EXPORT Float2 NDCSpaceToScreenSpace(Float2 coords, UInt2 framebufferSize)
	{
		return { (coords.x + 1) * 0.5f * framebufferSize.x,
			(coords.y + 1) * 0.5f * framebufferSize.y };
	}
	RAY_EXPORT Float2 ScreenSpaceToNDCSpace(Float2 coords, UInt2 framebufferSize)
	{
		return { ScreenSpaceToNDCSpaceX(coords.x, framebufferSize.x),
			ScreenSpaceToNDCSpaceY(coords.y, framebufferSize.y) };
	}
	RAY_EXPORT float NDCSpaceToScreenSpaceX(float x, uint32_t framebufferSizeX)
	{
		return (x + 1) * 0.5f * (float)framebufferSizeX;
	}
	RAY_EXPORT float ScreenSpaceToNDCSpaceX(float x, uint32_t framebufferSizeX)
	{
		return 2.f * (x / framebufferSizeX) - 1.f;
	}
	RAY_EXPORT float NDCSpaceToScreenSpaceY(float y, uint32_t framebufferSizeY)
	{
		return (y + 1) * 0.5f * (float)framebufferSizeY;
	}
	RAY_EXPORT float ScreenSpaceToNDCSpaceY(float y, uint32_t framebufferSizeY)
	{
		return y / framebufferSizeY;
	}

	RAY_EXPORT std::vector<uint32_t> GenerateChunks(uint32_t number, uint32_t chunks)
	{
		if (number < chunks)
			return { number };

		// Add fractions from left to right
		float firstFraction = (float)number / (float)chunks;
		std::vector<float> fractions(chunks);
		for (uint32_t i = 1; i <= chunks; ++i)
			fractions[i - 1] = firstFraction * i;

		// Taking the integer parts
		std::vector<uint32_t> integers(chunks);
		for (uint32_t i = 0; i < chunks; ++i)
			integers[i] = std::floor(fractions[i]);

		std::vector<uint32_t> ret(chunks);
		ret[0] = integers[0];
		for (uint32_t i = 1; i < chunks; ++i)
			ret[i] = integers[i] - integers[i - 1];
		return ret;
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
