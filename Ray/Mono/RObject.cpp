#include "Rpch.h"
#include "RObject.h"

#include "Ray/Utils/RString.h"
#include "Ray/Utils/RException.h"
#include "Ray/Utils/RAssert.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


namespace At0::Ray::Mono
{
	Object::MemberAccessProxy::MemberAccessProxy(MonoObject* pObj, MonoClassField* pField)
		: m_Object(pObj), m_Field(pField)
	{
	}

	void Object::MemberAccessProxy::Set(void* data)
	{
		mono_field_set_value(m_Object, m_Field, data);
	}

	std::vector<char> Object::MemberAccessProxy::Get(uint32_t size) const
	{
		std::vector<char> data(size);
		mono_field_get_value(m_Object, m_Field, data.data());
		return data;
	}

	uint32_t Object::MemberAccessProxy::SizeBytes() const
	{
		int alignment = 0;
		return mono_type_size(mono_field_get_type(m_Field), &alignment);
	}

	Object::Object(std::string_view className, MonoDomain* pDomain, MonoImage* pImage)
	{
		MonoClass* monoClass = nullptr;

		auto strings = String::Split(className.data(), ':');
		RAY_MEXPECTS(
			strings.size() <= 2, "[Mono::Object] Only one namespace is currently supported");

		if (strings.size() == 2)
			// Namespace
			monoClass = mono_class_from_name(pImage, strings[0].c_str(), strings[1].c_str());
		else
			// No namespace
			monoClass = mono_class_from_name(pImage, "", strings[0].c_str());
		if (!monoClass)
			ThrowRuntime("[Mono::Object] Failed to get class from name \"{0}\"", className);

		// Doesn't call the constructor
		m_Object = mono_object_new(pDomain, monoClass);
		// Calls constructor
		mono_runtime_object_init(m_Object);
	}

	Function Object::GetFunction(std::string_view methodName) const
	{
		return Function{ methodName, m_Object };
	}

	Object::MemberAccessProxy Object::operator[](std::string_view memberName) const
	{
		MonoClass* monoClass = mono_object_get_class(m_Object);
		MonoClassField* field = mono_class_get_field_from_name(monoClass, memberName.data());

		if (!field)
			ThrowRuntime("[Mono::Object] Could not find member \"{0}\" in class \"{0}\"",
				memberName, mono_class_get_name(monoClass));

		return MemberAccessProxy{ m_Object, field };
	}
}  // namespace At0::Ray::Mono
