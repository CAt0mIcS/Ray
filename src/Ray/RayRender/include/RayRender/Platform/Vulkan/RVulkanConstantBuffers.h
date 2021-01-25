#pragma once

#include "../../RConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	class RR_API VulkanConstantBuffer : public ConstantBuffer
	{
	public:
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