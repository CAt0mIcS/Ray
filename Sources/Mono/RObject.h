#pragma once

#include "../RBase.h"
#include "RFunction.h"

#include "../Utils/RAssert.h"

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoImage MonoImage;
typedef struct _MonoObject MonoObject;
typedef struct _MonoClassField MonoClassField;


namespace At0::Ray::Mono
{
	class RAY_EXPORT Object
	{
	public:
		class RAY_EXPORT MemberAccessProxy
		{
		public:
			MemberAccessProxy(MonoObject* pObj, MonoClassField* pField)
				: m_Object(pObj), m_Field(pField)
			{
			}

			template<typename T>
			void operator=(T&& data)
			{
				Set(&data);
			}

			template<typename T>
			T Get() const
			{
				return *(T*)Get(sizeof(T)).data();
			}

			void Set(void* data);
			std::vector<char> Get(uint32_t size) const;

		private:
			MonoObject* m_Object;
			MonoClassField* m_Field;
		};

	public:
		/**
		 * @param className Descriptor for the namespace and the name of the class (e.g.
		 * TestNamespace:TestClass)
		 */
		Object(std::string_view className, MonoDomain* pDomain, MonoImage* pImage);
		Object() = default;

		Function GetFunction(std::string_view methodName) const;

		/**
		 * Accesses a member of a class by name
		 */
		MemberAccessProxy operator[](std::string_view memberName) const
		{
			return ValueTypeAccess(memberName);
		}

	private:
		MemberAccessProxy ValueTypeAccess(std::string_view memberName) const;

	private:
		MonoObject* m_Object = nullptr;
		std::vector<char> m_CachedValueTypeData;
	};
}  // namespace At0::Ray::Mono
