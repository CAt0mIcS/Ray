#pragma once

#include "RRBase.h"


namespace At0::Ray
{
	class RR_API IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(std::initializer_list<uint32_t> indices);

		virtual void Bind() = 0;
	};
}