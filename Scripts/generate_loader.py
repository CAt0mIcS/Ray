import sys
import os


loader_file = sys.argv[1] + "/Ray/Core/RRendererLoader"
header_file_directories = [ sys.argv[1] + "/RayRenderer/Core"]
loader_template_cpp = r"""

#include "Rpch.h"
#include "RRendererLoader.h"

#include <RayBase/RException.h>
#include <Windows.h>


namespace At0::Ray
{
	namespace RendererAPI
	{
		Type API;
{function_definitions}
	}  // namespace RendererAPI


	void LoadRenderer(RendererAPI::Type type)
	{
		// RAY_TODO: Make platform independent

		HMODULE lib = nullptr;
		switch (type)
		{
		case RendererAPI::OpenGL: lib = LoadLibraryA("RayRendererOpenGL.dll"); break;
		case RendererAPI::Vulkan: lib = LoadLibraryA("RayRendererVulkan.dll"); break;
		default: ThrowRuntime("Invalid renderer type {0}", (uint32_t)type);
		}

		if (!lib)
		{
			std::string rendererStr;
			switch (type)
			{
			case RendererAPI::OpenGL: rendererStr = "OpenGL"; break;
			case RendererAPI::Vulkan: rendererStr = "Vulkan"; break;
			}

			ThrowRuntime("[Loader] Failed to load {0} renderer", rendererStr);
		}

{function_assignments}

		RendererAPI::API = type;
	}
}  // namespace At0::Ray

"""

loader_template_h = r"""
#pragma once

#include "Ray/RBase.h"
#include <RayRenderer/Core/RCore.h>


namespace At0::Ray
{
	namespace RendererAPI
	{
		enum Type
		{
			OpenGL,
			Vulkan
		};


		extern Type API;
{function_declarations}
	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

"""


sources = list(list())


class Function:
    def __init__(self, function: str):
        self.function = function

        function = function.replace("RR_API ", "")
        self.returnType = function.split(" ")[0]
        self.raw_name = function.split(" ")[1].split("(")[0]
        if "Rr" == self.raw_name[:2]:
            self.name = self.raw_name[2:]
        else:
            self.name = self.raw_name


def load_sources():
    raw_sources = list(list())
    for header_file_dir in header_file_directories:
        header_files = [os.path.join(dp, f) for dp, dn, filenames in os.walk(
            header_file_dir) for f in filenames if os.path.splitext(f)[1] == '.h']

        for header_file in header_files:
            with open(header_file, "r") as reader:
                raw_sources.append(reader.readlines())

        for i in range(0, len(raw_sources)):
            sources.append(list())
            for j in range(0, len(raw_sources[i])):
                sources[i].append("")
                lineStr = raw_sources[i][j]
                if "RR_API " in lineStr:
                    line = 0
                    while ";" not in raw_sources[i][j + line]:
                        sources[i][j] += (raw_sources[i][j + line])
                        line += 1
                    sources[i][j] += (raw_sources[i][j + line])

        # Remove empty lists
        for source in sources:
            while "" in source:
                source.remove("")
            if len(source) == 0:
                sources.remove(source)


def build_function_declarations():
    declarations = list()

    for source in sources:
        for raw_function in source:
            function = Function(raw_function)
            declarations.append(
                f"\t\textern RrPFN{function.name} {function.name};")

    template = ""
    for declaration in declarations:
        template += declaration + "\n"
    template = loader_template_h.replace(
        "{function_declarations}", template)

    with open(loader_file + ".h", "w") as writer:
        writer.write(template)


def build_function_definitions_and_assignments():
    definitions = list()
    assignments = list()

    for source in sources:
        for raw_function in source:
            function = Function(raw_function)
            definitions.append(
                f"\t\tRrPFN{function.name} {function.name} = nullptr;")
            assignments.append(
                f"\t\tRendererAPI::{function.name} = (RrPFN{function.name})GetProcAddress(lib, \"{function.raw_name}\");")

    template_defs = ""
    for definition in definitions:
        template_defs += definition + "\n"
    template_defs = loader_template_cpp.replace(
        "{function_definitions}", template_defs)

    template_assigns = ""
    for assignment in assignments:
        template_assigns += assignment + "\n"
    template_defs = template_defs.replace(
        "{function_assignments}", template_assigns)

    with open(loader_file + ".cpp", "w") as writer:
        writer.write(template_defs)


if __name__ == "__main__":
    load_sources()
    build_function_declarations()
    build_function_definitions_and_assignments()
