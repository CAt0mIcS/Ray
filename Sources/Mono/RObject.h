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
			MemberAccessProxy(MonoObject* pObj, MonoClassField* pField);

			/**
			 * Sets the value of the field
			 */
			template<typename T>
			void operator=(T&& data)
			{
				RAY_MEXPECTS(sizeof(T) == SizeBytes(),
					"[Mono::Object] Size of template parameter ({0}) is larger/smaller than size "
					"of the type you're trying to access ({1})",
					sizeof(T), SizeBytes());

				Set(&data);
			}

			/**
			 * @returns Value of the object in the field
			 */
			template<typename T>
			T Get() const
			{
				RAY_MEXPECTS(sizeof(T) == SizeBytes(),
					"[Mono::Object] Size of template parameter ({0}) is larger/smaller than size "
					"of the type you're trying to access ({1})",
					sizeof(T), SizeBytes());

				return *(T*)Get(sizeof(T)).data();
			}

			/**
			 * Sets the managed type to data
			 */
			void Set(void* data);

			/**
			 * @returns Raw buffer of the data
			 */
			std::vector<char> Get(uint32_t size) const;

			/**
			 * @returns Size in bytes of the managed type
			 */
			uint32_t SizeBytes() const;

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

		/**
		 * @param methodName Name of the method you're trying to access
		 * @returns Callable function
		 */
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
	};
}  // namespace At0::Ray::Mono
