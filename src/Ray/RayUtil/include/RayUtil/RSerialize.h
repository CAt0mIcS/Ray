#pragma once

#include <string>
#include <sstream>
#include <type_traits>
#include <assert.h>
#include "RTypeConvert.h"

#include "RUBase.h"


namespace At0::Ray::Util
{
	namespace Internal
	{
		template<typename T>
		using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

		template<typename, typename = void>
		struct HasOstreamOutputOperator : std::false_type {};

		template<typename T>
		struct HasOstreamOutputOperator<T, std::void_t<decltype(std::declval<std::ostream>() << std::declval<T>())>> : std::true_type {};

		template<typename, typename = void>
		struct HasWOstreamOutputOperator : std::false_type {};

		template<typename T>
		struct HasWOstreamOutputOperator<T, std::void_t<decltype(std::declval<std::wostream>() << std::declval<T>())>> : std::true_type {};
	}


	template<typename T>
	void WideCharSerialize(std::string& message, T&& arg, int& argCount)
	{
		if (argCount == -1)
			return;

		std::wostringstream oss;
		oss << arg;

		const std::string toFind = "{" + std::to_string(argCount) + "}";
		size_t foundIdx = message.find(toFind);

		if (foundIdx == std::string::npos)
		{
			argCount = -1;
			return;
		}

		message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), WideCharToMultiByte(oss.str()));
		++argCount;
	}

	template<typename T>
	void MultiByteSerialize(std::string& message, T&& arg, int& argCount)
	{
		if (argCount == -1)
			return;

		std::ostringstream oss;
		oss << arg;

		const std::string toFind = "{" + std::to_string(argCount) + "}";
		size_t foundIdx = message.find(toFind);

		if (foundIdx == std::string::npos)
		{
			argCount = -1;
			return;
		}

		message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), oss.str());
		++argCount;
	}

	template<typename T>
	void SerializeStringArg(std::string& message, T&& arg, int& argCount)
	{
		if constexpr (Internal::HasOstreamOutputOperator<Internal::BaseType<T>>::value)
		{
			MultiByteSerialize(message, std::move(arg), argCount);
		}
		else if constexpr (Internal::HasWOstreamOutputOperator<Internal::BaseType<T>>::value)
		{
			WideCharSerialize(message, std::move(arg), argCount);
		}
		else
		{
			// static_assert(false, "No output stream can convert the specified type T.");
			assert(false && "No output stream can convert the specified type T.");
		}
	}

	/// <summary>
	/// Takes a correct string possibly containing "{0}", "{1}", ... and fills these in with the corresponding argument
	/// </summary>
	/// <typeparam name="...Args">Is any list of arguments that have a output operator defined</typeparam>
	/// <param name="serializedStr">Is the string which should be serialized</param>
	/// <param name="...args">Are the arguments to insert into the string</param>
	/// <returns>The serialized string</returns>
	template<typename... Args>
	std::string SerializeString(std::string serializedStr, Args&&... args)
	{
		int argCount = 0;
		(SerializeStringArg(serializedStr, args, argCount), ...);
		return serializedStr;
	}
}

