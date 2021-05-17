#include "Rpch.h"
#include "RObject.h"

#include "Utils/RString.h"
#include "Utils/RException.h"
#include "Utils/RAssert.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


namespace At0::Ray::Mono
{
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
			RAY_THROW_RUNTIME("[Mono::Object] Failed to get class from name \"{0}\"", className);

		// Doesn't call the constructor
		m_Object = mono_object_new(pDomain, monoClass);
		// Calls constructor
		mono_runtime_object_init(m_Object);
	}

	Function Object::GetFunction(std::string_view methodName) const
	{
		return Function{ methodName, m_Object };
	}
}  // namespace At0::Ray::Mono
