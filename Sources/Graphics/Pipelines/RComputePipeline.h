#pragma once

#include "RPipeline.h"


namespace At0::Ray
{
	class ComputePipeline : public Pipeline
	{
	public:
		Pipeline::BindPoint GetBindPoint() const override { return Pipeline::BindPoint::Compute; }
	};
}  // namespace At0::Ray
