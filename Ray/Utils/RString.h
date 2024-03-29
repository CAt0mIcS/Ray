﻿#pragma once

#include "../RBase.h"
#include "../Core/RKeyCodes.h"
#include "RNonCopyable.h"

#include <string>
#include <sstream>
#include <type_traits>

#include <vulkan/vulkan_core.h>
#include <../../Extern/Violent/include/Violent/Utils/VSerialize.h>


namespace At0::Ray
{
	enum class ShaderStage;
	enum class ShaderDataType;

	class RAY_EXPORT String : NonCopyable
	{
	public:
		String() = delete;

		/**
		 * Constructs a string from a key
		 * @param key The key to strinify
		 * @returns The string representation of the key for debugging
		 */
		static std::string Construct(Key key);

		/**
		 * Constructs a string from a mouse button
		 * @param button The mouse button to stringify
		 * @returns The string representation of the mouse button for debugging
		 */
		static std::string Construct(MouseButton button);

		/**
		 * Constructs a string from a Vulkan function result
		 * @param res The result code
		 * @returns The string representation of the resulting code
		 */
		static std::string Construct(VkResult res);

		/**
		 * Constructs a string from a shader stage
		 * @param stage The stage to strinify
		 * @returns The string describing the stage (e.g. VertexShader, FragmentShader, ...)
		 */
		static std::string Construct(ShaderStage stage);

		/**
		 * Constructs a string from a physical device type
		 * @param deviceType The type to strinify
		 * @returns The string describing the type of device
		 */
		static std::string Construct(VkPhysicalDeviceType deviceType);

		/**
		 * Constructs a string from a shader data type
		 * @param type The shader data type
		 * @returns The string equal to the GLSL shader type
		 */
		static std::string Construct(ShaderDataType type);

		/**
		 * Converts a string from UTF16 to UTF8
		 * @param string The string to convert
		 * @returns The converted string
		 */
		static std::string ConvertUtf8(std::wstring_view string);

		/**
		 * Converts a char from UTF18 to UTF8
		 * @param ch The character to convert
		 * @returns The converted character
		 */
		static char ConvertUtf8(wchar_t ch);

		/**
		 * Converts a UTF16 string to a UTF8 string
		 * @param string The string to convert
		 * @returns The converted string
		 */
		static std::wstring ConvertUtf16(std::string_view string);

		/**
		 * Converts a UTF16 char to a UTF8 char
		 * @param string The char to convert
		 * @returns The converted char
		 */
		static wchar_t ConvertUtf16(char ch);

		/**
		 * Gets if a string starts with a token
		 * @param str The string
		 * @param token the supposed start of the string
		 * @returns If a string starts with the token
		 */
		static bool StartsWith(std::string_view str, std::string_view token);

		/**
		 * Gets if a string ends with a token
		 * @param str The string
		 * @param token the supposed end of the string
		 * @returns If a string ends with the token
		 */
		static bool EndsWith(std::string_view str, std::string_view token);

		/**
		 * Gets if a string contains another string
		 * @param str The string
		 * @param token The string that should be conained in str
		 * @returns If a string contains a token
		 */
		static bool Contains(std::string_view str, std::string_view token);

		/**
		 * Splits a string at token. The token will not be included in the result strings
		 * @returns The different parts of a string
		 */
		static std::vector<std::string> Split(std::string str, char sep);

		/**
		 * Searches for [] pair in string and returns the number inside
		 */
		static uint32_t GetIndex(std::string_view str);

		/**
		 * Splits the string at the separator. The separator will not be included in the result
		 * strings
		 * @tparam MaxTokens the maximum number of tokens in the string
		 * @returns The different parts of a string
		 */
		template<uint32_t MaxSeparators>
		static std::array<std::string, MaxSeparators + 1> Split(std::string_view str, char sep)
		{
			std::array<std::string, MaxSeparators + 1> tokens;

			size_t pos = 0;
			uint32_t i = 0;
			while ((pos = str.find(sep)) != std::string::npos)
			{
				tokens[i] = str.substr(0, pos);
				str = str.substr(pos + 1, str.size());
				++i;
			}
			tokens[MaxSeparators] = str;

			return tokens;
		}

		/**
		 * Takes a correct string possibly containing "{0}", "{1}", ... and fills these in with the
		 * corresponding argument
		 * @tparam Args Any list of arguments that have a output operator defined
		 * @param serializedStr The string which should be serialized
		 * @param args... The arguments to insert into the string
		 * @returns The serialized string
		 */
		template<typename... Args>
		static std::string Serialize(std::string serializedStr, Args&&... args);
	};


	template<typename... Args>
	inline std::string String::Serialize(std::string serializedStr, Args&&... args)
	{
		int argCount = 0;
		(Violent::SerializeStringArg(serializedStr, args, argCount), ...);
		return serializedStr;
	}
}  // namespace At0::Ray
