#pragma once

#include "../RBase.h"
#include "../Devices/RKeyCodes.h"
#include "RNonCopyable.h"

#include <string>

enum VkResult;


namespace At0::Ray
{

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
	};
}  // namespace At0::Ray
