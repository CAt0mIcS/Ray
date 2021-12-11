#include "RFunction.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


namespace At0::Ray::Mono
{
	Function::Function(std::string_view methodName, MonoObject* pObject) : m_Object(pObject)
	{
		MonoClass* monoClass = mono_object_get_class(pObject);
		void* iter = nullptr;
		MonoMethod* method;
		while ((method = mono_class_get_methods(monoClass, &iter)))
		{
			if (strcmp(mono_method_get_name(method), methodName.data()) == 0)
			{
				m_Method = method;
				break;
			}
		}

		if (!m_Method)
			ThrowRuntime("[Mono::Function] Failed to find method \"{0}\"", methodName);
	}

	void Function::Invoke(void** ppParamters) const
	{
		mono_runtime_invoke(m_Method, m_Object, ppParamters, nullptr);
	}
}  // namespace At0::Ray::Mono
