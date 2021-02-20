﻿#pragma once

#include "../RBase.h"
#include "../Utils/RSerialize.h"

#include <exception>
#include <string>
#include <stdint.h>

enum VkResult;


namespace At0::Ray
{
	class RAY_EXPORT Exception : public std::exception
	{
	public:
		Exception(uint16_t line, const char* file);
		std::string GetDefaultString() const;
		virtual const char* GetType() const { return "Exception"; };
		virtual const char* what() const noexcept override;
		virtual ~Exception() = default;

	protected:
		mutable std::string m_WhatBuffer;
		unsigned int m_Line;
		const char* m_File;
	};

	class RAY_EXPORT RuntimeException : public Exception
	{
	public:
		RuntimeException(const char* message, uint16_t line, const char* file);
		virtual const char* GetType() const { return "Runtime Exception"; };
		virtual const char* what() const noexcept override;

	protected:
		std::string m_Message;
	};

	class RAY_EXPORT VulkanException : public RuntimeException
	{
	public:
		VulkanException(const char* message, uint16_t line, const char* file, VkResult result);
		virtual const char* GetType() const { return "Vulkan Exception"; };
		virtual const char* what() const noexcept override;

	private:
		VkResult m_Error;
	};
}  // namespace At0::Ray


#ifdef _MSC_VER
	#define RAY_VK_THROW_NO_EXPR(msg, ...)                                                       \
		throw ::At0::Ray::VulkanException(::At0::Ray::SerializeString(msg, __VA_ARGS__).c_str(), \
			(uint16_t)__LINE__, __FILE__, VK_ERROR_UNKNOWN)

	#define RAY_VK_THROW_FAILED(expr, msg, ...)                                                  \
		if (VkResult RL__VKRES__RL = (expr); RL__VKRES__RL != VK_SUCCESS)                        \
		throw ::At0::Ray::VulkanException(::At0::Ray::SerializeString(msg, __VA_ARGS__).c_str(), \
			(uint16_t)__LINE__, __FILE__, RL__VKRES__RL)

	#define RAY_THROW_RUNTIME(msg, ...)     \
		throw ::At0::Ray::RuntimeException( \
			::At0::Ray::SerializeString(msg, __VA_ARGS__).c_str(), (uint16_t)__LINE__, __FILE__)
#else
	#define RAY_VK_THROW_NO_EXPR(msg, ...)                                                         \
		throw ::At0::Ray::VulkanException(::At0::Ray::SerializeString(msg, ##__VA_ARGS__).c_str(), \
			(uint16_t)__LINE__, __FILE__, VK_ERROR_UNKNOWN)

	#define RAY_VK_THROW_FAILED(expr, msg, ...)                                                    \
		if (VkResult RL__VKRES__RL = (expr); RL__VKRES__RL != VK_SUCCESS)                          \
		throw ::At0::Ray::VulkanException(::At0::Ray::SerializeString(msg, ##__VA_ARGS__).c_str(), \
			(uint16_t)__LINE__, __FILE__, RL__VKRES__RL)

	#define RAY_THROW_RUNTIME(msg, ...)     \
		throw ::At0::Ray::RuntimeException( \
			::At0::Ray::SerializeString(msg, ##__VA_ARGS__).c_str(), (uint16_t)__LINE__, __FILE__)
#endif
