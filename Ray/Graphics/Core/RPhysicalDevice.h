#pragma once

#include "Ray/RBase.h"
#include "RayBase/RNonCopyable.h"

#include <RayRenderer/Core/RPhysicalDevice.h>


namespace At0::Ray
{
	class RAY_EXPORT PhysicalDevice : NonCopyable
	{
	public:
		PhysicalDevice();
		~PhysicalDevice() = default;
		operator const RrPhysicalDevice&() const { return m_Device; }

		const RrPhysicalDevice& GetPhysicalDevice() const { return m_Device; }
		const RrPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }
		const RrPhysicalDeviceMemoryProperties& GetMemoryProperties() const;
		const RrPhysicalDeviceProperties& GetProperties() const { return m_Properties; }

		uint32_t FindMemoryType(uint32_t typeFilter, RrMemoryPropertyFlags properties) const;

		/**
		 * @returns If the GPU supports a combination of memory properties
		 */
		bool HasMemoryProperties(RrMemoryPropertyFlags memProps) const;

	private:
		RrPhysicalDevice m_Device = nullptr;

		RrPhysicalDeviceFeatures m_Features{};
		RrPhysicalDeviceMemoryProperties m_MemoryProperties{};
		RrPhysicalDeviceProperties m_Properties;
	};
}  // namespace At0::Ray
