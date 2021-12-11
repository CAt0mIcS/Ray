#pragma once

#include "../RBase.h"
#include "RStaticFunction.h"
#include "RObject.h"

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoImage MonoImage;


namespace At0::Ray::Mono
{
	class RAY_EXPORT Script
	{
	public:
		Script(std::string_view compiledFilePath);
		Script() = default;

		static Script FromFile(std::string_view filepath);
		static Script FromCompiled(std::string_view filepath);

		/**
		 * @param functionDescriptor Mono-compatible function descriptor
		 */
		StaticFunction GetStaticFunction(std::string_view functionDescriptor) const;

		/**
		 * @param className Descriptor for the namespace and the name of the class (e.g.
		 * TestNamespace:TestClass)
		 */
		Object GetObject(std::string_view className) const;

	public:
		static bool Compile(std::string_view filepath);

	private:
		static void MonoInit();

	private:
		static bool s_MonoInitialized;

		// RAY_TODO: Sharing between coppies
		MonoDomain* m_Domain = nullptr;
		MonoAssembly* m_Assembly = nullptr;
		MonoImage* m_Image = nullptr;
	};
}  // namespace At0::Ray::Mono
