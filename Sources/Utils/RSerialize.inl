#pragma once


namespace Internal
{
	template<typename T>
	using BaseType = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

	template<typename, typename = void>
	struct HasOstreamOutputOperator : std::false_type
	{
	};

	template<typename T>
	struct HasOstreamOutputOperator<T,
		std::void_t<decltype(std::declval<std::ostream>() << std::declval<T>())>> : std::true_type
	{
	};

	template<typename, typename = void>
	struct HasWOstreamOutputOperator : std::false_type
	{
	};

	template<typename T>
	struct HasWOstreamOutputOperator<T,
		std::void_t<decltype(std::declval<std::wostream>() << std::declval<T>())>> : std::true_type
	{
	};
}  // namespace Internal


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

	message.replace(message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind),
		String::ConvertUtf8(oss.str()));
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

	message.replace(
		message.begin() + foundIdx, message.begin() + foundIdx + std::size(toFind), oss.str());
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
		static_assert(false, "No output stream can convert the specified type T.");
	}
}
