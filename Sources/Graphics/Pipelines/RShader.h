#pragma once

#include "../../RBase.h"

#include <vulkan/vulkan_core.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <optional>

namespace At0::Ray
{
	inline std::optional<std::string> ReadFile(const std::filesystem::path& filepath)
	{
		std::ifstream reader(filepath, std::ios::binary | std::ios::ate);

		size_t filesize = (size_t)reader.tellg();
		if (filesize == std::numeric_limits<uint64_t>::max())
			return std::nullopt;

		std::string file;
		file.resize(filesize);
		reader.seekg(std::ios::beg);

		reader.read(file.data(), filesize);

		return file;
	}


	class RAY_EXPORT Shader
	{
	public:
		Shader();
		~Shader() = default;

		static VkFormat GlTypToVkFormat(int32_t type);
		VkShaderModule CreateShaderModule(const std::filesystem::path& moduleName,
			std::string_view moduleCode, std::string_view preamble, VkShaderStageFlags moduleFlag);
		static VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& filepath);
		static EShLanguage GetEShLanguage(VkShaderStageFlags stageFlags);

		void CreateReflection();
	};
}  // namespace At0::Ray
