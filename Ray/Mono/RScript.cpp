#include "Rpch.h"
#include "RScript.h"

#include "Ray/Utils/RString.h"
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
	bool Script::s_MonoInitialized = false;

	Script::Script(std::string_view compiledFilePath)
	{
		MonoInit();

		m_Domain = mono_jit_init(compiledFilePath.data());
		if (!m_Domain)
			ThrowRuntime("[Mono::Script] Failed to initialize mono jit for assembly \"{0}\"",
				compiledFilePath);

		// Open a assembly in the domain
		m_Assembly = mono_domain_assembly_open(m_Domain, compiledFilePath.data());
		if (!m_Assembly)
			ThrowRuntime("[Mono::Script] Failed to open assembly \"{0}\"", compiledFilePath);

		// Get a image from the assembly
		m_Image = mono_assembly_get_image(m_Assembly);
		if (!m_Image)
			ThrowRuntime(
				"[Mono::Script] Failed to get image from the assembly \"{0}\"", compiledFilePath);
	}

	Script Script::FromFile(std::string_view filepath)
	{
		MonoInit();
		if (!Compile(filepath))
			ThrowRuntime("[Mono::Script] Failed to compile script file \"{0}\"", filepath);

		return Script{ std::filesystem::path(filepath).replace_extension("dll").string() };
	}

	Script Script::FromCompiled(std::string_view filepath)
	{
		MonoInit();
		return Script{ filepath };
	}

	StaticFunction Script::GetStaticFunction(std::string_view functionDescriptor) const
	{
		return StaticFunction{ functionDescriptor, m_Image };
	}

	Object Script::GetObject(std::string_view className) const
	{
		return Object{ className, m_Domain, m_Image };
	}

	bool Script::Compile(std::string_view filepath)
	{
		RAY_MEXPECTS(!String::Contains(filepath, " "),
			"[Mono::Script] Filepath to script not allowed to contain spaces");

#ifdef _WIN32
		std::string command =
			String::Serialize("\"C:/Program Files/Mono/bin/mcs\" {0} -target:library", filepath);
#else
		std::string command = String::Serialize("mcs {0} -target:library", filepath);
#endif
		// Compile the script
		return system(command.c_str()) == 0;
	}

	void Script::MonoInit()
	{
		if (!s_MonoInitialized)
		{
#ifdef _WIN32
			mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
			mono_config_parse(NULL);
#endif
			s_MonoInitialized = true;
		}
	}
}  // namespace At0::Ray::Mono
