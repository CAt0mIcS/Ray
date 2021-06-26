#pragma once

#include "Ray/RBase.h"

#include <unordered_map>
#include <string>

typedef struct _MonoImage MonoImage;
typedef struct _MonoMethod MonoMethod;
typedef struct _MonoObject MonoObject;


namespace At0::Ray::Mono
{
	class RAY_EXPORT FunctionBase
	{
	public:
		/**
		 * Calls the method described in the function descriptor provided in the constructor
		 */
		template<typename... Params>
		void operator()(Params&&... params) const
		{
			// Typeid of parameter type mapped to it's size
			std::unordered_map<std::string, size_t> paramSizeMap;
			(paramSizeMap.emplace(typeid(params).name(), sizeof(params)), ...);

			// Offset in the array of all parameters in the parameter pack
			std::unordered_map<std::string, size_t> paramOffsetMap;
			(GetParamOffsetMap(paramOffsetMap, paramSizeMap, params), ...);

			// Allocate storage to hold all parameters in aligned memory
			size_t paramsSize = 0;
			if constexpr (sizeof...(params) > 0)
				paramsSize = (sizeof(params) + ...);

			void* parameterArray = nullptr;
			if (paramsSize > 0)
			{
				parameterArray = malloc(paramsSize);

				// Copy to the allocated pointer using the offset of every parameter and it's size
				(memcpy((char*)parameterArray + paramOffsetMap[typeid(params).name()], &params,
					 paramSizeMap[typeid(params).name()]),
					...);

				Invoke(&parameterArray);
				free(parameterArray);
			}
			else
				Invoke(nullptr);
		}

		/**
		 * Invokes the method with the parameters
		 * @param ppParameters List of all parameters of the function
		 */
		virtual void Invoke(void** ppParamters) const = 0;

	private:
		template<typename T>
		void GetParamOffsetMap(std::unordered_map<std::string, size_t>& offsetMap,
			std::unordered_map<std::string, size_t>& sizeMap, T&& arg) const
		{
			size_t offset = 0;
			for (auto& [typeID, size] : sizeMap)
			{
				if (typeID == typeid(T).name())
					break;

				offset += sizeMap[typeID];
			}
			offsetMap[typeid(T).name()] = offset;
		}
	};
}  // namespace At0::Ray::Mono
