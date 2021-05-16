#include <Utils/RString.h>
#include <iostream>

#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

namespace At0
{
	class MonoScript
	{
	public:
		MonoScript(std::string path)
		{
			if (!s_MonoInitialized)
			{
#ifdef _WIN32
				mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
				mono_config_parse(NULL);
#endif
				s_MonoInitialized = true;
			}

#ifdef _WIN32
			std::string command = "\"C:/Program Files/Mono/bin/mcs\" " + path + " -target:library";
#else
			std::string command = "mcs " + path + " -target:library";
#endif
			// Compile the script
			int sysRet = system(command.c_str());
			if (sysRet != 0)
				std::cerr << "Command \"" << command << "\" failed\n";

			// ----------------------------------------------------------
			// Init a domain
			path.replace(path.end() - 2, path.end(), "dll");
			m_Domain = mono_jit_init(path.c_str());
			if (!m_Domain)
				std::cerr << "mono_jit_init failed\n" << std::endl;

			// Open a assembly in the domain
			m_Assembly = mono_domain_assembly_open(m_Domain, path.c_str());
			if (!m_Assembly)
				std::cerr << "mono_domain_assembly_open failed\n" << std::endl;

			// Get a image from the assembly
			m_Image = mono_assembly_get_image(m_Assembly);
			if (!m_Image)
				std::cerr << "mono_assembly_get_image failed\n" << std::endl;
		}

		template<typename... Params>
		void CallStatic(std::string methodDesc, Params&&... params)
		{
			MonoMethodDesc* typeMethodDesc = mono_method_desc_new(methodDesc.c_str(), NULL);
			if (!typeMethodDesc)
				std::cerr << "mono_method_desc_new failed\n";

			MonoMethod* method;
			method = mono_method_desc_search_in_image(typeMethodDesc, m_Image);
			if (!method)
				std::cerr << "mono_method_desc_serach_in_image failed\n";


			std::unordered_map<std::string, size_t> paramSizeMap;
			(paramSizeMap.emplace(typeid(params).name(), sizeof(params)), ...);

			int run = 0;
			std::unordered_map<std::string, size_t> paramOffsetMap;
			(GetParamOffsetMap(paramOffsetMap, run, params), ...);

			size_t paramsSize = (sizeof(params) + ...);
			void* parameterArray = malloc(paramsSize);
			(memcpy((char*)parameterArray + paramOffsetMap[typeid(params).name()], &params,
				 paramSizeMap[typeid(params).name()]),
				...);

			mono_runtime_invoke(method, nullptr, &parameterArray, nullptr);
			mono_method_desc_free(typeMethodDesc);

			free(parameterArray);
		}

		~MonoScript() { mono_jit_cleanup(m_Domain); }

	private:
		template<typename... Args>
		void GetParamOffsetMap(
			std::unordered_map<std::string, size_t>& map, int& run, Args&&... args)
		{
			map["int"] = 0;
		}

	private:
		inline static bool s_MonoInitialized = false;
		MonoDomain* m_Domain = nullptr;
		MonoAssembly* m_Assembly = nullptr;
		MonoImage* m_Image = nullptr;
	};
}  // namespace At0
