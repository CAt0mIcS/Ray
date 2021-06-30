#include "Rpch.h"
#include "RStaticFunction.h"

#include "Ray/Utils/RAssert.h"
#include "Ray/Utils/RException.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


namespace At0::Ray::Mono
{
	StaticFunction::StaticFunction(std::string_view functionDescriptor, MonoImage* pImage)
		: m_Image(pImage)
	{
		MonoMethodDesc* typeMethodDesc = mono_method_desc_new(functionDescriptor.data(), 0);
		if (!typeMethodDesc)
			ThrowRuntime("[Mono::StaticFunction] Failed to create method description from \"{0}\"",
				functionDescriptor);

		m_Method = mono_method_desc_search_in_image(typeMethodDesc, m_Image);
		if (!m_Method)
			ThrowRuntime("[Mono::StaticFunction] Failed to find method \"{0}\" in image",
				functionDescriptor);

		mono_method_desc_free(typeMethodDesc);
	}

	void StaticFunction::Invoke(void** ppParamters) const
	{
		mono_runtime_invoke(m_Method, nullptr, ppParamters, nullptr);
	}
}  // namespace At0::Ray::Mono
