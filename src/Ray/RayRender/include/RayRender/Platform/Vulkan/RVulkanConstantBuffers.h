#pragma once

#include "../../RConstantBuffers.h"
#include "Internal/RVulkanGraphicsResources.h"


namespace At0::Ray
{
	template<typename T>
	class RR_API VulkanConstantBuffer : public ConstantBuffer, protected VulkanGraphicsResources
	{
	public:
		VulkanConstantBuffer(const T& data) {}
		VulkanConstantBuffer() {}

		virtual void Bind() override {}
	};


	template<typename T>
	class RR_API VulkanPixelConstantBuffer : public VulkanConstantBuffer<T>
	{
	public:
		using VulkanConstantBuffer<T>::VulkanConstantBuffer;
	};


	template<typename T>
	class RR_API VulkanVertexConstantBuffer : public VulkanConstantBuffer<T>
	{
	public:
		using VulkanConstantBuffer<T>::VulkanConstantBuffer;
	};
}