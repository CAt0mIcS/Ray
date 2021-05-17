#pragma once

#include "../RBase.h"
#include "RFunction.h"

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoImage MonoImage;
typedef struct _MonoObject MonoObject;


namespace At0::Ray::Mono
{
	class RAY_EXPORT Object
	{
	public:
		/**
		 * @param className Descriptor for the namespace and the name of the class (e.g.
		 * TestNamespace:TestClass)
		 */
		Object(std::string_view className, MonoDomain* pDomain, MonoImage* pImage);

		Function GetFunction(std::string_view methodName);

	private:
		MonoObject* m_Object = nullptr;
	};
}  // namespace At0::Ray::Mono
