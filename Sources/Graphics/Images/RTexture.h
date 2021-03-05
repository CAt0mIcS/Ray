#pragma once

#include "../../RBase.h"
#include "RImage2D.h"
#include "../Core/RBindable.h"

#include <string>


namespace At0::Ray
{
	class RAY_EXPORT Texture : public Bindable, NonCopyable
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		void CmdBind(const CommandBuffer& cmdBuff) const override;

		Texture& operator=(Texture&& other) = default;
		Texture(Texture&& other) = default;
	};
}  // namespace At0::Ray
