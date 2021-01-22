#pragma once

#include "RRBase.h"

namespace At0::Ray
{
	class RR_API Topology
	{
	public:
		enum Type
		{
			TriangleList
		};

	public:
		static Scope<Topology> Create(Type type);

		virtual void Bind() = 0;

	protected:
		Topology() = default;
	};
}